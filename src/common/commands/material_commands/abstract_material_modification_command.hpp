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

#ifndef ABSTRACT_MATERIAL_CHANGE_COMMAND_HPP
#define ABSTRACT_MATERIAL_CHANGE_COMMAND_HPP

#include "material_command.hpp"

namespace como {

enum class MaterialParameterName : std::uint8_t {
    COLOR = 0,
    AMBIENT_REFLECTIVITY,
    DIFFUSE_REFLECTIVITY,
    SPECULAR_REFLECTIVITY,
    SPECULAR_EXPONENT
};
typedef PackableUint8< MaterialParameterName > PackableMaterialParameterName;


class AbstractMaterialModificationCommand : public MaterialCommand
{
    private:
        const PackableMaterialParameterName parameterName_;

    public:
        /***
         * 1. Construction
         ***/
        AbstractMaterialModificationCommand( UserID userID, PackableMaterialParameterName parameterName );
        AbstractMaterialModificationCommand( UserID userID, ResourceID materialID, PackableMaterialParameterName parameterName );
        AbstractMaterialModificationCommand( const AbstractMaterialModificationCommand& );
        AbstractMaterialModificationCommand( AbstractMaterialModificationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~AbstractMaterialModificationCommand() = default;


        /***
         * 3. Getters
         ***/
        virtual MaterialParameterName getParameterName() const;


        /***
         * 4. Buffer pre-reading
         ***/
        static MaterialParameterName getParameterName( const void* buffer );


        /***
         * 5. Operators
         ***/
        AbstractMaterialModificationCommand& operator = ( const AbstractMaterialModificationCommand& ) = default;
        AbstractMaterialModificationCommand& operator = ( AbstractMaterialModificationCommand&& ) = delete;
};

} // namespace como

#endif // ABSTRACT_MATERIAL_CHANGE_COMMAND_HPP
