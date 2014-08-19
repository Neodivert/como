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

#ifndef ENTITIES_SET_HPP
#define ENTITIES_SET_HPP

#include <client/models/3d/abstract_entities_set.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace como {

const glm::vec4 DEFAULT_BORDER_COLOR = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );

template <class EntitySubtype>
class EntitiesSet : public AbstractEntitiesSet, public virtual ResourcesSelection< EntitySubtype >
{
    public:
        /***
         * 1. Construction
         ***/
        EntitiesSet( glm::vec4 borderColor = DEFAULT_BORDER_COLOR, PivotPointMode mode = PivotPointMode::WORLD_ORIGIN );
        EntitiesSet( const EntitiesSet& ) = default;
        EntitiesSet( EntitiesSet&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~EntitiesSet() = default;


        /***
         * 3. Getters
         ***/
        virtual glm::vec3 centroid() const;
        PivotPointMode pivotPointMode() const;
        glm::vec4 borderColor() const;
        virtual unsigned int size() const;


        /***
         * 4. Setters
         ***/
        void setPivotPointMode( PivotPointMode mode );
        void setBorderColor( const glm::vec4& borderColor );


        /***
         * 5. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotate( const GLfloat& angle, const glm::vec3& axis );
        virtual void scale( const glm::vec3& scaleFactors );
        virtual void applyTransformationMatrix( const glm::mat4& transformation );


        /***
         * 6. Intersections
         ***/
        bool intersectsRay( glm::vec3 r0, glm::vec3 r1, ResourceID& closestEntity, float& minT ) const;


        /***
         * 7. Drawing
         ***/
        virtual void drawAll( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix ) const;


        /***
         * 7. Operators
         ***/
        EntitiesSet& operator = ( const EntitiesSet& ) = default;
        EntitiesSet& operator = ( EntitiesSet&& ) = default;


    private:
        PivotPointMode pivotPointMode_;

        glm::vec4 borderColor_;
};


/***
 * 1. Construction
 ***/

template <class EntitySubtype>
EntitiesSet<EntitySubtype>::EntitiesSet( glm::vec4 borderColor, PivotPointMode mode ) :
    pivotPointMode_( mode ),
    borderColor_( borderColor )
{}


/***
 * 3. Getters
 ***/

template <class EntitySubtype>
glm::vec3 EntitiesSet<EntitySubtype>::centroid() const
{
    glm::vec3 centroid( 0.0f );

    for( auto entityPair : this->resources_ ){
        centroid += entityPair.second->centroid();
    }

    if( this->size() ){
        centroid /= this->size();
    }

    return centroid;
}


template <class EntitySubtype>
PivotPointMode EntitiesSet<EntitySubtype>::pivotPointMode() const
{
    return pivotPointMode_;
}


template <class EntitySubtype>
glm::vec4 EntitiesSet<EntitySubtype>::borderColor() const
{
    return borderColor_;
}


template <class EntitySubtype>
unsigned int EntitiesSet<EntitySubtype>::size() const
{
    return this->ResourcesSelection<EntitySubtype>::size();
}


/***
 * 4. Setters
 ***/

template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::setPivotPointMode( PivotPointMode mode)
{
    pivotPointMode_ = mode;
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::setBorderColor( const glm::vec4& borderColor )
{
    this->borderColor_ = borderColor;
}


/***
 * 5. Transformations
 ***/

template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::translate( glm::vec3 direction )
{
    for( auto& entityPair : this->resources_ ){
        entityPair.second->translate( direction );
    }
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::rotate( const GLfloat& angle, const glm::vec3& axis )
{
    //mutex_.lock();

    // Rotate every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( auto& entityPair : this->resources_ ){
                entityPair.second->rotateAroundCentroid( angle, axis );
                this->notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( auto& entityPair : this->resources_ ){
                entityPair.second->rotateAroundPivot( angle, axis, centroid() );
                this->notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( auto& entityPair : this->resources_ ){
                entityPair.second->rotateAroundOrigin( angle, axis );
                this->notifyElementUpdate( entityPair.first );
            }
        break;
    }

    //updateSelectionCentroid();


    //mutex_.unlock();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::scale( const glm::vec3& scaleFactors )
{
    // mutex_.lock();

    // Scale every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( auto& entityPair : this->resources_ ){
                entityPair.second->scaleAroundCentroid( scaleFactors );
                this->notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( auto& entityPair : this->resources_ ){
                entityPair.second->scaleAroundPivot( scaleFactors, centroid() );
                this->notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( auto& entityPair : this->resources_ ){
                entityPair.second->scaleAroundOrigin( scaleFactors );
                this->notifyElementUpdate( entityPair.first );
            }
        break;
    }

    //updateSelectionCentroid();

    //mutex_.unlock();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::applyTransformationMatrix( const glm::mat4& transformation )
{
    for( auto& entityPair : this->resources_ ){
        entityPair.second->applyTransformationMatrix( transformation );
    }
}


/***
 * 6. Intersections
 ***/

template <class EntitySubtype>
bool EntitiesSet<EntitySubtype>::intersectsRay( glm::vec3 r0, glm::vec3 r1, ResourceID& closestEntity, float& minT ) const
{
    float t;
    bool entityIntersected = false;

    // Check if the given ray intersects any drawable in the selection.
    for( auto entityPair : this->resources_ ){
        entityPair.second->intersects( r0, r1, t );

        // New closest object, get its ID and distance.
        if( ( t >= 0.0f ) && ( t < minT ) ){
            entityIntersected = true;

            closestEntity = entityPair.first;
            minT = t;
        }
    }

    return entityIntersected;
}


/***
 * 7. Drawing
 ***/

template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) const
{
    for( auto& entityPair : this->resources_ ){
        entityPair.second->draw( openGL, viewMatrix, projectionMatrix, &borderColor_ );
    }
}


} // namespace como

#endif // ENTITIES_SET_HPP
