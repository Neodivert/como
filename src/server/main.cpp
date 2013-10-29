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

#include "server.hpp"
#include "../common/packets/packets.hpp"
#include <endian.h>

int main( int argc, char* argv[] )
{
    try {
        /*
        unsigned int i = 0;
        como::UserConnected p1, p2;

        char buffer[256];
        const std::uint8_t* selectionColor;

        p1.setUserID( 5 );
        p1.setName( "Neodivert" );
        p1.setSelectionColor( 255, 150, 123, 35 );
        p1.pack( buffer );

        p2.unpack( buffer );
        std::cout << "P2" << std::endl
                  << "--------------------------" << std::endl
                  << "ID: " << p2.getUserID() << std::endl
                  << "Name: " << p2.getName() << std::endl
                  << "SelectionColor: ";
        selectionColor = p2.getSelectionColor();
        for( ; i<4; i++ ){
            std::cout << (unsigned int)selectionColor[i] << ", ";
        }
        std::cout << "--------------------------" << std::endl;


        std::uint32_t value = 0x01000000;

        std::cout << "LITTLE_ENDIAN: " << value << std::endl
                  << "BIG_ENDIAN: " << como::packer::translateToNetworkOrder( value ) << std::endl;
        //char buffer[256];
        */
        /*

        como::UserAccepted p;
        p.id = 32;
        strcpy( p.name, "Neodivert" );
        p.selectionColor[0] = 0;
        p.selectionColor[1] = 255;
        p.selectionColor[2] = 0;
        p.selectionColor[3] = 255;

        p.encode( buffer );

        unsigned int i;
        for( i=0; i<4; i++ ){
            std::cout << "[" << i << "]: " << (int)buffer[i] << std::endl;
        }
        for( ; i<68; i++ ){
            std::cout << "[" << i << "]: " << buffer[i] << std::endl;
        }
        for( ; i<p.getCodeSize(); i++ ){
            std::cout << "\t[" << i << "]: " << (int)buffer[i] << std::endl;
        }

        como::UserAccepted p1;

        p1.decode( buffer );

        std::cout << p1.id << std::endl
                  << p1.name << std::endl
                  << "(" << p1.selectionColor[0] << ", " << p1.selectionColor[1] << ", " << p1.selectionColor[2] << ", " << p1.selectionColor[3] << ")" << std::endl;
        */

        if( argc < 3 ){
            std::cerr << "Usage: server <port> <max_users>" << std::endl;
            exit( -1 );
        }

        como::Server server( atoi( argv[1] ), atoi( argv[2] ), 1 );
        server.run();
    }catch (std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

