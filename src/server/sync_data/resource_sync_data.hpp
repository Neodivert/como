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

#ifndef RESOURCE_SYNC_DATA_HPP
#define RESOURCE_SYNC_DATA_HPP

#include "sync_data.hpp"

namespace como {

class ResourceSyncData : public SyncData
{
    public:
        /***
         * 1. Construction
         ***/
        ResourceSyncData( const Command* creationCommand );
        ResourceSyncData() = delete;
        ResourceSyncData( const ResourceSyncData& ) = default;
        ResourceSyncData( ResourceSyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~ResourceSyncData() = default;


        /***
         * 3. Getters
         ***/
        virtual std::list< CommandConstPtr > generateUpdateCommands() const;


        /***
         * 4. Updating
         ***/
        virtual void processCommand( const Command& command );


        /***
         * 5. Operators
         ***/
        ResourceSyncData& operator = ( const ResourceSyncData& ) = default;
        ResourceSyncData& operator = ( ResourceSyncData&& ) = default;


    private:
        //const ResourceID resourceID_;
        std::string resourceName_;
};

typedef std::unique_ptr< ResourceSyncData > ResourceSyncDataPtr;

} // namespace como

#endif // RESOURCE_SYNC_DATA_HPP
