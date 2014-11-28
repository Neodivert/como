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
#include <common/ids/resource_ids_generator.hpp>


namespace como {

class ServerPrimitivesManager : public AbstractPrimitivesManager
{
    public:
        /***
         * 1. Construction
         ***/
        ServerPrimitivesManager() = delete;
        ServerPrimitivesManager( const std::string& sceneDirPath,
                                 const std::string& tempDirPath,
                                 CommandsHistoricPtr commandsHistoric,
                                 LogPtr log,
                                 ResourceIDsGeneratorPtr resourceIDsGenerator );
        ServerPrimitivesManager( const ServerPrimitivesManager& ) = delete;
        ServerPrimitivesManager( ServerPrimitivesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ServerPrimitivesManager() = default;


        /***
         * 3. Getters
         ***/
        std::list< PlainMaterialData > primitivePlainMaterialsData( const ResourceID& primitiveID );


        /***
         * 4. Primitives management
         ***/
        void registerPrimitive( PrimitiveInfo primitive );
        void registerPrimitive( PrimitiveInfo primitive, const ResourceID& primitiveID );


        /***
         * 5. Operators
         ***/
        ServerPrimitivesManager& operator = ( const ServerPrimitivesManager& ) = delete;
        ServerPrimitivesManager& operator = ( ServerPrimitivesManager&& ) = delete;


    private:
        /***
         * 6. Initialization
         ***/
        void createPrimitivesDir();
        void syncPrimitivesDir();
        ResourceID createCategory( std::string name );
        void syncPrimitivesCategoryDir( std::string dirPath );


        /***
         * 7. Categories management
         ***/
        ResourceID registerCategory( std::string categoryName );


    private:
        CommandsHistoricPtr commandsHistoric_;
        ResourceIDsGeneratorPtr resourceIDsGenerator_;
};

} // namespace como

#endif // SERVER_PRIMITIVES_MANAGER_HPP
