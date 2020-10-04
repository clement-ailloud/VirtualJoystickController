/*!
 *
 * @file    main.cpp
 *
 * @date    22 october, 2018
 *
 * @version 1.0
 *
 * @author  Clement Ailloud
 *
 * @brief   Virtual joystick controller test
 *
 **/


#include "qapplication.h"

#include "virtual_joystick_controller.h"

#include "qcolor.h"
#include "qpalette.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Controller::Joystick joystick;
    joystick.setPalette(QPalette(QColor("#003366")));
    joystick.showMaximized();

    return a.exec();
}
