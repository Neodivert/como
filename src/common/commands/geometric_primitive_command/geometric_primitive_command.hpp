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

#ifndef GEOMETRIC_PRIMITIVE_COMMAND_HPP
#define GEOMETRIC_PRIMITIVE_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class GeometricPrimitiveCommandType : std::uint8_t
{
    CUBE_CREATION = 0
};

class GeometricPrimitiveCommand : public TypeCommand< GeometricPrimitiveCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        GeometricPrimitiveCommand( GeometricPrimitiveCommandType type, const ResourceID& meshID, const ResourceID& materialID );
        GeometricPrimitiveCommand() = delete;
        GeometricPrimitiveCommand( const GeometricPrimitiveCommand& );
        GeometricPrimitiveCommand( GeometricPrimitiveCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~GeometricPrimitiveCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getMeshID() const;
        ResourceID getMaterialID() const;



        /***
         * 4. Operators
         ***/
        GeometricPrimitiveCommand& operator = ( const GeometricPrimitiveCommand& ) = delete;
        GeometricPrimitiveCommand& operator = ( GeometricPrimitiveCommand&& ) = delete;


    private:
        PackableResourceID meshID_;
        PackableResourceID materialID_;
};

} // namespace como

#endif // GEOMETRIC_PRIMITIVE_COMMAND_HPP
