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

#include "drawable_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DrawableCommand::DrawableCommand( DrawableCommandType drawableCommandType, UserID userID, DrawableID drawableID ) :
    Command( CommandTarget::DRAWABLE, userID ),
    commandType_( drawableCommandType ),
    drawableID_( drawableID )
{
}


DrawableCommand::DrawableCommand( const DrawableCommand& b ) :
    Command( b ),
    commandType_( b.commandType_ ),
    drawableID_( b.drawableID_ )
{
}


/***
 * 3. Packing and unpacking
 ***/

char* DrawableCommand::pack( char* buffer ) const
{
    // Pack Command attributes.
    buffer = Command::pack( buffer );

    // Pack DrawableCommand attributes.
    packer::pack( static_cast< std::uint8_t >( commandType_ ), buffer );
    packer::pack( drawableID_.creatorID, buffer );
    packer::pack( drawableID_.drawableIndex, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


const char* DrawableCommand::unpack( const char* buffer )
{
    std::uint8_t commandType;

    // Unpack Command attributes.
    buffer = Command::unpack( buffer );

    // Unpack DrawableCommand ID.
    packer::unpack( commandType, buffer );

    // TODO: Remove this check in future versions.
    if( commandType_ != static_cast< DrawableCommandType >( commandType ) ){
        throw std::runtime_error( "ERROR: Unexpected DrawableCommand type" );
    }

    packer::unpack( drawableID_.creatorID, buffer );
    packer::unpack( drawableID_.drawableIndex, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


/***
 * 4. Getters
 ***/

DrawableCommandType DrawableCommand::getType() const
{
    return commandType_;
}


std::uint16_t DrawableCommand::getPacketSize() const
{
    return ( Command::getPacketSize() +
             sizeof( commandType_ ) +
             sizeof( drawableID_.creatorID ) +
             sizeof( drawableID_.drawableIndex ) );
}


DrawableID DrawableCommand::getDrawableID() const
{
    return drawableID_;
}


/***
 * 5. Buffer pre reading
 ***/

DrawableCommandType DrawableCommand::getType( const char* buffer )
{
    return static_cast< DrawableCommandType >( *( reinterpret_cast< const std::uint8_t* >( buffer ) ) );
}


/***
 * 6. Setters
 ***/

void DrawableCommand::setDrawableID( const DrawableID& drawableID )
{
    drawableID_ = drawableID;
}


} // namespace como
