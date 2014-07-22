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

#include "abstract_material_modification_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

AbstractMaterialModificationCommand::AbstractMaterialModificationCommand( UserID userID, PackableMaterialParameterName parameterName ) :
    MaterialCommand( MaterialCommandType::MATERIAL_MODIFICATION, userID, ResourceID() ),
    parameterName_( parameterName )
{
    addPackable( &parameterName_ );
}


AbstractMaterialModificationCommand::AbstractMaterialModificationCommand( UserID userID, ResourceID materialID, PackableMaterialParameterName parameterName ) :
    MaterialCommand( MaterialCommandType::MATERIAL_MODIFICATION, userID, materialID ),
    parameterName_( parameterName )
{
    addPackable( &parameterName_ );
}


AbstractMaterialModificationCommand::AbstractMaterialModificationCommand( const AbstractMaterialModificationCommand& b ) :
    MaterialCommand( b ),
    parameterName_( b.parameterName_ )
{
    addPackable( &parameterName_ );
}


/***
 * 3. Getters
 ***/

MaterialParameterName AbstractMaterialModificationCommand::getParameterName() const
{
    return parameterName_.getValue();
}


/***
 * 4. Buffer pre-reading
 ***/

MaterialParameterName AbstractMaterialModificationCommand::getParameterName( const void* buffer )
{
    MaterialCommand materialCommand( MaterialCommandType::MATERIAL_CREATION );

    return static_cast< MaterialParameterName >( (static_cast< const std::uint8_t* >( buffer ))[materialCommand.getPacketSize()] );
}

} // namespace como
