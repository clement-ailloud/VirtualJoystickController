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

#include "qwidget.h"

#include "qpoint.h"

#include <cmath>

class QPainter;
class QMouseEvent;

namespace Controller
{

struct Circle
{
public:

    Circle() : m_radius(0)
    {
    }
//
//    Circle(const Circle& other);
//    Circle& operator=(const Circle& other);
//
//    Circle(Circle&& other);
//    Circle& operator=(Circle&& other);
//
//    ~Circle();

    int radius() const
    {
        return m_radius;
    }

    void setRadius(int radius)
    {
        m_radius = radius;
    }

    bool contains(int x, int y) const
    {
        return pow(x, 2) + pow(y, 2) <= pow(m_radius, 2);
    }

private:

    int m_radius;
};


/**
 * @class Joystick
 *
 * @brief   Virtual joystick widget
 *
 *          The controller is based on an electronical joystick controller. It has been design in
 *          such way to reproduce partially the same behavior:
 *          - Rest position (that means it gets back the the center if the joystick is released)
 *          - The range of the X-value and a Y-value is 0 - 255
 *
 *          Major innovation:
 *
 *          A mode has been added to restrict the degree of freedom (see JoystickMode)
 */

class Joystick : /*public IVirtualController,*/ public QWidget
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Joystick)

public:


    Joystick(QWidget* parent = nullptr);


    ~Joystick() override;

    /**
     * @brief   Returns the X-value
     */
    char xValue() const;

    /**
     * @brief   Sets the X-value
     */
    void setXValue(char x);

    /**
     * @brief   Returns the Y-value
     */
    char yValue() const;

    /**
     * @brief   Sets the Y-value
     */
    void setYValue(char y);

    bool backToZero() const;
    void setBackToZero(bool enable);

    /**
     * @enum    JoystickMode
     *
     * @brief   Provides degree of freedom management
     */
    enum JoystickMode { NoAxis, XAxisOnly, YAxisOnly, AllAxis };

    /**
     * @brief   Return the degree of freedom currently allowed
     */
    JoystickMode mode() const;

    /**
     * @brief   Set the degree of freedom
     */
    void setMode(JoystickMode degree);

protected:

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

    void initPainter(QPainter* painter) const override;

signals:

    void pressed() const;
    void valueChanged(char x, char y) const;

private:

    char m_x; //<! Byte value of the joystick controller for the X-axis 
    char m_y; //<! Byte value of the joystick controller for the Y-axis 

    bool m_backToZero; //<! Says whether the joystick should get back to the rest position or not

    JoystickMode m_mode; //<! Degree of freedom

    Circle m_joystick;
    Circle m_controller;
    QPoint m_controllerPosition;
};

#define DEFINE_ENUM_OR_OPERATOR(T)                                        \
inline T operator|(T lhs, T rhs)                                      \
{                                                                         \
    return static_cast<T>(static_cast<int>(lhs) | static_cast<int>(rhs)); \
}                                                                         \

DEFINE_ENUM_OR_OPERATOR(Joystick::JoystickMode)

//inline Joystick::JoystickMode operator|(Joystick::JoystickMode lhs, Joystick::JoystickMode rhs)
//{
//    return static_cast<Joystick::JoystickMode>(static_cast<int>(lhs) | static_cast<int>(rhs));
//}

inline Joystick::JoystickMode operator&(Joystick::JoystickMode lhs, Joystick::JoystickMode rhs)
{
    return static_cast<Joystick::JoystickMode>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

}

#endif

