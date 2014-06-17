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

#include <map>
#include <common/utilities/paths.hpp>
#include <common/ids/primitive_category_id.hpp>
#include <string>
#include <server/commands_historic.hpp>

namespace como {

class ServerPrimitivesManager
{
    private:
        std::string scenePrimitivesDir_;

        std::map< PrimitiveCategoryID, std::string > categoryNames_;

        PrimitiveCategoryID nextPrimitiveCategoryID_;
        PrimitiveID nextPrimitiveID_;

        CommandsHistoricPtr commandsHistoric_;

        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        ServerPrimitivesManager() = delete;
        ServerPrimitivesManager( std::string sceneName_, CommandsHistoricPtr commandsHistoric, LogPtr log );
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
        void addCategory( std::string categoryName );


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
