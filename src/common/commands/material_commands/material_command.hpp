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

#ifndef MATERIAL_COMMAND_HPP
#define MATERIAL_COMMAND_HPP

#include <common/commands/type_command.hpp>
#include <common/ids/material_id.hpp>

namespace como {

enum class MaterialCommandType : std::uint8_t {
    MATERIAL_CREATION = 0,
    MATERIAL_MODIFICATION
};
typedef PackableUint8< MaterialCommandType > PackableMaterialCommandType;

class MaterialCommand : public TypeCommand< MaterialCommandType >
{
    private:
        PackableMaterialID materialID_;

    /***
     * 1. Construction
     ***/
    public:
        MaterialCommand() = delete;
        MaterialCommand( MaterialCommandType commandType );
    protected:
        MaterialCommand( MaterialCommandType commandType, const MaterialID& materialID );
    public:
        MaterialCommand( const MaterialCommand& b );
        MaterialCommand( MaterialCommand&& ) = delete;

    /***
     * 2. Destruction
     ***/
        ~MaterialCommand() = default;

    /***
     * 3. Getters
     ***/
        MaterialID getMaterialID() const;


    /***
     * 4. Operators
     ***/
        MaterialCommand& operator = ( const MaterialCommand& ) = delete;
        MaterialCommand& operator = ( MaterialCommand&& ) = delete;
};

typedef std::shared_ptr< MaterialCommand > MaterialCommandPtr;
typedef std::shared_ptr< const MaterialCommand > MaterialCommandConstPtr;

} // namespace como

#endif // MATERIAL_COMMAND_HPP
