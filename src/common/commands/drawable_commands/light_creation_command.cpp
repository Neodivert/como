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

#include "light_creation_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

LightCreationCommand::LightCreationCommand( LightType lightType, PackableDrawableID drawableID, const MaterialID& materialID, const PackableColor& lightColor ) :
    MeshCreationCommand( MeshType::LIGHT, drawableID, materialID ),
    lightType_( lightType ),
    lightColor_( lightColor )
{
    addPackable( &lightType_ );
    addPackable( &lightColor_ );
}


LightCreationCommand::LightCreationCommand( const LightCreationCommand& b ) :
    MeshCreationCommand( b ),
    lightType_( b.lightType_ ),
    lightColor_( b.lightColor_ )
{
    addPackable( &lightType_ );
    addPackable( &lightColor_ );
}


/***
 * 3. Getters
 ***/

LightType LightCreationCommand::getLightType() const
{
    return lightType_.getValue();
}


const PackableColor& LightCreationCommand::getLightColor() const
{
    return lightColor_;
}


/***
 * 4. Buffer pre-reading
 ***/

LightType LightCreationCommand::getLightType( const void* buffer )
{
    MeshCreationCommand meshCreationCommand( MeshType::LIGHT, NULL_DRAWABLE_ID, MaterialID() );

    return static_cast< LightType >( static_cast< const std::uint8_t* >( buffer )[meshCreationCommand.getPacketSize()] );
}

} // namespace como
