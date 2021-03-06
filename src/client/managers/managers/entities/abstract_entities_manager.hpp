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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef ABSTRACT_ENTITIES_MANAGER_HPP
#define ABSTRACT_ENTITIES_MANAGER_HPP

#include <client/managers/managers/resources/resource_commands_executer.hpp>
#include <client/models/3d/sets/drawables_set.hpp>
#include <client/models/3d/sets/pickables_set.hpp>

namespace como {

/*
 * This is an auxiliar base class created for allowing vectors of managers
 * implementing the multiple interfaces this class inherits from.
 ***/
class AbstractEntitiesManager : public virtual ResourceCommandsExecuter, public PickablesSet
{
    public:
        /***
         * 1. Construction
         ***/
        AbstractEntitiesManager( ServerInterfacePtr server );
        AbstractEntitiesManager() = delete;
        AbstractEntitiesManager( AbstractEntitiesManager& ) = default;
        AbstractEntitiesManager( AbstractEntitiesManager&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~AbstractEntitiesManager() = default;


        /***
         * 3. Getters
         ***/
        // TODO: Duplicated method in another classes without a common
        // interface.
        virtual bool containsResource( const ResourceID& resourceID ) const = 0;


        /***
         * 4. Selecting
         ***/
        virtual ResourceID selectEntityByRayPicking( glm::vec3 rayOrigin, glm::vec3 rayDirection, bool addToSelection, float& t, const float& MAX_T = FLT_MAX );


        /***
         * 5. Drawing
         ***/
        virtual void drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const = 0;


        /***
         * 6. Operators
         ***/
        AbstractEntitiesManager& operator = ( const AbstractEntitiesManager& ) = default;
        AbstractEntitiesManager& operator = ( AbstractEntitiesManager&& ) = default;
};

} // namespace como

#endif // ABSTRACT_ENTITIES_MANAGER_HPP
