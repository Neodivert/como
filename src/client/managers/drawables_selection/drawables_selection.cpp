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

#include "drawables_selection.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

DrawablesSelection::DrawablesSelection( glm::vec4 borderColor ) :
    borderColor_( borderColor ),
    centroid_( 0.0f, 0.0f, 0.0f, 1.0f ),
    pivotPointMode_( PivotPointMode::MEDIAN_POINT ),
    highlightedProperty_( nullptr ),
    displayEdges_( true )
{
}

/*
DrawablesSelection::DrawablesSelection( const DrawablesSelection& b ) :
    ResourcesSelection( b ),
    borderColor_( b.borderColor_ ),
    centroid_( b.centroid_ ),
    pivotPointMode_( b.pivotPointMode_ ),
    highlightedProperty_( nullptr ),
    displayEdges_( b.displayEdges_ )
{
    DrawablesMap::const_iterator it;

    // Clone all the drawables held by b.
    for( it = b.resources_.begin(); it != b.resources_.end(); it++ ){ // Make this in MeshesSelection copy constructor.
        resources_.insert( std::pair< ResourceID, DrawablePtr >(
                               it->first,
                               it->second->clone()
                            ) );
    }

    // This selection has changed, so indicate it.
    notifyObservers();
}


DrawablesSelection::DrawablesSelection( DrawablesSelection&& b ) :
    resources_( b.resources_ ),
    borderColor_( b.borderColor_ ),
    centroid_( b.centroid_ ),
    pivotPointMode_( b.pivotPointMode_ ),
    displayEdges_( b.displayEdges_ )
{
}
*/

/***
 * 2. Getters
 ***/

glm::vec4 DrawablesSelection::getCentroid() const
{
    glm::vec4 centroid;

    mutex_.lock();
    centroid = centroid_;
    mutex_.unlock();

    return centroid;
}


PivotPointMode DrawablesSelection::getPivotPointMode() const
{
    PivotPointMode pivotPointMode;

    mutex_.lock();
    pivotPointMode = pivotPointMode_;
    mutex_.unlock();

    return pivotPointMode;
}


bool DrawablesSelection::contains( DrawableType drawableType ) const
{
    DrawablesMap::const_iterator it;

    mutex_.lock();

    for( it = resources_.begin(); it != resources_.end(); it++ ){
        if( it->second->getType() == drawableType ){
            mutex_.unlock();
            return true;
        }
    }

    mutex_.unlock();
    return false;
}


bool DrawablesSelection::contains( MeshType meshType ) const
{
    DrawablesMap::const_iterator it;
    const Drawable* drawable = nullptr;

    mutex_.lock();

    for( it = resources_.begin(); it != resources_.end(); it++ ){
        drawable = it->second.get();

        if( ( drawable->getType() == DrawableType::MESH ) &&
                ( dynamic_cast< const Mesh* >( drawable ) )->getType() == meshType ){
            mutex_.unlock();
            return true;
        }
    }

    mutex_.unlock();
    return false;
}


bool DrawablesSelection::existsDrawable( const ResourceID &id ) const
{
    return resources_.count( id );
}

std::string DrawablesSelection::getDrawableName( const ResourceID& id ) const
{
    return resources_.at( id )->getName();
}


std::string DrawablesSelection::getName() const
{
    const Drawable* drawable = nullptr;

    if( size() == 0 ){
        return std::string( "Nothing selected" );
    }else if( size() == 1 ){
        drawable = resources_.begin()->second.get();

        return std::string( drawable->getName() );
    }else{
        return getTypeName();
    }
}

std::string DrawablesSelection::getTypeName() const
{
    const Drawable* drawable = nullptr;

    if( size() == 0 ){
        return std::string( "Nothing selected" );
    }else if( size() == 1 ){
        drawable = resources_.begin()->second.get();
        switch( drawable->getType() ){
            case DrawableType::MESH:
                switch( ( dynamic_cast< const Mesh* >( drawable ) )->getType() ){
                    case MeshType::MESH:
                        return std::string( "Primitive mesh" );
                    break;
                    case MeshType::CAMERA:
                        return std::string( "Camera" );
                    break;
                    case MeshType::LIGHT:
                        // TODO: Include more lights.
                        return std::string( "Directional light" );
                    break;
                    default:
                        return std::string( "Undefined mesh" );
                    break;
                }
            break;
            default:
                return std::string( "Undefined drawable" );
            break;
        }
    }else{
        return std::string( "Selection" );
    }
}


/***
 * 3. Setters
 ***/

void DrawablesSelection::setPivotPointMode( PivotPointMode pivotPointMode )
{
    mutex_.lock();
    pivotPointMode_ = pivotPointMode;

    // This selection has changed, so indicate it.
    notifyObservers();

    mutex_.unlock();
}


void DrawablesSelection::displayEdges( bool displayEdges )
{
    displayEdges_ = displayEdges;
}


/***
 * 4. Transformations
 ***/

void DrawablesSelection::translate( glm::vec3 direction )
{   
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // Translate every drawable in the selection.
    for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
        drawable->second->translate( direction );

        notifyElementUpdate( drawable->first );
    }

    updateSelectionCentroid();

    mutex_.unlock();
}


