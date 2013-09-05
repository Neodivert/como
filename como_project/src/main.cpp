/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <QGuiApplication>
#include "src/gui/main_window.hpp"
#include <glm/gtx/vector_angle.hpp>
#include "managers/tester.hpp"

#define TESTING_MODE 1

int main( int argc, char *argv[] )
{
    // Create a Qt application.
    QApplication app( argc, argv );

#ifdef TESTING_MODE
    // Run tests.
    como::Tester* tester = como::Tester::getInstance();
    tester->testMeshes();
    tester->destroy();
#else
    // Create a Qt window and show it.
    como::MainWindow window;
    window.show();

    // Run Qt application.
    return app.exec();
#endif
}

