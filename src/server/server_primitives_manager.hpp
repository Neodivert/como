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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SERVER_PRIMITIVES_MANAGER_HPP
#define SERVER_PRIMITIVES_MANAGER_HPP

#include <common/managers/primitives/abstract_primitives_manager.hpp>
#include <common/ids/resource_id.hpp>
#include <string>
#include <server/commands_historic.hpp>
#include <common/utilities/paths.hpp>


namespace como {

class ServerPrimitivesManager : public AbstractPrimitivesManager
{
    private:
        ResourceID nextPrimitiveCategoryID_;
        ResourceID nextPrimitiveID_;

        CommandsHistoricPtr commandsHistoric_;

    public:
        /***
         * 1. Construction
         ***/
        ServerPrimitivesManager() = delete;
        ServerPrimitivesManager( std::string sceneDirPath, CommandsHistoricPtr commandsHistoric, LogPtr log );
        ServerPrimitivesManager( const ServerPrimitivesManager& ) = delete;
        ServerPrimitivesManager( ServerPrimitivesManager&& ) = delete;


        /***
         * 2. Initialization
         ***/
    private:
        void createPrimitivesDir();
        void syncPrimitivesDir();
        void syncPrimitivesCategoryDir( std::string dirPath );
    public:


        /***
         * 3. Destruction
         ***/
        ~ServerPrimitivesManager() = default;


        /***
         * 4. Categories management
         ***/
    private:
        ResourceID registerCategory( std::string categoryName );


        /***
         * 5. Primitives management
         ***/
        void registerPrimitive( std::string filePath, ResourceID categoryID );


        /***
         * 5. Auxiliar methods
         ***/
    private:
        bool categoryNameInUse( std::string categoryName );


        /***
         * 6. Operators
         ***/
        ServerPrimitivesManager& operator = ( const ServerPrimitivesManager& ) = delete;
        ServerPrimitivesManager& operator = ( ServerPrimitivesManager&& ) = delete;
};

} // namespace como

#endif // SERVER_PRIMITIVES_MANAGER_HPP
