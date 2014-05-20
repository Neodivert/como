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
#include "gui/connection_wizard/connection_wizard.hpp"
#include "managers/como_app.hpp"
#include <boost/filesystem.hpp>

//#define TESTING_MODE 1

#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <clocale>

#include <common/packables/packable_float.hpp>

int main( int argc, char *argv[] )
{
    boost::system::error_code errorCode;

    // Create a Qt application.
    QApplication app( argc, argv );
    int dialogCode;

    // Variable argv[0] contains the string used for invoking this program
    // We now retrieve the basename of that string for getting a
    // relative path to the same directory where the client resides.
    // FIXME: This won't work if argv[0] points to a symlink, because we'd go
    // to the same directory where the symlink resides, not to the same
    // directory where the actual executable resides.
    // (The same for server program).
    std::string programRelativePath( argv[0] );
    programRelativePath = programRelativePath.substr( 0, programRelativePath.rfind( "/" ) );

    // Change directory so now we are in the same directory as the client
    // executable (we need this because this program uses relative paths).
    boost::filesystem::current_path( programRelativePath, errorCode );
    if( errorCode ){
        throw std::runtime_error( errorCode.message() );
    }

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
