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

const unsigned int TRANSFORMATION_FLOAT_PRECISION = 10000;

LocalDrawablesSelection::LocalDrawablesSelection( UserID localUserID, glm::vec4 selectionBorderColor, ServerInterfacePtr server ) :
    QObject(),
    DrawablesSelection( selectionBorderColor ),
    server_( server ),
    localUserID_( localUserID )
{
    // Initialize the unique ID to be given to the mext drawable added to this
    // selection (bind it to the local user).
    nextDrawableID_.creatorID = localUserID_;
    nextDrawableID_.drawableIndex = 0;
}


/***
 * 3. Setters
 ***/

void LocalDrawablesSelection::setPivotPointMode( PivotPointMode pivotPointMode )
{
    DrawablesSelection::setPivotPointMode( pivotPointMode );

    server_->sendCommand( CommandConstPtr( new ParameterChangeCommand( localUserID_, pivotPointMode ) ) );
}


void LocalDrawablesSelection::setMeshColor( const std::uint8_t* meshColor )
{
    DrawablesSelection::setMeshColor( meshColor );

    server_->sendCommand( CommandConstPtr( new MeshColorChangeCommand( localUserID_, meshColor ) ) );
}


/***
 * 4. Drawables management
 ***/

PackableDrawableID LocalDrawablesSelection::addDrawable( DrawablePtr drawable )
{
    PackableDrawableID newDrawableID = nextDrawableID_;

    DrawablesSelection::addDrawable( nextDrawableID_, drawable );

    // Increment the drawable index to be given to the next drawable added to
    // this selection.
    nextDrawableID_.drawableIndex = nextDrawableID_.drawableIndex.getValue() + 1;

    return newDrawableID;
}


/***
 * 5. Transformations
 ***/

void LocalDrawablesSelection::translate( glm::vec3 direction )
{
    SelectionTransformationCommand translationCommand( localUserID_ );

    // Round the transformation magnitude.
    roundTransformationMagnitude( direction );

    // Translate the selection.
    DrawablesSelection::translate( direction );

    // Send the translation command to server.
    translationCommand.setTranslation( &direction[0] );
    server_->sendCommand( CommandConstPtr( new SelectionTransformationCommand( translationCommand ) ) );
}


void LocalDrawablesSelection::rotate( GLfloat angle, glm::vec3 axis )
{
    SelectionTransformationCommand rotationCommand( localUserID_ );

    // Round the transformation magnitude.
    roundTransformationMagnitude( angle, axis );

    // Rotate the selection.
    DrawablesSelection::rotate( angle, axis );

    // Send the rotation command to server.
    rotationCommand.setRotation( angle, &axis[0] );
    server_->sendCommand( CommandConstPtr( new SelectionTransformationCommand( rotationCommand ) ) );
}


void LocalDrawablesSelection::scale( glm::vec3 scaleFactors )
{
    SelectionTransformationCommand scaleCommand( localUserID_ );

    // Round the transformation magnitude.
    roundTransformationMagnitude( scaleFactors );

    // Scale the selection.
    DrawablesSelection::scale( scaleFactors );

    // Send the scale command to server.
    scaleCommand.setScale( &scaleFactors[0] );
    server_->sendCommand( CommandConstPtr( new SelectionTransformationCommand( scaleCommand ) ) );
}


/***
 * 6. Updating
 ***/

void LocalDrawablesSelection::onChange()
{
    DrawablesSelection::onChange();

    emit hasChanged();
}


/***
 * 7. Auxiliar methods
 ***/

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
    angle = floorf( angle * TRANSFORMATION_FLOAT_PRECISION + 0.5) / TRANSFORMATION_FLOAT_PRECISION;
    v.x = floorf( v.x * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    v.y = floorf( v.y * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    v.z = floorf( v.z * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
}

} // namespace como
