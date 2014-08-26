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

#include "drawables_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***

DrawablesManager::DrawablesManager( ServerInterfacePtr server, const PackableColor& localSelectionBorderColor, LogPtr log )
{
    // TODO: Set local selection color.
    glm::vec4 selectionColor = localSelectionBorderColor.toVec4();
    (void)( selectionColor );

    // Set a default mode for displaying the edges of the meshes in this
    // selection.
    displayEdges( MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED );
}


***
 * 3. Getters
 ***

string DrawablesManager::getResourceName( const ResourceID& resourceID ) const
{
    std::string resourceName;
    for( auto drawablesSelection : resourcesSelections_ ){
        if( drawablesSelection.second->existsDrawable( resourceID ) ){
            resourceName = drawablesSelection.second->getDrawableName( resourceID );
            return resourceName;
        }
    }

    throw std::runtime_error( "DrawablesManager::getResourceName() - drawable not found" );
}


DrawablesSelectionPtr DrawablesManager::getUserSelection( UserID userID ) const
{
    return resourcesSelections_.at( userID );
}


DrawablesSelectionPtr DrawablesManager::getUserSelection( UserID userID )
{
    return resourcesSelections_.at( userID );
}



bool DrawablesManager::existsDrawable( const ResourceID& id ) const
{
    DrawablesSelections::const_iterator it;

    for( it = resourcesSelections_.begin(); it != resourcesSelections_.end(); it++ ){
        if( it->second->existsDrawable( id ) ){
            return true;
        }
    }
    return false;
}


***
 * 4. Setters
 ***




***
 * 5. Drawables administration
 ***

ResourceID DrawablesManager::addDrawable( DrawablePtr drawable )
{
    //takeOpenGLContext();

    return getLocalResourcesSelection()->addResource( drawable );
}

void DrawablesManager::addDrawable( UserID userID, DrawablePtr drawable, ResourceID drawableID )
{
    //takeOpenGLContext();

    getUserSelection( userID )->addResource( drawableID, drawable );
}


void DrawablesManager::deleteSelection()
{
    deleteSelection( localUserID() );

    // Send Command to the server.
    requestSelectionDeletion();
}


void DrawablesManager::deleteSelection( const unsigned int& userId )
{
    getUserSelection( userId )->clear();

    notifyObservers();

    //emit renderNeeded();
}


***
 * 5. Selections management
 ***

void DrawablesManager::addDrawablesSelection( UserID userID, const PackableColor& selectionBorderColor )
{
    // TODO: Set color.
    DrawablesSelectionPtr newDrawablesSelection( new DrawablesSelection( selectionBorderColor.toVec4() ) );
    resourcesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( userID, newDrawablesSelection ) );
    newDrawablesSelection->Observable::addObserver( this );
}


***
 * 6. Drawables (de)seletion.
 ***

void DrawablesManager::selectDrawable( ResourceID drawableID )
{
    selectDrawable( drawableID, localUserID() );
}


void DrawablesManager::selectDrawable( ResourceID drawableID, UserID userID )
{
    // Retrieve user's selection.
    DrawablesSelection& userSelection = *( getUserSelection( userID ) );

    // Check if the desired drawable is among the non selected ones, and move
    // it to the user's selection in that case.
    getResourcesSelection( NO_USER )->moveResource( drawableID, userSelection );
}


void DrawablesManager::unselectAll()
{
    unselectAll( localUserID() );

    requestSelectionUnlock();
}


void DrawablesManager::unselectAll( UserID userID )
{
    DrawablesSelection& userSelection = *( getUserSelection( userID ) );

    // Move all drawables from user selection to non selected set.
    userSelection.moveAll( *getResourcesSelection( NO_USER ) );
}



ResourceID DrawablesManager::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    ResourceID closestObject;

    DrawablesSelection& userSelection = *( getUserSelection( localUserID() ) );

    r1 = glm::normalize( r1 );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        unselectAll();
    }

    // Check if the given ray intersect any of the non selected drawables.
    if( getResourcesSelection( NO_USER )->intersect( r0, r1, closestObject, minT ) ){
        // A non selected drawable has been intersected.
        log()->debug( "Object picked\n" );

        // Request to the server the lock of the intersected drawable.
        requestResourceLock( closestObject );

        // Save the collision point (in world coordinates) for returning it to
        // caller.
        worldCollisionPoint = r0 + r1 * minT;
    }else{
        // If user dind't selected any non-selected drawable, check if he / she
        // clicked on an already selected one.
        if( userSelection.intersect( r0, r1, closestObject, minT ) ){
            log()->debug( "RETURN 0\n" );
            ////emit renderNeeded();
            return NO_RESOURCE;
        }else{
            log()->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
            unselectAll();
        }

        // Even if no object is collided, we return in "worldCollisionPoint"
        // the "collision" with the near plane.
        //worldCollisionPoint = r0;
        worldCollisionPoint = glm::vec3( 0.0f );
    }

    // TODO: Send a request to server even when trying to select another
    // user's drawables?

    return closestObject;
}


***
 * 10. Drawing
 ***

void DrawablesManager::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    DrawablesSelections::const_iterator it;

    // Draw the user's selections.
    for( it = resourcesSelections_.begin(); it != resourcesSelections_.end(); it++  ){
        (it->second)->draw( openGL, viewMatrix, projectionMatrix );
    }
}


***
 * 8. Auxiliar methods
 ***

void DrawablesManager::highlightProperty( const void *property )
{
    for( auto drawablesSelection : resourcesSelections_ ){
        drawablesSelection.second->highlighDrawableProperty( property );
    }
}


***
 * 12. Resource management
 ***

void DrawablesManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    selectDrawable( resourceID, userID );
}

void DrawablesManager::unlockResourcesSelection( UserID userID )
{
    unselectAll( userID );
}

void DrawablesManager::deleteResourcesSelection( UserID userID )
{
    deleteSelection( userID );
}

*/

} // namespace como
