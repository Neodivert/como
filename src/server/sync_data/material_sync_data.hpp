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

#ifndef MATERIAL_SYNC_DATA_HPP
#define MATERIAL_SYNC_DATA_HPP

#include "resource_sync_data.hpp"
#include <common/3d/plain_material_data.hpp>

namespace como {

class MaterialSyncData : public ResourceSyncData
{
    public:
        /***
         * 1. Construction
         ***/
        MaterialSyncData( const Command* creationCommand, const ResourceID& id );
        MaterialSyncData() = delete;
        MaterialSyncData( const MaterialSyncData& ) = default;
        MaterialSyncData( MaterialSyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~MaterialSyncData() = default;


        /***
         * 3. Getters
         ***/
        virtual std::list<CommandConstPtr> generateUpdateCommands() const;


        /***
         * 4. Updating
         ***/
        virtual void processCommand( const Command &command );


        /***
         * 5. Operators
         ***/
        MaterialSyncData& operator = ( const MaterialSyncData& ) = default;
        MaterialSyncData& operator = ( MaterialSyncData&& ) = default;


    private:
        PlainMaterialData material_;
};

} // namespace como

#endif // MATERIAL_SYNC_DATA_HPP
