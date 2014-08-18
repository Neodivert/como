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

EntitiesSelection::EntitiesSelection( LightsSelection* lightsSelection, PivotPointMode pivotPointMode ) :
    lightsSelection_( lightsSelection ),
    centroid_( 0.0f )
{
    specializedEntitiesSelections_.push_back( lightsSelection_ );

    for( auto& selection : specializedEntitiesSelections_ ){
        selection->Observable::addObserver( this );
    }

    setPivotPointMode( pivotPointMode );
}


/***
 * 3. Getters
 ***/

glm::vec3 EntitiesSelection::centroid() const
{
    return centroid_;
}


PivotPointMode EntitiesSelection::pivotPointMode() const
{
    return pivotPointMode_;
}


glm::vec4 EntitiesSelection::borderColor() const
{
    return lightsSelection_->borderColor();
}


unsigned int EntitiesSelection::size() const
{
    unsigned int size = 0;

    for( auto selection : specializedEntitiesSelections_ ){
        size += selection->size();
    }

    return size;
}


/***
 * 4. Setters
 ***/

void EntitiesSelection::setPivotPointMode( PivotPointMode mode )
{
    pivotPointMode_ = mode;
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->setPivotPointMode( mode );
    }
}


void EntitiesSelection::setBorderColor(const glm::vec4 &borderColor)
{
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->setBorderColor( borderColor );
    }
}


/***
 * 5. Transformations
 ***/

void EntitiesSelection::translate( glm::vec3 direction )
{
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->translate( direction );
    }
}


void EntitiesSelection::rotate(const GLfloat &angle, const glm::vec3 &axis)
{
    if( pivotPointMode_ == PivotPointMode::MEDIAN_POINT ){
        for( auto& selection : specializedEntitiesSelections_ ){
            selection->rotate( angle, axis );
        }
    }else{
        for( auto& selection : specializedEntitiesSelections_ ){
            selection->rotateAroundPivot( angle, axis, centroid() );
        }
    }
}


void EntitiesSelection::scale(const glm::vec3 &scaleFactors)
{
    if( pivotPointMode_ == PivotPointMode::MEDIAN_POINT ){
        for( auto& selection : specializedEntitiesSelections_ ){
            selection->scaleAroundPivot( scaleFactors, centroid() );
        }
    }else{
        for( auto& selection : specializedEntitiesSelections_ ){
            selection->scale( scaleFactors );
        }
    }
}


void EntitiesSelection::applyTransformationMatrix(const glm::mat4 &transformation)
{
    for( auto& selection : specializedEntitiesSelections_ ){
        selection->applyTransformationMatrix( transformation );
    }
}


/***
 * 6. Intersections
 ***/

bool EntitiesSelection::intersectsRay(glm::vec3 r0, glm::vec3 r1, ResourceID &closestEntity, float &minT) const
{
    // TODO: Select / lock only the closest entity of all personalized entities
    // selections.
    return lightsSelection_->intersectsRay( r0, r1, closestEntity, minT );
}


/***
 * 7. Updating (observer pattern)
 ***/

void EntitiesSelection::update()
{
    centroid_ = glm::vec3( 0.0f );
    unsigned int nEntities = 0;

    for( auto selection : specializedEntitiesSelections_ ){
        centroid_ += selection->centroid();

        nEntities += selection->size();
    }

    if( nEntities ){
        centroid_ /= nEntities;
    }
}


/***
 * 8. Drawing
 ***/

void EntitiesSelection::drawAll( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix ) const
{
    for( auto selection : specializedEntitiesSelections_ ){
        selection->drawAll( openGL, viewMatrix, projectionMatrix );
    }
}

} // namespace como
