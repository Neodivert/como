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

#ifndef LIGHT_SYNC_DATA_HPP
#define LIGHT_SYNC_DATA_HPP

#include "entity_sync_data.hpp"
#include <common/commands/light_commands/light_commands.hpp>

namespace como {

class LightSyncData : public EntitySyncData
{
    public:
        /***
         * 1. Construction
         ***/
        LightSyncData( const DirectionalLightCreationCommand& creationCommand );
        LightSyncData() = delete;
        LightSyncData( const LightSyncData& ) = default;
        LightSyncData( LightSyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~LightSyncData() = default;


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
        LightSyncData& operator = ( const LightSyncData& ) = default;
        LightSyncData& operator = ( LightSyncData&& ) = default;


    private:
        /***
         * Attributes
         ***/
        glm::vec3 color_;
        float ambientCoefficient_;
};

} // namespace como

#endif // LIGHT_SYNC_DATA_HPP
