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

#include "local_drawables_selection.hpp"

namespace como {

LocalDrawablesSelection::LocalDrawablesSelection( UserID localUserID, ServerInterfacePtr server ) :
    DrawablesSelection(),
    server_( server ),
    localUserID_( localUserID )
{}


/***
 * 3. Setters
 ***/

void LocalDrawablesSelection::setPivotPointMode( PivotPointMode pivotPointMode )
{
    DrawablesSelection::setPivotPointMode( pivotPointMode );

    server_->sendCommand( CommandConstPtr( new ParameterChangeCommand( localUserID_, pivotPointMode ) ) );
}


/***
 * 4. Transformations
 ***/

void LocalDrawablesSelection::translate( glm::vec3 direction )
{
    SelectionTransformationCommand translationCommand( localUserID_ );

    // Translate the selection.
    DrawablesSelection::translate( direction );

    // Send the translation command to server.
    translationCommand.setTranslation( &direction[0] );
    server_->sendCommand( CommandConstPtr( new SelectionTransformationCommand( translationCommand ) ) );
}


void LocalDrawablesSelection::rotate( GLfloat angle, glm::vec3 axis )
{
    SelectionTransformationCommand rotationCommand( localUserID_ );

    // Rotate the selection.
    DrawablesSelection::rotate( angle, axis );

    // Send the rotation command to server.
    rotationCommand.setRotation( angle, &axis[0] );
    server_->sendCommand( CommandConstPtr( new SelectionTransformationCommand( rotationCommand ) ) );
}


void LocalDrawablesSelection::scale( glm::vec3 scaleFactors )
{
    SelectionTransformationCommand scaleCommand( localUserID_ );

    // Scale the selection.
    DrawablesSelection::scale( scaleFactors );

    // Send the scale command to server.
    scaleCommand.setScale( &scaleFactors[0] );
    server_->sendCommand( CommandConstPtr( new SelectionTransformationCommand( scaleCommand ) ) );
}


/***
 * 5. Updating
 ***/

void LocalDrawablesSelection::onChange()
{
    DrawablesSelection::onChange();

    emit hasChanged();
}

} // namespace como
