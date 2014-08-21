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

#ifndef RESOURCE_COMMANDS_EXECUTER_HPP
#define RESOURCE_COMMANDS_EXECUTER_HPP

#include <client/managers/resources/resources_ownership_requester.hpp>

namespace como {

class ResourceCommandsExecuter : public ResourcesOwnershipRequester
{
    public:
        /***
         * 1. Construction
         ***/
        ResourceCommandsExecuter();
        ResourceCommandsExecuter( const ResourceCommandsExecuter& ) = delete;
        ResourceCommandsExecuter( ResourceCommandsExecuter&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourceCommandsExecuter() = default;


        /***
         * 3. Command execution
         ***/
        void executeResourceCommand( const ResourceCommand& command );
        void executeResourcesSelectionCommand( const ResourcesSelectionCommand& command );


        /***
         * 4. Operators
         ***/
        ResourceCommandsExecuter& operator = ( const ResourceCommandsExecuter& ) = delete;
        ResourceCommandsExecuter& operator = ( ResourceCommandsExecuter&& ) = delete;
};

} // namespace como

#endif // RESOURCE_COMMANDS_EXECUTER_HPP
