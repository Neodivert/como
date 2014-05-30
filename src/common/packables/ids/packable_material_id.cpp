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

PackableMaterialID::PackableMaterialID() :
    CompositePackable()
{
    addPackable( &creatorID_ );
    addPackable( &materialIndex_ );
}


PackableMaterialID::PackableMaterialID( const MaterialID& materialID ) :
    CompositePackable(),
    creatorID_( materialID.getCreatorID() ),
    materialIndex_( materialID.getMaterialIndex() )
{
    addPackable( &creatorID_ );
    addPackable( &materialIndex_ );
}

PackableMaterialID::PackableMaterialID( const PackableMaterialID& b ) :
    CompositePackable( b ),
    creatorID_( b.creatorID_ ),
    materialIndex_( b.materialIndex_ )
{
    addPackable( &creatorID_ );
    addPackable( &materialIndex_ );
}


/***
 * 3. Getters
 ***/

MaterialID PackableMaterialID::getValue() const
{
    return MaterialID( creatorID_.getValue(), materialIndex_.getValue() );
}


/***
 * 4. Setters
 ***/

void PackableMaterialID::setValue( MaterialID materialID )
{
    creatorID_ = materialID.getCreatorID();
    materialIndex_ = materialID.getMaterialIndex();
}


/***
 * 5. Operators
 ***/

PackableMaterialID& PackableMaterialID::operator = ( const MaterialID& materialID )
{
    setValue( materialID );

    return *this;
}

} // namespace como
