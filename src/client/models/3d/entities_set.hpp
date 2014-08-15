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

#include <client/managers/selections/resources/resources_selection.hpp>
#include <client/models/3d/entity.hpp>
#include <common/commands/user_commands/user_parameter_change_command.hpp> // PivotPointMode type


namespace como {

template <class EntitySubtype>
class EntitiesSet : public Drawable, Transformable, ResourcesSelection< EntitySubtype >
{
    public:
        /***
         * 1. Construction
         ***/
        EntitiesSet( PivotPointMode mode = PivotPointMode::WORLD_ORIGIN );
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


        /***
         * 4. Setters
         ***/
        void setPivotPointMode( PivotPointMode& mode );


        /***
         * 5. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotate( const GLfloat &angle, const glm::vec3& axis, const glm::vec3& pivot );
        virtual void scale( const glm::vec3& scaleFactors, const glm::vec3& pivot );
        virtual void applyTransformationMatrix( const glm::mat4& transformation );



        /***
         * 6. Drawing
         ***/
        virtual void draw( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const GLfloat *contourColor ) const;


        /***
         * 7. Operators
         ***/
        EntitiesSet& operator = ( const EntitiesSet& ) = default;
        EntitiesSet& operator = ( EntitiesSet&& ) = default;


    private:
        PivotPointMode pivotPointMode_;
};


/***
 * 1. Construction
 ***/

template <class EntitySubtype>
EntitiesSet<EntitySubType>::EntitiesSet( PivotPointMode mode ) :
    pivotPointMode_( mode )
{}


/***
 * 3. Getters
 ***/

template <class EntitySubtype>
glm::vec3 EntitiesSet::centroid() const
{
    glm::vec3 centroid( 0.0f );

    for( auto entity : resources_ ){
        centroid += entity->centroid();
    }

    if( size() ){
        centroid /= size();
    }

    return centroid;
}


template <class EntitySubtype>
PivotPointMode EntitiesSet::pivotPointMode() const
{
    return pivotPointMode_;
}


/***
 * 4. Setters
 ***/

void EntitiesSet::setPivotPointMode( PivotPointMode& mode)
{
    pivotPointMode_ = mode;
}


/***
 * 5. Transformations
 ***/


void EntitiesSet::translate( glm::vec3 direction )
{
    for( auto& entityPair : resources_ ){
        entityPair.second->translate( direction );
    }
}


void EntitiesSet::rotate( GLfloat angle, glm::vec3 axis )
{
    //mutex_.lock();

    // Rotate every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( auto& entityPair : resources_ ){
                entityPair.second->rotateAroundCentroid( angle, axis );
                notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( auto& entityPair : resources_ ){
                entityPair.second->rotateAroundPivot( angle, axis, centroid() );
                notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( auto& entityPair : resources_ ){
                entityPair.second->rotateAroundOrigin( angle, axis );
                notifyElementUpdate( entityPair.first );
            }
        break;
    }

    updateSelectionCentroid();


    //mutex_.unlock();
}


void EntitiesSet::scale( glm::vec3 scaleFactors )
{
    // mutex_.lock();

    // Scale every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( auto& entityPair : resources_ ){
                entityPair.second->scaleAroundCentroid( scaleFactors );
                notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( auto& entityPair : resources_ ){
                entityPair.second->scaleAroundPivot( scaleFactors, centroid() );
                notifyElementUpdate( entityPair.first );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( auto& entityPair : resources_ ){
                entityPair.second->scaleAroundOrigin( scaleFactors );
                notifyElementUpdate( entityPair.first );
            }
        break;
    }

    updateSelectionCentroid();

    //mutex_.unlock();
}


void EntitiesSet::applyTransformationMatrix( const glm::mat4& transformation )
{
    for( auto& entityPair : resources_ ){
        entityPair.second->applyTransformationMatrix( transformation );
    }
}


} // namespace como

#endif // ENTITIES_SET_HPP
