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

#ifndef CAMERA_SYNC_DATA_HPP
#define CAMERA_SYNC_DATA_HPP

#include "entity_sync_data.hpp"
#include <common/commands/camera_commands/camera_commands.hpp>

namespace como {

class CameraSyncData : public EntitySyncData
{
    public:
        /***
         * 1. Construction
         ***/
        CameraSyncData( const CameraCreationCommand& cameraCreationCommand );
        CameraSyncData() = delete;
        CameraSyncData( const CameraSyncData& ) = default;
        CameraSyncData( CameraSyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~CameraSyncData() = default;


        /***
         * 3. Operators
         ***/
        CameraSyncData& operator = ( const CameraSyncData& ) = default;
        CameraSyncData& operator = ( CameraSyncData&& ) = default;
};

} // namespace como

#endif // CAMERA_SYNC_DATA_HPP
