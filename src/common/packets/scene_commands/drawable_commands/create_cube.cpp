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

#include "create_cube.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

CreateCube::CreateCube() :
    DrawableCommand( SceneCommandType::CREATE_CUBE )
{
}


CreateCube::CreateCube( UserID userID, DrawableID drawableID, const std::uint8_t* color ) :
    DrawableCommand( userID, drawableID, SceneCommandType::CREATE_CUBE )
{
    setColor( color );
}

CreateCube::CreateCube( const CreateCube& b ) :
    DrawableCommand( b )
{
    setColor( b.color_ );
}


/***
 * 2. Packing and unpacking
 ***/

char* CreateCube::pack( char* buffer ) const
{
    unsigned int i;

    char* auxBuffer = buffer;

    // Pack the DrawableCommand struct fields.
    buffer = DrawableCommand::pack( buffer );

    // Pack the color.
    for( i = 0; i < 4; i++ ){
        packer::pack( color_[i], buffer );
        std::cout << (int)(color_[i]) << ":(" << (int)(*(buffer - 1)) << ")" << std::endl;
    }


    std::cout << "(CREATE_CUBE) - auxBuffer: ";
    for( unsigned int i = 0; i < getPacketSize(); i++ ){
        std::cout << (int)(auxBuffer[i]) << ", ";
    }
    std::cout << std::endl;

    // Return the updated buffer pointer.
    return buffer;
}


const char* CreateCube::CreateCube::unpack( const char* buffer )
{
    unsigned int i;

    std::cout << "(CREATE_CUBE) - buffer: ";
    for( unsigned int i = 0; i < getPacketSize(); i++ ){
        std::cout << (int)(buffer[i]) << ", ";
    }
    std::cout << std::endl;

    // Unpack the DrawableCommand struct fields.
    buffer = DrawableCommand::unpack( buffer );

    // Unpak the color.
    for( i = 0; i < 4; i++ ){
        packer::unpack( color_[i], buffer );
        std::cout << (int)( color_[i] ) << ":(" << (int)(*(buffer - 1)) << ")" << std::endl;
    }

    // Return the updated buffer pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t CreateCube::getPacketSize() const
{
    return ( DrawableCommand::getPacketSize() +
             sizeof( color_[0] ) * 4 );
}


SceneCommandType CreateCube::getType() const
{
    return SceneCommandType::CREATE_CUBE;
}


const std::uint8_t* CreateCube::getColor() const
{
    return color_;
}


/***
 * 4. Setters
 ***/

void CreateCube::setColor( const std::uint8_t* color )
{
    unsigned int i = 0;
    for( ; i < 4; i++ ){
        color_[i] = color[i];
    }
}

} // namespace como
