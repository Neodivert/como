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

#include "packable_material_id.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PackableMaterialID::PackableMaterialID( const MaterialID& materialID ) :
    AbstractPackableWrapper<MaterialID>(),
    materialID_( materialID )
{}

PackableMaterialID::PackableMaterialID( const PackableMaterialID& b ) :
    AbstractPackableWrapper<MaterialID>( b ),
    materialID_( b.materialID_ )
{}


/***
 * 3. Getters
 ***/

MaterialID PackableMaterialID::getValue() const
{
    return materialID_;
}

std::uint16_t PackableMaterialID::getPacketSize() const
{
    return sizeof( materialID_.getCreatorID() ) + sizeof( materialID_.getMaterialIndex() );
}


/***
 * 4. Setters
 ***/

void PackableMaterialID::setValue( MaterialID value )
{
    materialID_ = value;
}


/***
 * 5. Packing and unpacking
 ***/

void* PackableMaterialID::pack( void* buffer ) const
{
    PackableUserID userID( materialID_.getCreatorID() );
    PackableMaterialIndex materialIndex( materialID_.getMaterialIndex() );

    buffer = userID.pack( buffer );
    buffer = materialIndex.pack( buffer );

    return buffer;
}

const void* PackableMaterialID::unpack( const void* buffer )
{
    PackableUserID userID( materialID_.getCreatorID() );
    PackableMaterialIndex materialIndex( materialID_.getMaterialIndex() );

    buffer = userID.unpack( buffer );
    buffer = materialIndex.unpack( buffer );

    return buffer;
}

const void* PackableMaterialID::unpack( const void* buffer ) const
{
    const PackableUserID userID( materialID_.getCreatorID() );
    const PackableMaterialIndex materialIndex( materialID_.getMaterialIndex() );

    buffer = userID.unpack( buffer );
    buffer = materialIndex.unpack( buffer );

    return buffer;
}


/***
 * 6. Operators
 ***/

PackableMaterialID& PackableMaterialID::operator = ( const MaterialID& materialID )
{
    materialID_ = materialID;

    return *this;
}

} // namespace como
