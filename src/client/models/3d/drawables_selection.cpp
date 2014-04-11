/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "drawables_selection.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

DrawablesSelection::DrawablesSelection() :
    centroid_( 0.0f, 0.0f, 0.0f, 1.0f ),
    pivotPointMode_( PivotPointMode::MEDIAN_POINT )
{
}


/***
 * 2. Getters
 ***/

glm::vec4 DrawablesSelection::getCentroid() const
{
    return centroid_;
}


PivotPointMode DrawablesSelection::getPivotPointMode() const
{
    return pivotPointMode_;
}


/***
 * 3. Setters
 ***/

void DrawablesSelection::setPivotPointMode( PivotPointMode pivotPointMode )
{
    pivotPointMode_ = pivotPointMode;
}


/***
 * 4. Transformations
 ***/

void DrawablesSelection::translate( glm::vec3 direction )
{
    DrawablesMap::iterator drawable;

    // Translate every drawable in the selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        drawable->second->translate( direction );
    }

    // Update the selection's centroid.
    updateSelectionCentroid();
}


void DrawablesSelection::rotate( GLfloat angle, glm::vec3 axis )
{
    DrawablesMap::iterator drawable;

    // Rotate every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->rotate( angle, axis, glm::vec3( drawable->second->getCentroid() ) );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->rotate( angle, axis, glm::vec3( centroid_ ) );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->rotate( angle, axis );
            }
        break;
    }

    // Update the selection's centroid.
    updateSelectionCentroid();
}


void DrawablesSelection::scale( glm::vec3 scaleFactors )
{
    DrawablesMap::iterator drawable;

    // Scale every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->scale( scaleFactors, glm::vec3( drawable->second->getCentroid() ) );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->scale( scaleFactors, glm::vec3( centroid_ ) );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
                drawable->second->scale( scaleFactors );
            }
        break;
    }

    // Update the selection's centroid.
    updateSelectionCentroid();
}


/***
 * 5. Centroid updating
 ***/

void DrawablesSelection::updateSelectionCentroid()
{
    DrawablesMap::iterator drawable;

    // Initialize the selection's centroid.
    centroid_ = glm::vec4( 0.0f );

    // Sum every drawable's centroid in the selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        centroid_ += drawable->second->getCentroid();
    }

    // Get the selection centroid.
    centroid_ /= drawables_.size();
    centroid_.w = 1.0f;

    /*
    // Map the pivot point VBO to client memory and update the selection centroid
    // coordinates (for drawing).
    glBindBuffer( GL_ARRAY_BUFFER, selectionCentroidVBO );
    selectionCentroidBuffer = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    for( unsigned int i=0; i<3; i++ ){
        selectionCentroidBuffer[i] = selectionCentroid[i];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
    */
}


/***
 * 6. Drawables management
 ***/


void DrawablesSelection::addDrawable( PackableDrawableID drawableID, DrawablePtr drawable )
{
    // Insert the given pair <ID, drawable> into the selection.
    drawables_[ drawableID ] = drawable;
}


bool DrawablesSelection::moveDrawable( PackableDrawableID drawableID, DrawablesSelection& destinySelection )
{
    // Check if the required drawable is in this selection.
    if( drawables_.count( drawableID ) ){
        // Move drawable from current selection to destiny one.
        destinySelection.addDrawable( drawableID, drawables_.at( drawableID ) );

        // Erase drawable from current selection.
        drawables_.erase( drawableID );

        // Update the selection centroid.
        updateSelectionCentroid();

        // Drawable found and moved.
        return true;
    }else{
        // Drawable not found.
        return false;
    }
}


void DrawablesSelection::moveAll( DrawablesSelection& destinySelection )
{
    DrawablesMap::iterator drawable;

    // COPY every drawable from current selection to destiny.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        destinySelection.addDrawable( drawable->first, drawable->second );
    }

    // Clear the current selection.
    clear();
}


void DrawablesSelection::clear()
{
    // Clear the current selection.
    drawables_.clear();

    // Update the selection centroid.
    centroid_ = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
}


/***
 * 7. Ray picking
 ***/

bool DrawablesSelection::intersect( glm::vec3 r0, glm::vec3 r1, PackableDrawableID& closestDrawable, float& minT ) const
{
    DrawablesMap::const_iterator drawable;
    float t;
    bool drawableIntersected = false;

    // Check if the given ray intersects any drawable in the selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        drawable->second->intersects( r0, r1, t );

        // New closest object, get its ID and distance.
        if( ( t >= 0.0f ) && ( t < minT ) ){
            drawableIntersected = true;

            closestDrawable = drawable->first;
            minT = t;
        }
    }

    return drawableIntersected;
}


/***
 * 8. Drawing
 ***/

void DrawablesSelection::draw( const glm::mat4& viewProjMatrix, const GLfloat* contourColor ) const
{
    DrawablesMap::const_iterator drawable;

    // Draw every drawable in current selection.
    for( drawable = drawables_.begin(); drawable != drawables_.end(); drawable++ ){
        drawable->second->draw( viewProjMatrix, contourColor );
    }
}

} // namespace como
