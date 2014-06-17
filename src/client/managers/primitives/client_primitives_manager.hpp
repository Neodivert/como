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

#ifndef CLIENT_PRIMITIVES_MANAGER_HPP
#define CLIENT_PRIMITIVES_MANAGER_HPP

#include <string>
#include <common/utilities/log.hpp>
#include <common/commands/primitive_category_commands/primitive_category_commands.hpp>
#include <common/commands/primitive_commands/primitive_commands.hpp>
#include <map>
#include <common/managers/primitives/abstract_primitives_manager.hpp>

namespace como {

class ClientPrimitivesManager : public AbstractPrimitivesManager
{
    private:

    public:
        /***
         * 1. Construction
         ***/
        ClientPrimitivesManager() = delete;
        ClientPrimitivesManager( std::string sceneName, LogPtr log );
        ClientPrimitivesManager( const ClientPrimitivesManager& ) = delete;
        ClientPrimitivesManager( ClientPrimitivesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ClientPrimitivesManager() = default;


        /***
         * 3. Remote command execution
         ***/
        void executeRemoteCommand( PrimitiveCategoryCommandConstPtr command );


        /***
         * 4. Operators
         ***/
        ClientPrimitivesManager& operator = ( const ClientPrimitivesManager& ) = delete;
        ClientPrimitivesManager& operator = ( ClientPrimitivesManager&& ) = delete;
};

} // namespace como

#endif // CLIENT_PRIMITIVES_MANAGER_HPP
