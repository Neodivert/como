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

int main( int argc, char *argv[] )
{
    /*
    const glm::vec3 yAxis( 0.0f, 1.0f, 0.0f );
    const glm::vec3 zAxis( 0.0f, 0.0f, 1.0f );

    glm::vec4 up( 0.0f, 0.0f, 0.0f, 1.0f );

    glm::mat4 totalRotation = glm::mat4( 1.0f );
    glm::mat4 rotation45 = glm::rotate( glm::mat4( 1.0f ), 45.0f, zAxis );

    for( unsigned int i = 0; i<8; i++ ){
        cout << "Rotating: " << ((i+1)*45.0f) << " dregrees" << endl;

        totalRotation = rotation45 * totalRotation;

        up = totalRotation * glm::vec4( yAxis, 1.0f );

        cout << "Resulting Up: (" << up.x << ", " << up.y << ", " << up.z << ")" << endl;

        cout << "Resulting angle (up - Y): " << glm::angle( glm::normalize( glm::vec3( up ) ), yAxis ) << endl << endl;
    }
    */

    // Create a Qt application.
    QApplication app( argc, argv );

    // Create a Qt window and show it.
    como::MainWindow window;
    window.show();
    
    // Run Qt application.
    return app.exec();
}
