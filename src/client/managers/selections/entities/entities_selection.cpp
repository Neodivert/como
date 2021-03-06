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

#include "entities_selection.hpp"

namespace como {


/***
 * 1. Construction
 ***/

EntitiesSelection::EntitiesSelection( LightsSelection* lightsSelection, MeshesSelection* meshesSelection, CamerasSelection* camerasSelection ) :
    lightsSelection_( lightsSelection ),
    meshesSelection_( meshesSelection ),
    camerasSelection_( camerasSelection ),
    centroid_( 0.0f )
{
    specializedEntitiesSelections_.push_back( lightsSelection_ );
    specializedEntitiesSelections_.push_back( meshesSelection_ );
    specializedEntitiesSelections_.push_back( camerasSelection_ );

    for( auto& selection : specializedEntitiesSelections_ ){
        selection->Observable::addObserver( this );
    }
}


/***
 * 3. Getters
 ***/

glm::vec3 EntitiesSelection::centroid() const
{
    LOCK
    return centroid_;
}


glm::vec4 EntitiesSelection::borderColor() const
{
    LOCK
    return lightsSelection_->borderColor();
}


unsigned int EntitiesSelection::size() const
{
    LOCK
    unsigned int size = 0;

    for( auto selection : specializedEntitiesSelections_ ){
        size += selection->size();
    }

    return size;
}


bool EntitiesSelection::containsEntity(const ResourceID &entityID) const
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        if( selection->containsEntity( entityID ) ){
            return true;
        }
    }
    return false;
}


std::string EntitiesSelection::name() const
{
    LOCK

    if( size() == 0 ){
        return "(Nothing selected)";
    }else if( size() == 1 ){
        std::string name;
        for( auto& selection : specializedEntitiesSelections_ ){
            if( selection->size() == 1 ){
                name = selection->name();
            }
        }
        return name;
    }else{
        return "(Multiple objects)";
    }
}


std::string EntitiesSelection::typeName() const
{
    LOCK

    if( size() == 1 ){
        std::string typeName;
        for( auto& selection : specializedEntitiesSelections_ ){
            if( selection->size() == 1 ){
                typeName = selection->typeName();
            }
        }
        return typeName;
    }else{
        return "Selection";
    }
}


/***
 * 4. Setters
 ***/

void EntitiesSelection::setBorderColor(const glm::vec4 &borderColor)
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->setBorderColor( borderColor );
    }
}


void EntitiesSelection::setEntityModelMatrix( const ResourceID &entityID,
                                              const glm::mat4 &modelMatrix )
{
    LOCK

    for( auto& selection : specializedEntitiesSelections_ ){
        if( selection->containsEntity( entityID ) ){
            selection->setEntityModelMatrix( entityID, modelMatrix );
        }
    }
}


/***
 * 5. Transformations
 ***/

void EntitiesSelection::translate( glm::vec3 direction )
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->translate( direction );
    }
}


void EntitiesSelection::rotateAroundPivot( GLfloat angle, glm::vec3 axis, glm::vec3 pivot)
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->rotateAroundPivot( angle, axis, pivot );
    }
}


void EntitiesSelection::rotateAroundIndividualCentroids( GLfloat angle, glm::vec3 axis)
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->rotateAroundIndividualCentroids( angle, axis );
    }
}


void EntitiesSelection::scaleAroundPivot( glm::vec3 scaleFactors, glm::vec3 pivot)
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->scaleAroundPivot( scaleFactors, pivot );
    }
}


void EntitiesSelection::scaleAroundIndividualCentroids( glm::vec3 scaleFactors )
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->scaleAroundIndividualCentroids( scaleFactors );
    }
}


void EntitiesSelection::applyTransformationMatrix(const glm::mat4 &transformation)
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->applyTransformationMatrix( transformation );
    }
}


void EntitiesSelection::setModelMatrix(const glm::mat4 &modelMatrix)
{
    LOCK
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->setModelMatrix( modelMatrix );
    }
}


/***
 * 6. Intersections
 ***/

bool EntitiesSelection::intersectsRay(glm::vec3 r0, glm::vec3 r1, ResourceID &closestEntity, float &minT) const
{
    LOCK
    // TODO: Return also the index of the closest intersected manager for
    // manipulating the closest entity faster after intersecting it?
    minT = FLT_MAX;
    float selectionT;
    ResourceID selectionClosestEntity;
    unsigned int nIntersectedSelections = 0;

    for( auto& selection : specializedEntitiesSelections_ ){
        if( selection->intersectsRay( r0, r1, selectionClosestEntity, selectionT ) ){
            nIntersectedSelections++;
        }
        if( selectionT < minT ){
            minT = selectionT;
            closestEntity = selectionClosestEntity;
        }
    }

    return (nIntersectedSelections > 0);
}


/***
 * 7. Updating (observer pattern)
 ***/

void EntitiesSelection::update()
{
    LOCK
    centroid_ = glm::vec3( 0.0f );
    unsigned int nSelections = 0;

    for( const AbstractEntitiesSet* selection : specializedEntitiesSelections_ ){
        if( selection->size() ){
            centroid_ += selection->centroid();
            nSelections++;
        }
    }

    if( nSelections ){
        centroid_ /= nSelections;
    }

    notifyObservers();
}


/***
 * 8. Drawing
 ***/

void EntitiesSelection::drawAll( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix ) const
{
    LOCK
    for( auto selection : specializedEntitiesSelections_ ){
        selection->drawAll( openGL, viewMatrix, projectionMatrix );
    }
}

} // namespace como
