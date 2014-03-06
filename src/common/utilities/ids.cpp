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

#include "ids.hpp"

namespace como {

/***
 * 1. Construction
 ***/


PackableDrawableID::PackableDrawableID() :
    creatorID( 0 ),
    drawableIndex( 0 )
{
    addBodyPackable( &creatorID );
    addBodyPackable( &drawableIndex );
}


PackableDrawableID::PackableDrawableID( const PackableDrawableID& b ) :
    CompositePackable(),
    creatorID( b.creatorID ),
    drawableIndex( b.drawableIndex )
{
    addBodyPackable( &creatorID );
    addBodyPackable( &drawableIndex );
}


PackableDrawableID::PackableDrawableID( PackableDrawableID&& b ) :
    CompositePackable(),
    creatorID( b.creatorID ),
    drawableIndex( b.drawableIndex )
{
    addBodyPackable( &creatorID );
    addBodyPackable( &drawableIndex );
}


/***
 * 3. Operators
 ***/

bool PackableDrawableID::operator < ( const PackableDrawableID& b ) const
{
    return ( ( creatorID.getValue() < b.creatorID.getValue() ) ||
             ( (creatorID.getValue() == b.creatorID.getValue()) && (drawableIndex.getValue() < b.drawableIndex.getValue()) ) );
}

bool PackableDrawableID::operator == ( const PackableDrawableID& b ) const
{
    return ( ( creatorID.getValue() == b.creatorID.getValue() ) &&
             (drawableIndex.getValue() == b.drawableIndex.getValue() ) );
}


PackableDrawableID& PackableDrawableID::operator = (const PackableDrawableID& b )
{
    if( this != &b ){
        creatorID = b.creatorID;
        drawableIndex = b.drawableIndex;
    }

    return *this;
}

} // namespace como
