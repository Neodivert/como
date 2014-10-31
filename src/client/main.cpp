/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifdef _WIN32
#include <winsock2.h>
#define WIN32_LEAN_AND_MEAN
#endif

#include <client/gui/application.hpp>
#include "gui/main_window.hpp"
#include "gui/connection_wizard/connection_wizard.hpp"
#include "managers/como_app.hpp"
#include <boost/filesystem.hpp>

#include <common/primitives/primitive_file.hpp>
#include <client/managers/scene/scene.hpp>

extern "C" {
    #include <SDL2/SDL_image.h>
}

int main( int argc, char *argv[] )
{
    // TODO: Add checking code.
    IMG_Init( IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF );
    atexit( IMG_Quit );

    int mainExitValue = 0;

    boost::system::error_code errorCode;

    // Create a Qt application.
    como::Application app( argc, argv );

    // Variable argv[0] contains the std::string used for invoking this program
    // We now retrieve the basename of that std::string for getting a
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

    como::LogPtr log( new como::Log() );

    // Create a wizzard for creating a scene or connecting to a created one.
    como::ConnectionWizard connectionWizard( log );

    // Execute the connection wizard, then show the main window.
    como::ScenePtr scene = connectionWizard.run();

    // If the user doesn't reject the connection wizard, show the main window
    // and run the app.
    if( scene != nullptr ){
        // Create the COMO app.
        std::shared_ptr< como::ComoApp > comoApp( new como::ComoApp( scene, log ) );

        // Create and show the main window.
        como::MainWindow window( nullptr, comoApp );
        window.show();

        // "Run" the scene (start synchronization with server).
        comoApp->getScene()->run();

        // Run Qt application.
        mainExitValue = app.exec();
    }

    return mainExitValue;
}
