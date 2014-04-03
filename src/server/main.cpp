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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "server.hpp"
#include "../common/packets/packets.hpp"
#include <endian.h>
#include "../common/utilities/log.hpp"
#include <clocale>

int main( int argc, char* argv[] )
{
    try {
        if( argc < 4 ){
            std::cerr << "Usage: server <port> <max_users> <scene_name>" << std::endl;
            exit( -1 );
        }

        como::Server server( atoi( argv[1] ), atoi( argv[2] ), argv[3], 1 );
        server.run();
    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}
