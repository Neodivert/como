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

#include "user_accepted.hpp"

namespace como {


/***
 * 1. Packing and unpacking
 ***/

void UserAccepted::pack( char* code ) const
{
    unsigned int i = 0;
    std::uint8_t* colorPtr = nullptr;

    // Place the user's id at the beginning of the stream.
    ( reinterpret_cast< std::uint32_t* >( code ) )[0] = translateToNetworkOrder( id );

    // Place the user's name after the id in the stream.
    strncpy( &code[4], name, NAME_SIZE );

    // Place the user's selection color at the end.
    colorPtr = reinterpret_cast< std::uint8_t* >( &code[4+NAME_SIZE] );
    for( ; i < 4; i++ ){
        *colorPtr = selectionColor[i];

        colorPtr++;
    }
}

void UserAccepted::unpack( const char* code )
{
    const std::uint8_t* colorPtr = nullptr;
    unsigned int i = 0;

    // Get the user's id.
    id = translateFromNetworkOrder( ( reinterpret_cast< const std::uint32_t* >( code ) )[0] );

    // Get the user's name.
    strncpy( name, &code[4], NAME_SIZE );

    // Get the user's selection color.
    colorPtr = reinterpret_cast< const std::uint8_t* >( &code[4+NAME_SIZE] );
    for( ; i<4; i++ ){
        selectionColor[i] = *colorPtr;

        colorPtr++;
    }
}


/***
 * 2. Getters
 ***/

std::uint16_t UserAccepted::getCodeSize()
{
    return sizeof( id ) + sizeof( char ) * NAME_SIZE + sizeof( uint8_t ) * 4;
}

} // namespace como
