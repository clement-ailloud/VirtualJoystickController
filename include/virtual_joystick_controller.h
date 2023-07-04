/**
 * @file        joystick.h
 *
 * @date        22 october, 2018
 *
 * @version     1.0
 *
 * @author      Clement Ailloud
 *
 * @brief       Header file for virtual joystick controller
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
 **/

#ifndef JOYSTICK_WIDGET_H
#define JOYSTICK_WIDGET_H

#include "qpoint.h"
#include "qwidget.h"

#include <cmath>

class QPainter;
class QMouseEvent;

namespace Controller
{

struct Point
{
    int x, y;
};

struct Circle
{
public:
    explicit Circle()
        : m_radius(0)
    {
    }

    Point center() const { return m_center; }
    void setCenter(Point pos) { m_center = std::move(pos); }

    int radius() const { return m_radius; }
    void setRadius(int radius) { m_radius = radius; }

private:
    int m_radius;
};

class Joystick : public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Joystick)

public:
    Joystick(QWidget *parent = nullptr);

    ~Joystick() override;

    char x() const;
    void setX(char x);

    char y() const;
    void setY(char y);

    bool backToZero() const;
    void setBackToZero(bool enable);

    /**
     * @enum    JoystickMode
     *
     * @brief   Provides degree of freedom management
     */
    enum JoystickMode
    {
        NoAxis,
        XAxisOnly,
        YAxisOnly,
        AllAxis
    };

    JoystickMode mode() const;
    void setMode(JoystickMode degree);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void initPainter(QPainter *painter) const override;

signals:

    void pressed() const;
    void valueChanged(char x, char y) const;

private:
    char m_x; //<! Byte value of the joystick controller for X-axis
    char m_y; //<! Byte value of the joystick controller for Y-axis

    bool m_backToZero; //<! Says whether the joystick should get back to rest
                       // position
                       // once released.

    JoystickMode m_mode; //<! Degree of freedom

    Circle m_joystick;
    Circle m_controller;
    QPoint m_controllerPosition;
};

#define DEFINE_ENUM_OR_OPERATOR(T)                                                                 \
    inline T operator|(T lhs, T rhs)                                                               \
    {                                                                                              \
        return static_cast<T>(static_cast<int>(lhs) | static_cast<int>(rhs));                      \
    }

DEFINE_ENUM_OR_OPERATOR(Joystick::JoystickMode)

// inline Joystick::JoystickMode operator|(Joystick::JoystickMode lhs,
// Joystick::JoystickMode rhs)
//{
//     return static_cast<Joystick::JoystickMode>(static_cast<int>(lhs) |
//     static_cast<int>(rhs));
// }

inline Joystick::JoystickMode operator&(Joystick::JoystickMode lhs, Joystick::JoystickMode rhs)
{
    return static_cast<Joystick::JoystickMode>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

} // namespace Controller

#endif