void DrawablesSelection::rotate( GLfloat angle, glm::vec3 axis )
{
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // Rotate every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
                drawable->second->rotate( angle, axis, glm::vec3( drawable->second->getCentroid() ) );

                notifyElementUpdate( drawable->first );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
                drawable->second->rotate( angle, axis, glm::vec3( centroid_ ) );

                notifyElementUpdate( drawable->first );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
                drawable->second->rotate( angle, axis );

                notifyElementUpdate( drawable->first );
            }
        break;
    }

    updateSelectionCentroid();


    mutex_.unlock();
}


void DrawablesSelection::scale( glm::vec3 scaleFactors )
{
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // Scale every drawable in the selection according to the selected
    // pivot point mode.
    switch( pivotPointMode_ ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
                drawable->second->scale( scaleFactors, glm::vec3( drawable->second->getCentroid() ) );

                notifyElementUpdate( drawable->first );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
                drawable->second->scale( scaleFactors, glm::vec3( centroid_ ) );

                notifyElementUpdate( drawable->first );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
                drawable->second->scale( scaleFactors );

                notifyElementUpdate( drawable->first );
            }
        break;
    }

    updateSelectionCentroid();

    mutex_.unlock();
}


/***
 * 5. Updating
 ***/

void DrawablesSelection::updateSelectionCentroid()
{
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // Initialize the selection's centroid.
    centroid_ = glm::vec4( 0.0f );

    // Sum every drawable's centroid in the selection.
    for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
        centroid_ += drawable->second->getCentroid();
    }

    // Get the selection centroid.
    if( resources_.size() ){
        centroid_ /= resources_.size();
    }
    centroid_.w = 1.0f;

    mutex_.unlock();
}

/***
 * 6. Drawables management
 ***/


void DrawablesSelection::addDrawable( ResourceID drawableID, DrawablePtr drawable )
{
    mutex_.lock();

    // Insert the given pair <ID, drawable> into the selection.
    resources_[ drawableID ] = drawable;

    // This selection has changed, so indicate it.
    notifyElementInsertion( drawableID );

    mutex_.unlock();
}

// TODO: ¿Possible dead lock (if A moves a drawable to B and B moves a drawable
// to A at the same time)?
bool DrawablesSelection::moveDrawable( ResourceID drawableID, DrawablesSelection& destinySelection )
{
    mutex_.lock();

    // Check if the required drawable is in this selection.
    if( resources_.count( drawableID ) ){
        // Move drawable from current selection to destiny one.
        destinySelection.addDrawable( drawableID, resources_.at( drawableID ) );

        // Erase drawable from current selection.
        resources_.erase( drawableID );

        // This selection has changed, so indicate it.
        //notifyElementDeletion( drawableID );

        mutex_.unlock();
        return true;
    }else{

        // Drawable not found.
        mutex_.unlock();
        return false;
    }
}


// TODO: ¿Possible dead lock (if A moves all to B and B moves all to A at the
// same time)?
void DrawablesSelection::moveAll( DrawablesSelection& destinySelection )
{
    DrawablesMap::iterator drawable;

    mutex_.lock();

    // COPY every drawable from current selection to destiny.
    for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
        destinySelection.addDrawable( drawable->first, drawable->second );

        //notifyElementDeletion( drawable->first );
    }

    // Clear the current selection.
    clear();

    mutex_.unlock();
}


void DrawablesSelection::clear()
{
    mutex_.lock();

    // Clear the current selection.
    resources_.clear();

    mutex_.unlock();
}


void DrawablesSelection::erase()
{
    mutex_.unlock();

    for( auto drawable : resources_ ){
        notifyElementDeletion( drawable.first );
    }

    clear();

    mutex_.lock();
}


/***
 * 7. Ray picking
 ***/

bool DrawablesSelection::intersect( glm::vec3 r0, glm::vec3 r1, ResourceID& closestDrawable, float& minT ) const
{
    DrawablesMap::const_iterator drawable;
    float t;
    bool drawableIntersected = false;

    mutex_.lock();

    // Check if the given ray intersects any drawable in the selection.
    for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
        drawable->second->intersects( r0, r1, t );

        // New closest object, get its ID and distance.
        if( ( t >= 0.0f ) && ( t < minT ) ){
            drawableIntersected = true;

            closestDrawable = drawable->first;
            minT = t;
        }
    }

    return drawableIntersected;

    mutex_.unlock();
}


/***
 * 8. Drawing
 ***/

void DrawablesSelection::draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    DrawablesMap::const_iterator drawable;

    mutex_.lock();

    // Draw every drawable in current selection.
    for( drawable = resources_.begin(); drawable != resources_.end(); drawable++ ){
        glm::vec4 borderColor;

        if( displayEdges_ || drawable->second->containsProperty( highlightedProperty_ ) ){
            if( drawable->second->containsProperty( highlightedProperty_ ) ){
                borderColor = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
            }else{
                borderColor = borderColor_;
            }

            drawable->second->draw( openGL, viewMatrix, projectionMatrix, &borderColor[0] );
        }else{
            drawable->second->draw( openGL, viewMatrix, projectionMatrix, nullptr );
        }
    }

    mutex_.unlock();
}


/***
 * 10. Auxiliar methods
 ***/

void DrawablesSelection::highlighDrawableProperty( const void* property )
{
    highlightedProperty_ = property;
}

} // namespace como
