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
#include <common/utilities/lockable.hpp>
#include <common/managers/abstract_resources_ownership_manager.hpp>
#include <set>
#include <server/public_user.hpp>

namespace como {

class ResourcesSynchronizationLibrary : public AbstractResourcesOwnershipManager
{
    public:
        /***
         * 1. Construction
         ***/
        ResourcesSynchronizationLibrary( CommandsHistoricPtr commandsHistoric,
                                         UsersMap& users,
                                         const std::string& unpackingDirPath,
                                         LogPtr log );
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
        void saveToFile( std::ofstream& file ) const;
        void readFromFile( std::ifstream& file );


        /***
         * 4. Operators
         ***/
        ResourcesSynchronizationLibrary& operator = ( const ResourcesSynchronizationLibrary& ) = delete;
        ResourcesSynchronizationLibrary& operator = ( ResourcesSynchronizationLibrary&& ) = delete;


        /***
         * 5. Auxiliar methods
         ***/
    protected:
        void saveCommandToFile();


        /***
         * 6. Owners management
         ***/
        void removeUser( UserID userID );


        /***
         * 7. Resources ownership management
         ***/
        virtual void lockResource( const ResourceID &resourceID, UserID userID );
        virtual void unlockResourcesSelection( UserID userID );
        virtual void deleteResourcesSelection( UserID userID );
        virtual void processLockResponse( const ResourceID &resourceID, bool lockResponse );

    private:
        std::map< ResourceID, ResourceSyncDataPtr > resourcesSyncData_;

        CommandsHistoricPtr commandsHistoric_;
        const std::string unpackingDirPath_;

        UsersMap& users_;
        std::set< ResourceID > undeletableResources_;
};

typedef std::unique_ptr< ResourcesSynchronizationLibrary > ResourcesSynchronizationLibraryPtr;

} // namespace como

#endif // RESOURCES_SYNCHRONIZATION_LIBRARY_HPP
