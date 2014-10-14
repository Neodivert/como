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

#ifndef ABSTRACT_ENTITIES_SET_HPP
#define ABSTRACT_ENTITIES_SET_HPP

#include <client/managers/selections/resources/resources_selection.hpp>
#include <client/models/3d/entity.hpp>
#include <common/commands/user_commands/user_parameter_change_command.hpp> // PivotPointMode type

namespace como {

class AbstractEntitiesSet : public Transformable, public virtual Observable // TODO: Inherit from an abstraction: public virtual ResourcesSelection< EntitySubtype >
{
    public:
        /***
         * 1. Construction
         ***/
        AbstractEntitiesSet() = default;
        AbstractEntitiesSet( const AbstractEntitiesSet& ) = default;
        AbstractEntitiesSet( AbstractEntitiesSet&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~AbstractEntitiesSet() = default;


        /***
         * 3. Getters
         ***/
        virtual glm::vec3 centroid() const = 0;
        virtual PivotPointMode pivotPointMode() const = 0;
        virtual glm::vec4 borderColor() const = 0;
        virtual unsigned int size() const = 0;
        virtual bool containsEntity( const ResourceID& entityID ) const = 0;


        /***
         * 4. Setters
         ***/
        virtual void setPivotPointMode( PivotPointMode mode ) = 0;
        virtual void setBorderColor( const glm::vec4& borderColor ) = 0;
        virtual void setEntityModelMatrix( const ResourceID& entityID,
                                           const glm::mat4& modelMatrix ) = 0;


        /***
         * 5. Transformations
         ***/
        virtual void translate( glm::vec3 direction ) = 0;
        virtual void rotateAroundPivot( GLfloat angle, glm::vec3 axis, glm::vec3 pivot ) = 0;
        virtual void rotateAroundIndividualCentroids( GLfloat angle, glm::vec3 axis ) = 0;
        virtual void scaleAroundPivot( glm::vec3 scaleFactors, glm::vec3 pivot ) = 0;
        virtual void scaleAroundIndividualCentroids( glm::vec3 scaleFactors ) = 0;
        virtual void applyTransformationMatrix( const glm::mat4& transformation ) = 0;


        /***
         * 6. Intersections
         ***/
        virtual bool intersectsRay( glm::vec3 r0, glm::vec3 r1, ResourceID& closestEntity, float& minT ) const = 0;


        /***
         * 7. Drawing
         ***/
        virtual void drawAll( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix ) const = 0;


        /***
         * 8. Operators
         ***/
        AbstractEntitiesSet& operator = ( const AbstractEntitiesSet& ) = delete;
        AbstractEntitiesSet& operator = ( AbstractEntitiesSet&& ) = delete;
};

} // namespace como

#endif // ABSTRACT_ENTITIES_SET_HPP
