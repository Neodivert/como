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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "server.hpp"
#include <common/packets/packets.hpp>
extern "C" {
    #include <endian.h>
}
#include <common/utilities/log.hpp>
#include <clocale>
#include <boost/filesystem.hpp>

#include <common/primitives/obj_primitives_importer.hpp>

int main( int argc, char* argv[] )
{
    char oldPath[256] = {0};
    boost::system::error_code errorCode;

    try {
        if( argc < 4 ){
            std::cerr << "Usage: server <port> <max_users> <scene_name>" << std::endl;
            exit( -1 );
        }

        // We are about to change the current path, so save it as the old one.
        strncpy( oldPath, boost::filesystem::current_path().string().c_str(), 256 );

        // Variable argv[0] contains the string used for invoking this server
        // program. We now retrieve the basename of that string for getting a
        // relative path to the same directory where the server resides.
        std::string serverRelativePath( argv[0] );
        serverRelativePath = serverRelativePath.substr( 0, serverRelativePath.rfind( "/server" ) );

        // Change directory so now we are in the same directory as the server
        // executable (we need this because Server class uses relative paths).
        boost::filesystem::current_path( serverRelativePath, errorCode );
        if( errorCode ){
            throw std::runtime_error( errorCode.message() );
        }

        // Run the server.
        como::Server server( atoi( argv[1] ), atoi( argv[2] ), argv[3], 1 );
        server.run();

    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }

    // The server has been shutdown. Return to the path from where the server
    // was invoked.
    boost::filesystem::current_path( oldPath );
}
