/*!
 *
 * @file        joystick.cpp
 *
 * @date        22 october, 2018
 *
 * @version     1.0
 *
 * @author      Clement Ailloud
 *
 * @brief       Source file for virtual joystick controller
 *
 * @copyright   Copyright (C) 2018  Clement Ailloud
 *
 *              This program is free software; you can redistribute it and/or
 *              modify it under the terms of the GNU General Public License
 *              as published by the Free Software Foundation; either version 2
 *              of the License, or (at your option) any later version.
 *
 *              This program is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *              GNU General Public License for more details.
 *
 * @class Joystick
 *
 * @brief   Virtual joystick widget
 *
 *          The controller is based on an electronical joystick controller. It
 * has been designed in such way it reproduces the same behavior:
 *          - The range of the X-value and a Y-value is between [0;255]
 *          - It comes back to rest position by default, however this behavior
 *          can also be disabled. The position of the joystick then remains once
 *          it is released.
 *
 * The widget also enables you to restrict the degree of freedom (see
 * JoystickMode).
 */

#include "virtual_joystick_controller.h"

#include "QMouseEvent"
#include "QRadialGradient"
#include "qdebug.h"
#include "qpainter.h"

#include <cmath>

using namespace Controller;

Joystick::Joystick(QWidget *parent)
    : QWidget(parent)
    , m_x{0}
    , m_y{0}
    , m_backToZero(true)
    , m_mode(AllAxis)
{
    setAttribute(Qt::WA_TranslucentBackground);
}

Joystick::~Joystick() = default;

/**
 * @brief   Returns the X-value
 */
char Joystick::x() const
{
    return m_x;
}

/**
 * @brief   Sets the X-value
 */
void Joystick::setX(char x)
{
    if (x != m_x)
    {
        m_x = x;
        update();
    }
}

/**
 * @brief   Returns the Y-value
 */
char Joystick::y() const
{
    return m_y;
}

/**
 * @brief   Sets the Y-value
 */
void Joystick::setY(char y)
{
    if (y != m_y)
    {
        m_y = y;
        update();
    }
}

/**
 * @brief   Return the degree of freedom currently allowed
 */
Joystick::JoystickMode Joystick::mode() const
{
    return m_mode;
}

/**
 * @brief   Set the degree of freedom
 */
void Joystick::setMode(Joystick::JoystickMode mode)
{
    m_mode = mode;

    update();
}

void Joystick::setBackToZero(bool enable)
{
    m_backToZero = enable;
}

bool Joystick::backToZero() const
{
    return m_backToZero;
}

void Joystick::initPainter(QPainter *painter) const
{
    // Enable antialiasing rendering
    painter->setRenderHint(QPainter::Antialiasing);

    // Disable pen color for transparent border
    painter->setPen(Qt::NoPen);

    // Half opacity for reducing color contrast
    //    painter->setOpacity(0.5);

    // Color selection for filling joystick
    painter->setBrush(QBrush(QWidget::palette().color(QWidget::backgroundRole())));
}

void drawCircle(QPainter &painter, const QPoint &center, int r)
{
    painter.drawEllipse(center, r, r);
}

QPoint getCenter(int width, int height)
{
    return {width / 2, height / 2};
}

void drawDefaultJoystickController(QPainter &painter, const Circle &circle, const QColor &color) {}

void Joystick::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    const auto jr = m_joystick.radius();
    const auto center = getCenter(QWidget::width(), QWidget::height());

    {
        // Color filling the background
        QRadialGradient gradient(center, jr);
        QColor color("#004d99");
        color.setAlpha(255);
        gradient.setColorAt(0, color);
        color.setAlpha(224);
        gradient.setColorAt(1, color);

        painter.setBrush(QBrush(gradient));
        drawCircle(painter, center, jr);
    }

    {
        // Color filling joystick controller
        QColor color("#FFFFFF");
        painter.setBrush(QBrush(color));

        const auto cr = m_controller.radius();
        drawCircle(painter, m_controllerPosition, cr);
    }
}

bool contains(int x, int y, int radius)
{
    return (pow(x, 2) + pow(y, 2) <= pow(radius, 2));
}

bool contains(const QPoint &pos, int radius)
{
    return (pow(pos.x(), 2) + pow(pos.y(), 2) <= pow(radius, 2));
}

QPoint translateTopLeftToCenter(const QPoint &pos, const QSize &boundingSize)
{
    // Recenter coordinates
    return {pos.x() - boundingSize.width() / 2, pos.y() - boundingSize.height() / 2};
}

void Joystick::mousePressEvent(QMouseEvent *event)
{
    const auto pos = translateTopLeftToCenter({event->x(), event->y()});

    if (contains(pos, m_controller.radius()))
    {
        emit pressed();
    }

    update();
}

void Joystick::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
    {
        return;
    }

    const QPoint relativeMousePosition(event->x(), event->y());

    // Recenter coordinates
    const auto x = relativeMousePosition.x() - width() / 2;
    const auto y = relativeMousePosition.y() - height() / 2;

    switch (m_mode)
    {
    case AllAxis:
    {
        if (contains(x, y, m_joystick.radius()))
        {
            m_controllerPosition = std::move(relativeMousePosition);
        }
        else
        {
            const auto radius = std::sqrt(std::pow(x, 2) + std::pow(y, 2));

            auto angle = acos(x / radius);
            // or angle = asin(y / radius);

            if (y > 0)
            {
                angle = angle + (M_PI - angle) * 2.0;
            }

            // Get opposite of angle because Qt system coordinate is inverted
            angle = -angle;

            m_controllerPosition.setX(m_joystick.radius() * cos(angle) + width() / 2);
            m_controllerPosition.setY(m_joystick.radius() * sin(angle) + height() / 2);
        }
    }
    break;
    case XAxisOnly:
    {
        if (x < m_joystick.radius() && x > -m_joystick.radius())
        {
            m_controllerPosition.setX(relativeMousePosition.x());
        }
        else
        {
            if (x > 0)
            {
                m_controllerPosition.setX(width() / 2 + m_joystick.radius());
            }
            else
            {
                m_controllerPosition.setX(width() / 2 - m_joystick.radius());
            }
        }
    }
    break;
    case YAxisOnly:
    {
        if (y < m_joystick.radius() && y > -m_joystick.radius())
        {
            m_controllerPosition.setY(relativeMousePosition.y());
        }
        else
        {
            if (y > 0)
            {
                m_controllerPosition.setY(height() / 2 + m_joystick.radius());
            }
            else
            {
                m_controllerPosition.setY(height() / 2 - m_joystick.radius());
            }
        }
    }
    break;
    default:
        throw;
    }

    emit valueChanged(x, y);

    update();
}

void Joystick::mouseReleaseEvent(QMouseEvent * /*event*/)
{
    if (m_backToZero)
    {
        // Put controller back to joystick center
        m_controllerPosition = getCenter(QWidget::width(), QWidget::height());
        update();
    }
}

int getRadius(int width, int height)
{
    return ((qMin(width, height)) / 2) - ((qMin(width, height)) / 6);
}

void Joystick::resizeEvent(QResizeEvent * /*event*/)
{
    const auto radius = getRadius(QWidget::width(), QWidget::height());
    m_joystick.setRadius(radius);
    m_controller.setRadius(radius / 2);

    m_controllerPosition = getCenter(QWidget::width(), QWidget::height());
}
