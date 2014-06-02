/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "material_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MaterialCommand::MaterialCommand( MaterialCommandType commandType ) :
    TypeCommand( CommandTarget::MATERIAL, commandType, NO_USER )
{
    addPackable( &materialID_ );
}

MaterialCommand::MaterialCommand( MaterialCommandType commandType, const MaterialID& materialID ) :
    TypeCommand( CommandTarget::MATERIAL, commandType, materialID.getCreatorID() ),
    materialID_( materialID )
{
    addPackable( &materialID_ );
}

MaterialCommand::MaterialCommand( const MaterialCommand& b ) :
    TypeCommand( b ),
    materialID_( b.materialID_ )
{
    addPackable( &materialID_ );
}


/***
 * 3. Getters
 ***/

MaterialID MaterialCommand::getMaterialID() const
{
    return materialID_.getValue();
}

} // namespace como
