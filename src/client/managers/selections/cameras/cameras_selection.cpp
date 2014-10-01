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

#include "cameras_selection.hpp"

namespace como {


/***
 * 1. Construction
 ***/

CamerasSelection::CamerasSelection( glm::vec4 borderColor ) :
    EntitiesSet( borderColor )
{}


/***
 * 3. Getters
 ***/

glm::mat4 CamerasSelection::cameraViewMatrix( const ResourceID& cameraID ) const
{
    return resources_.at( cameraID )->getViewMatrix();
}


/***
 * 4. Shader communication
 ***/

void CamerasSelection::sendCameraToShader( OpenGL& openGL, const ResourceID &cameraID ) const
{
    resources_.at( cameraID )->sendToShader( openGL );
}


} // namespace como
