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

#include "drawables_manager.hpp"

namespace como {

/***
 * 1. Construction
 ***/

DrawablesManager::DrawablesManager( UserID localUserID, ServerInterfacePtr server, LogPtr log ) :
    Changeable( true ),
    localUserID_( localUserID ),
    server_( server ),
    log_( log )
{
    // Create an empty drawables selection for the local user.
    addDrawablesSelection( localUserID_ );
}


/***
 * 3. Selections management
 ***/

void DrawablesManager::addDrawablesSelection( UserID userID )
{
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelection >( userID, DrawablesSelection() ) );
}


/***
 * 4. Drawables (de)seletion.
 ***/

void DrawablesManager::selectDrawable( PackableDrawableID drawableID )
{
    selectDrawable( drawableID, localUserID_ );
}


void DrawablesManager::selectDrawable( PackableDrawableID drawableID, UserID userID )
{
    bool drawableFound = false;

    // Retrieve user's selection.
    DrawablesSelection& userSelection = drawablesSelections_.at( userID );

    // Check if the desired drawable is among the non selected ones, and move
    // it to the user's selection in that case.
    drawableFound = nonSelectedDrawables_.moveDrawable( drawableID, userSelection );

    if( !drawableFound ){
        throw std::runtime_error( "ERROR when selecting drawable. Drawable not found" );
    }

    setChanged();
}


void DrawablesManager::unselectAll()
{
    unselectAll( localUserID_ );

    // Send command to the server.
    server_->sendCommand( CommandConstPtr( new FullDeselectionCommand( localUserID_ ) ) );
}


void DrawablesManager::unselectAll( UserID userID )
{
    DrawablesSelection& userSelection = drawablesSelections_.at( userID );

    // Move all drawables from user selection to non selected set.
    userSelection.moveAll( nonSelectedDrawables_ );

    setChanged();
}



PackableDrawableID DrawablesManager::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    PackableDrawableID closestObject;

    DrawablesSelection& userSelection = drawablesSelections_.at( localUserID_ );

    r1 = glm::normalize( r1 );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        unselectAll();
    }

    // Check if the given ray intersect any of the non selected drawables.
    if( nonSelectedDrawables_.intersect( r0, r1, closestObject, minT ) ){
        // A non selected drawable has been intersected.
        log_->debug( "Object picked\n" );

        // Send a DRAWABLE_SELECTION command to the server.
        server_->sendCommand( CommandConstPtr(
                                 new DrawableSelectionCommand( localUserID_,
                                                     closestObject,
                                                     addToSelection ) ) );

        // Insert the selected drawable's ID in a queue of pending selections.
        localUserPendingSelections_.push( closestObject );
    }else{
        // If user dind't selected any non-selected drawable, check if he / she
        // clicked on an already selected one.
        if( userSelection.intersect( r0, r1, closestObject, minT ) ){
            log_->debug( "RETURN 0\n" );
            //emit renderNeeded();
            return NULL_DRAWABLE_ID;
        }else{
            log_->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
            unselectAll();
        }
    }

    /*
    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    DrawablesSelection::iterator it;
    for( it = nonSelectedDrawables_.begin(); it != nonSelectedDrawables_.end(); it++ ){
        it->second->intersects( r0, r1, t );
        if( ( t >= 0.0f ) && (t < minT ) ){
            // New closest object, get its ID and distance.
            closestObject = it->first;
            minT = t;
        }
    }

    // If user dind't selected any non-selected drawable, check if he / she
    // clicked on an already selected one.
    if( minT == MAX_T ){
        for( it = userSelection.begin(); it != userSelection.end(); it++ ){
            it->second->intersects( r0, r1, t );
            if( ( t > 0.0f ) && (t < minT ) ){
                // New closest object, get its index and distance.
                closestObject = it->first;
                minT = t;
                log_->debug( "RETURN 0\n" );
                emit renderNeeded();
                return NULL_DRAWABLE_ID;
            }
        }
    }



    // If there were intersections, select the closest one.
    if( minT < MAX_T ){
        log_->debug( "FINAL CLOSEST OBJECT: (", closestObject.creatorID, ", ", closestObject.drawableIndex, ")\n",
                     "\t min t: ", minT, ")\n",
                     "\t min distance: ", glm::distance( glm::vec3( 0.0f, 0.0f, 0.0f ), r1 * t ), "\n" );

        // Send a DRAWABLE_SELECTION command to the server.
        server_.sendCommand( CommandConstPtr(
                                 new DrawableSelectionCommand( localUserID_,
                                                     closestObject,
                                                     addToSelection ) ) );

        // Insert the selected drawable's ID in a queue of pending selections.
        localUserPendingSelections_.push( closestObject );

        //selectDrawable( closestObject );
    }else{
        log_->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
        unselectAll();
    }
    */

    setChanged();

    return closestObject;
}

} // namespace como
