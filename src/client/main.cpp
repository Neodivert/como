/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include <QGuiApplication>
#include "gui/main_window.hpp"
#include <glm/gtx/vector_angle.hpp>
#include "managers/tester.hpp"
#include "gui/connection_wizard/connection_wizard.hpp"
#include "managers/como_app.hpp"

//#define TESTING_MODE 1

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <clocale>

int main( int argc, char *argv[] )
{
    // Create a Qt application.
    QApplication app( argc, argv );
    int dialogCode;

    // Create the COMO app.
    std::shared_ptr< como::ComoApp > comoApp( new como::ComoApp );

    // Create the main window.
    como::MainWindow window( nullptr, comoApp );

    // Create a wizzard for creating a scene or connecting to a created one.
    como::ConnectionWizard connectionWizard( comoApp->getScene(), comoApp->getLog(), &window );

    // Execute the connection wizard, then show the main window.
    dialogCode = connectionWizard.exec();

    // If the user doesn't reject the connection wizard, show the main window
    // and run the app.
    if( dialogCode != QDialog::Rejected ){
        window.show();

        // Run Qt application.
        return app.exec();
    }
}
