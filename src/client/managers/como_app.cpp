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

#include "como_app.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ComoApp::ComoApp( ScenePtr scene, LogPtr log ) :
    scene_( scene ),
    transformationMode( TransformationMode::FREE ),
    transformationType( TransformationType::NONE ),
    log_( log )
{}


ComoApp::~ComoApp()
{
    log_->debug( "ComoApp destroyed\n" );
}


/***
 * 2. Getters
 ***/

TransformationMode ComoApp::getTransformationMode() const
{
    LOCK
    return transformationMode;
}


TransformationType ComoApp::getTransformationType() const
{
    LOCK
    return transformationType;
}


LogPtr ComoApp::getLog() const
{
    LOCK
    return log_;
}

ScenePtr ComoApp::getScene() const
{
    LOCK
    return scene_;
}


/***
 * 4. Setters (slots)
 ***/

void ComoApp::setTransformationMode( TransformationMode transformationMode )
{
    LOCK

    // Change the transformation mode.
    this->transformationMode = transformationMode;

    // Emit signal.
    emit transformationModeIndexChanged( static_cast< int >( transformationMode ) );

    // TODO: Reimplement this.
    //scene->renderNeeded();
}


void ComoApp::setTransformationType( TransformationType transformationType )
{
    LOCK
    this->transformationType = transformationType;
}

} // namespace como
