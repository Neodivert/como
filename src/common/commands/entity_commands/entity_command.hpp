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

#ifndef ENTITY_COMMAND_HPP
#define ENTITY_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class EntityCommandType {
    MODEL_MATRIX_REPLACEMENT
};

class EntityCommand : public TypeCommand< EntityCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        EntityCommand( EntityCommandType commandType, const ResourceID& entityID, UserID userID );
        EntityCommand() = delete;
        EntityCommand( const EntityCommand& );
        EntityCommand( EntityCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~EntityCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID entityID() const;


        /***
         * 4. Operators
         ***/
        EntityCommand& operator = ( const EntityCommand& ) = delete;
        EntityCommand& operator = ( EntityCommand&& ) = delete;


    private:
        PackableResourceID entityID_;
};

} // namespace como

#endif // ENTITY_COMMAND_HPP
