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

#include "local_drawables_selection.hpp"

namespace como {

const unsigned int TRANSFORMATION_FLOAT_PRECISION = 10000;

/*
LocalDrawablesSelection::LocalDrawablesSelection( ServerInterfacePtr server ) :
    DrawablesSelection( server->getLocalUserColor().toVec4() ),
    LocalResourcesSelection( server )
{}


***
 * 3. Setters
 ***

void LocalDrawablesSelection::setPivotPointMode( PivotPointMode pivotPointMode )
{
    DrawablesSelection::setPivotPointMode( pivotPointMode );

    sendCommandToServer( CommandConstPtr( new UserParameterChangeCommand( localUserID(), pivotPointMode ) ) );
}


***
 * 5. Transformations
 ***

void LocalDrawablesSelection::translate( glm::vec3 direction )
{
    // Only aplly the transformation if there are drawables selected.
    if( size() ){
        SelectionTransformationCommand translationCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( direction );

        // Translate the selection.
        DrawablesSelection::translate( direction );

        // Send the translation command to server.
        translationCommand.setTranslation( &direction[0] );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( translationCommand ) ) );
    }
}


void LocalDrawablesSelection::rotate( GLfloat angle, glm::vec3 axis )
{
    // Only aplly the transformation if there are drawables selected.
    if( size() ){
        SelectionTransformationCommand rotationCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( angle, axis );

        // Rotate the selection.
        DrawablesSelection::rotate( angle, axis );

        // Send the rotation command to server.
        rotationCommand.setRotation( angle, &axis[0] );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( rotationCommand ) ) );
    }
}


void LocalDrawablesSelection::scale( glm::vec3 scaleFactors )
{
    // Only aplly the transformation if there are drawables selected.
    if( size() ){
        SelectionTransformationCommand scaleCommand( localUserID() );

        // Round the transformation magnitude.
        roundTransformationMagnitude( scaleFactors );

        // Scale the selection.
        DrawablesSelection::scale( scaleFactors );

        // Send the scale command to server.
        scaleCommand.setScale( &scaleFactors[0] );
        sendCommandToServer( CommandConstPtr( new SelectionTransformationCommand( scaleCommand ) ) );
    }
}


***
 * 7. Auxiliar methods
 ***

void LocalDrawablesSelection::roundTransformationMagnitude( glm::vec3& v )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    v.x = floorf( v.x * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    v.y = floorf( v.y * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    v.z = floorf( v.z * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
}


void LocalDrawablesSelection::roundTransformationMagnitude( float& angle, glm::vec3& v )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    angle = floorf( angle * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    roundTransformationMagnitude( v );
}
*/
} // namespace como
