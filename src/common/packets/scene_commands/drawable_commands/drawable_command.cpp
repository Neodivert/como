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

#include "drawable_command.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

DrawableCommand::DrawableCommand( DrawableID drawableID, SceneCommandType sceneCommandType ) :
    SceneCommand( sceneCommandType ),
    drawableID_( drawableID )
{
}


DrawableCommand::DrawableCommand( const DrawableCommand& b ) :
    SceneCommand( b ),
    drawableID_( b.drawableID_ )
{
}


/***
 * 2. Packing and unpacking
 ***/

char* DrawableCommand::pack( char* buffer ) const
{
    // Pack the SceneCommand fields.
    buffer = SceneCommand::pack( buffer );

    // Pack the drawable ID.
    packer::pack( drawableID_.creatorID, buffer );
    packer::pack( drawableID_.drawableIndex, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


const char* DrawableCommand::unpack( const char* buffer )
{
    // Unpack the SceneCommand fields.
    buffer = SceneCommand::unpack( buffer );

    // Unpack the drawable ID.
    packer::unpack( drawableID_.creatorID, buffer );
    packer::unpack( drawableID_.drawableIndex, buffer );

    // Return the buffer updated pointer.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t DrawableCommand::getPacketSize() const
{
    return ( sizeof( drawableID_.creatorID ) + sizeof( drawableID_.drawableIndex ) );
}


DrawableID DrawableCommand::getDrawableID() const
{
    return drawableID_;
}


/***
 * 4. Setters
 ***/

void DrawableCommand::setDrawableID( const DrawableID& drawableID )
{
    drawableID_ = drawableID;
}


} // namespace como
