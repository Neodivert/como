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

#ifndef ENTITY_SYNC_DATA_HPP
#define ENTITY_SYNC_DATA_HPP

#include "resource_sync_data.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <common/transformable/transformable.hpp>

namespace como {

class EntitySyncData : public ResourceSyncData, public Transformable
{
    public:
        /***
         * 1. Construction
         ***/
        EntitySyncData( const Command* creationCommand, const ResourceID& id, const glm::vec3& centroid );
        EntitySyncData() = delete;
        EntitySyncData( const EntitySyncData& ) = default;
        EntitySyncData( EntitySyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~EntitySyncData() = default;


        /***
         * 3. Getters
         ***/
        virtual std::list<CommandConstPtr> generateUpdateCommands() const;


        /***
         * 4. Updating
         ***/
        virtual void processCommand( const Command &command );


        /***
         * 5. Transformations
         ***/
        virtual glm::vec3 centroid() const;
        virtual void applyTransformationMatrix(const glm::mat4 &transformation);
        virtual void setModelMatrix( const glm::mat4 &modelMatrix );


        /***
         * 6. Operators
         ***/
        EntitySyncData& operator = ( const EntitySyncData& ) = default;
        EntitySyncData& operator = ( EntitySyncData&& ) = default;


    private:
        const glm::vec4 originalCentroid_;
        glm::vec3 centroid_;
        glm::mat4 modelMatrix_;
};

} // namespace como

#endif // ENTITY_SYNC_DATA_HPP
