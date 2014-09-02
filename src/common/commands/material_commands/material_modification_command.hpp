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

#ifndef MATERIAL_MODIFICATION_COMMAND_HPP
#define MATERIAL_MODIFICATION_COMMAND_HPP

#include "abstract_material_modification_command.hpp"

namespace como {

template < MaterialParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
class MaterialModificationCommand : public AbstractMaterialModificationCommand
{
    private:
        ParameterPackableType parameterValue_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialModificationCommand();
        MaterialModificationCommand( UserID userID, ResourceID materialID, ParameterPackableType parameterValue );
        MaterialModificationCommand( UserID userID, ResourceID materialID, ParameterPlainType parameterValue );
        MaterialModificationCommand( const MaterialModificationCommand& b );
        MaterialModificationCommand( MaterialModificationCommand& ) = delete;
        virtual Command* clone() const;


        /***
         * 2. Destruction
         ***/
        virtual ~MaterialModificationCommand() = default;


        /***
         * 3. Getters
         ***/
        ParameterPlainType getParameterValue() const;


        /***
         * 4. Operators
         ***/
        MaterialModificationCommand& operator = ( const MaterialModificationCommand& ) = delete;
        MaterialModificationCommand& operator = ( MaterialModificationCommand&& ) = delete;
};

typedef MaterialModificationCommand< MaterialParameterName::COLOR, PackableColor, std::array< std::uint8_t, 4 > > MaterialColorChangeCommand;
typedef MaterialModificationCommand< MaterialParameterName::AMBIENT_REFLECTIVITY, PackableColor, std::array< std::uint8_t, 4 > > MaterialAmbientReflectivityChangeCommand;
typedef MaterialModificationCommand< MaterialParameterName::DIFFUSE_REFLECTIVITY, PackableColor, std::array< std::uint8_t, 4 > > MaterialDiffuseReflectivityChangeCommand;
typedef MaterialModificationCommand< MaterialParameterName::SPECULAR_REFLECTIVITY, PackableColor, std::array< std::uint8_t, 4 > > MaterialSpecularReflectivityChangeCommand;
typedef MaterialModificationCommand< MaterialParameterName::SPECULAR_EXPONENT, PackableFloat, float > MaterialSpecularExponentChangeCommand;


/***
 * 1. Construction
 ***/

template < MaterialParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
MaterialModificationCommand<ParameterName, ParameterPackableType, ParameterPlainType>::MaterialModificationCommand() :
    AbstractMaterialModificationCommand( NO_USER, ParameterName )
{
    addPackable( &parameterValue_ );
}


template < MaterialParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
MaterialModificationCommand<ParameterName, ParameterPackableType, ParameterPlainType>::MaterialModificationCommand( UserID userID, ResourceID materialID, ParameterPackableType parameterValue ) :
    AbstractMaterialModificationCommand( userID, materialID, ParameterName ),
    parameterValue_( parameterValue )
{
    addPackable( &parameterValue_ );
}


template < MaterialParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
MaterialModificationCommand<ParameterName, ParameterPackableType, ParameterPlainType>::MaterialModificationCommand( UserID userID, ResourceID materialID, ParameterPlainType parameterValue ) :
    AbstractMaterialModificationCommand( userID, materialID, ParameterName ),
    parameterValue_( parameterValue )
{
    addPackable( &parameterValue_ );
}


template < MaterialParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
MaterialModificationCommand<ParameterName, ParameterPackableType, ParameterPlainType>::MaterialModificationCommand( const MaterialModificationCommand& b ) :
    AbstractMaterialModificationCommand( b ),
    parameterValue_( b.parameterValue_ )
{
    addPackable( &parameterValue_ );
}


template < MaterialParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
Command* MaterialModificationCommand<ParameterName, ParameterPackableType, ParameterPlainType>::clone() const
{
    return new MaterialModificationCommand<ParameterName, ParameterPackableType, ParameterPlainType>( *this );
}


/***
 * 3. Getters
 ***/

template < MaterialParameterName ParameterName, class ParameterPackableType, class ParameterPlainType >
ParameterPlainType MaterialModificationCommand<ParameterName, ParameterPackableType, ParameterPlainType>::getParameterValue() const
{
    return parameterValue_.getValue();
}

} // namespace como

#endif // MATERIAL_MODIFICATION_COMMAND_HPP
