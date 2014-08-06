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

#ifndef RESOURCE_COMMAND_HPP
#define RESOURCE_COMMAND_HPP

#include <common/commands/type_command.hpp>
#include <common/packables/ids/packable_resource_id.hpp>

namespace como {

enum class ResourceCommandType : std::uint8_t
{
    RESOURCE_LOCK = 0,
    RESOURCE_SELECTION_RESPONSE
};

// TODO: Make constructors protected and inherit specialized commands.
class ResourceCommand : public TypeCommand< ResourceCommandType >
{
    private:
        PackableResourceID resourceID_;

    public:
        /***
         * 1. Construction
         ***/
        ResourceCommand() = delete;
        ResourceCommand( ResourceCommandType commandType );
        ResourceCommand( ResourceCommandType commandType, UserID userID, const ResourceID resourceID );
        ResourceCommand( const ResourceCommand& b );
        ResourceCommand( ResourceCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourceCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getResourceID() const;


        /***
         * 4. Operators
         ***/
        ResourceCommand& operator = ( const ResourceCommand& ) = delete;
        ResourceCommand& operator = ( ResourceCommand&& ) = delete;
};

typedef std::shared_ptr< const ResourceCommand > ResourceCommandConstPtr;

} // namespace como

#endif // RESOURCE_COMMAND_HPP
