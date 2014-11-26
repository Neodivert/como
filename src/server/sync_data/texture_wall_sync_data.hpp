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

#ifndef TEXTURE_WALL_SYNC_DATA_HPP
#define TEXTURE_WALL_SYNC_DATA_HPP

#include "resource_sync_data.hpp"
#include <common/commands/texture_wall_commands/texture_wall_commands.hpp>
#include <common/3d/texture_wall.hpp>

namespace como {

class TextureWallSyncData : public ResourceSyncData
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallSyncData( const Command* creationCommand, const ResourceID& id );
        TextureWallSyncData() = delete;
        TextureWallSyncData( const TextureWallSyncData& ) = default;
        TextureWallSyncData( TextureWallSyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~TextureWallSyncData() = default;


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
        TextureWallSyncData& operator = ( const TextureWallSyncData& ) = default;
        TextureWallSyncData& operator = ( TextureWallSyncData&& ) = default;


    private:
        TextureWall textureWall_;
};

} // namespace como

#endif // TEXTURE_WALL_SYNC_DATA_HPP
