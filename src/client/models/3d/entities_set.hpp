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

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace como {

const glm::vec4 DEFAULT_BORDER_COLOR = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );

template <class EntitySubtype>
class EntitiesSet : public AbstractEntitiesSet, public ResourcesSelection< EntitySubtype >
{
    public:
        /***
         * 1. Construction
         ***/
        EntitiesSet( glm::vec4 borderColor = DEFAULT_BORDER_COLOR );
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
        glm::vec4 borderColor() const;
        virtual unsigned int size() const;
        virtual bool containsEntity(const ResourceID &entityID) const;


        /***
         * 4. Setters
         ***/
        void setBorderColor( const glm::vec4& borderColor );
        virtual void setEntityModelMatrix(const ResourceID &entityID, const glm::mat4 &modelMatrix);
        /*
         * size() == 0 -> "(Nothing selected)"
         * size() == 1 -> Name of the unique resource in the selection.
         * size() > 1 -> (Multiple entities)".
         */
        virtual std::string name() const;
        virtual std::string typeName() const;


        /***
         * 5. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotateAroundPivot(GLfloat angle, glm::vec3 axis, glm::vec3 pivot);
        virtual void rotateAroundIndividualCentroids(GLfloat angle, glm::vec3 axis);
        virtual void scaleAroundPivot(glm::vec3 scaleFactors, glm::vec3 pivot);
        virtual void scaleAroundIndividualCentroids(glm::vec3 scaleFactors);
        virtual void applyTransformationMatrix( const glm::mat4 &transformationMatrix );
        virtual void setModelMatrix(const glm::mat4 &modelMatrix);


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
        glm::vec4 borderColor_;
};


/***
 * 1. Construction
 ***/

template <class EntitySubtype>
EntitiesSet<EntitySubtype>::EntitiesSet( glm::vec4 borderColor ) :
    borderColor_( borderColor )
{}


/***
 * 3. Getters
 ***/

template <class EntitySubtype>
glm::vec3 EntitiesSet<EntitySubtype>::centroid() const
{
    LOCK

    glm::vec3 centroid( 0.0f );

    for( const auto& entityPair : this->resources_ ){
        centroid += entityPair.second->centroid();
    }

    if( this->size() ){
        centroid /= this->size();
    }

    return centroid;
}


template <class EntitySubtype>
glm::vec4 EntitiesSet<EntitySubtype>::borderColor() const
{
    LOCK

    return borderColor_;
}


template <class EntitySubtype>
unsigned int EntitiesSet<EntitySubtype>::size() const
{
    LOCK

    return this->ResourcesSelection<EntitySubtype>::size();
}


template <class EntitySubtype>
bool EntitiesSet<EntitySubtype>::containsEntity(const ResourceID &entityID) const
{
    LOCK

    return this->containsResource( entityID );
}


/***
 * 4. Setters
 ***/

template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::setBorderColor( const glm::vec4& borderColor )
{
    LOCK

    this->borderColor_ = borderColor;
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::setEntityModelMatrix(const ResourceID &entityID, const glm::mat4 &modelMatrix)
{
    LOCK

    this->resources_.at( entityID )->setModelMatrix( modelMatrix );
}


template <class EntitySubtype>
std::string EntitiesSet<EntitySubtype>::name() const
{
    LOCK

    if( this->size() == 0 ){
        return "(Nothing selected)";
    }else if( this->size() == 1 ){
        return this->resources_.begin()->second->name();
    }else{
        return "(Multiple entities selected)";
    }
}


template <class EntitySubtype>
std::string EntitiesSet<EntitySubtype>::typeName() const
{
    LOCK

    if( this->size() == 1 ){
        return this->resources_.begin()->second->typeName();
    }else{
        return "Selection";
    }
}


/***
 * 5. Transformations
 ***/

template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::translate( glm::vec3 direction )
{
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->translate( direction );
    }
    this->notifyObservers();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::rotateAroundPivot( GLfloat angle, glm::vec3 axis, glm::vec3 pivot )
{
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->rotateAroundPivot( angle, axis, pivot );
    }
    this->notifyObservers();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::rotateAroundIndividualCentroids( GLfloat angle, glm::vec3 axis)
{
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->rotateAroundCentroid( angle, axis );
    }
    this->notifyObservers();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::scaleAroundPivot( glm::vec3 scaleFactors, glm::vec3 pivot)
{
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->scaleAroundPivot( scaleFactors, pivot );
    }
    this->notifyObservers();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::scaleAroundIndividualCentroids( glm::vec3 scaleFactors)
{
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->scaleAroundCentroid( scaleFactors );
    }
    this->notifyObservers();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::applyTransformationMatrix(const glm::mat4 &transformationMatrix)
{
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->applyTransformationMatrix( transformationMatrix );
    }
    this->notifyObservers();
}


template <class EntitySubtype>
void EntitiesSet<EntitySubtype>::setModelMatrix( const glm::mat4 &modelMatrix )
{
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->setModelMatrix( modelMatrix );
    }
    this->notifyObservers();
}


/***
 * 6. Intersections
 ***/

template <class EntitySubtype>
bool EntitiesSet<EntitySubtype>::intersectsRay( glm::vec3 r0, glm::vec3 r1, ResourceID& closestEntity, float& minT ) const
{
    LOCK

    float t;
    bool entityIntersected = false;

    // Check if the given ray intersects any drawable in the selection.
    for( const auto& entityPair : this->resources_ ){
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
    LOCK

    for( auto& entityPair : this->resources_ ){
        entityPair.second->draw( openGL, viewMatrix, projectionMatrix, &borderColor_ );
    }
}


} // namespace como

#endif // ENTITIES_SET_HPP
