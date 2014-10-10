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

#ifndef RESOURCES_SYNCHRONIZATION_LIBRARY_HPP
#define RESOURCES_SYNCHRONIZATION_LIBRARY_HPP

#include <map>
#include <server/sync_data/resource_sync_data.hpp>
#include <server/commands_historic.hpp>

namespace como {

class ResourcesSynchronizationLibrary
{
    public:
        /***
         * 1. Construction
         ***/
        ResourcesSynchronizationLibrary( CommandsHistoricPtr commandsHistoric,
                                         const std::string& unpackingDirPath,
                                         const std::string &sceneFilePath = "" );
        ResourcesSynchronizationLibrary() = delete;
        ResourcesSynchronizationLibrary( const ResourcesSynchronizationLibrary& ) = delete;
        ResourcesSynchronizationLibrary( ResourcesSynchronizationLibrary&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesSynchronizationLibrary() = default;


        /***
         * 3. Command execution
         ***/
        void processCommand( const Command& command );


        /***
         * 4. File management
         ***/
        void saveToFile( const std::string& filePath ) const;
        void readFromFile( const std::string& filePath );


        /***
         * 4. Operators
         ***/
        ResourcesSynchronizationLibrary& operator = ( const ResourcesSynchronizationLibrary& ) = delete;
        ResourcesSynchronizationLibrary& operator = ( ResourcesSynchronizationLibrary&& ) = delete;


    private:
        std::map< ResourceID, ResourceSyncDataPtr > resourcesSyncData_;

        CommandsHistoricPtr commandsHistoric_;
        const std::string unpackingDirPath_;
};

typedef std::unique_ptr< ResourcesSynchronizationLibrary > ResourcesSynchronizationLibraryPtr;

} // namespace como

#endif // RESOURCES_SYNCHRONIZATION_LIBRARY_HPP
