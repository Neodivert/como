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

#include "local_entities_selection.hpp"

namespace como {

const unsigned int TRANSFORMATION_FLOAT_PRECISION = 10000;

/***
 * 1. Construction
 ***/

LocalEntitiesSelection::LocalEntitiesSelection( ServerInterfacePtr server,
                                                LocalLightsSelection* lightsSelection,
                                                LocalMeshesSelection* meshesSelection,
                                                LocalCamerasSelection* camerasSelection,
                                                PivotPointMode pivotPointMode ) :
    EntitiesSelection( lightsSelection, meshesSelection, camerasSelection ),
    ServerWriter( server )
{
    setPivotPointMode( pivotPointMode );
}


/***
 * 3. Getters
 ***/

glm::vec3 LocalEntitiesSelection::graphicPivotPoint() const
{
    LOCK
    switch( pivotPointMode() ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
        case PivotPointMode::SELECTION_CENTROID:
            return centroid();
        break;
        default:
            return glm::vec3( 0.0f, 0.0f, 0.0f );
        break;
    }
}


PivotPointMode LocalEntitiesSelection::pivotPointMode() const
{
    LOCK
    return pivotPointMode_;
}


/***
 * 4. Setters
 ***/

void LocalEntitiesSelection::setPivotPointMode( PivotPointMode pivotPointMode )
{
    LOCK
    pivotPointMode_ = pivotPointMode;
}


/***
 * 5. Transformations
 ***/

void LocalEntitiesSelection::translate( glm::vec3 direction )
{
    LOCK
    // Only aplly the transformation if there are drawables selected.
    if( size() ){
        SelectionTransformationCommand translationCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( direction );

        // Translate the selection.
        EntitiesSelection::translate( direction );

        // Send the translation command to server.
        translationCommand.setTranslation( direction );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( translationCommand ) ) );
    }
}


void LocalEntitiesSelection::rotateAroundPivot( GLfloat angle, glm::vec3 axis, glm::vec3 pivot )
{
    LOCK
    // Only aplly the transformation if there are entities selected.
    if( size() ){
        SelectionTransformationCommand rotationCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( angle, axis );

        // Rotate the selection.
        EntitiesSelection::rotateAroundPivot( angle, axis, pivot );

        // Send the rotation command to server.
        rotationCommand.setRotationAroundPivot( angle, axis, pivot );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( rotationCommand ) ) );
    }
}


void LocalEntitiesSelection::rotateAroundIndividualCentroids(GLfloat angle, glm::vec3 axis)
{
    LOCK
    // Only aplly the transformation if there are entities selected.
    if( size() ){
        SelectionTransformationCommand rotationCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( angle, axis );

        // Rotate the selection.
        EntitiesSelection::rotateAroundIndividualCentroids( angle, axis );

        // Send the rotation command to server.
        rotationCommand.setRotationAroundIndividualCentroids( angle, axis );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( rotationCommand ) ) );
    }
}


void LocalEntitiesSelection::scaleAroundPivot(glm::vec3 scaleFactors, glm::vec3 pivot)
{
    LOCK
    // Only aplly the transformation if there are entities selected.
    if( size() ){
        SelectionTransformationCommand scaleCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( scaleFactors );

        // Scale the selection.
        EntitiesSelection::scaleAroundPivot( scaleFactors, pivot );

        // Send the scale command to server.
        scaleCommand.setScaleAroundPivot( scaleFactors, pivot );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( scaleCommand ) ) );
    }
}


void LocalEntitiesSelection::scaleAroundIndividualCentroids(glm::vec3 scaleFactors)
{
    LOCK
    // Only aplly the transformation if there are entities selected.
    if( size() ){
        SelectionTransformationCommand scaleCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( scaleFactors );

        // Scale the selection.
        EntitiesSelection::scaleAroundIndividualCentroids( scaleFactors );

        // Send the scale command to server.
        scaleCommand.setScaleAroundIndividualCentroids( scaleFactors );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( scaleCommand ) ) );
    }
}


void LocalEntitiesSelection::rotate( GLfloat angle, glm::vec3 axis )
{
    LOCK
    switch( pivotPointMode() ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            rotateAroundIndividualCentroids( angle, axis );
        break;
        case PivotPointMode::SELECTION_CENTROID:
            rotateAroundPivot( angle, axis, centroid() );
        break;
        case PivotPointMode::WORLD_ORIGIN:
            rotateAroundPivot( angle, axis, glm::vec3( 0.0f, 0.0f, 0.0f ) );
        break;
    }
}


void LocalEntitiesSelection::scale( glm::vec3 scaleFactors )
{
    LOCK
    switch( pivotPointMode() ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            scaleAroundIndividualCentroids( scaleFactors );
        break;
        case PivotPointMode::SELECTION_CENTROID:
            scaleAroundPivot( scaleFactors, centroid() );
        break;
        case PivotPointMode::WORLD_ORIGIN:
            scaleAroundPivot( scaleFactors, glm::vec3( 0.0f, 0.0f, 0.0f ) );
        break;
    }
}


/***
 * 6. Auxiliar methods
 ***/

void LocalEntitiesSelection::roundTransformationMagnitude( glm::vec3& v )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    v.x = floorf( v.x * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    v.y = floorf( v.y * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    v.z = floorf( v.z * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
}


void LocalEntitiesSelection::roundTransformationMagnitude( float& angle, glm::vec3& v )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    angle = floorf( angle * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    roundTransformationMagnitude( v );
}

} // namespace como
