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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "como_app.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ComoApp::ComoApp()
{
    // Create a log
    log_ = LogPtr( new Log );

    // Set default app mode.
    appMode = AppMode::OBJECT;

    // Set default edition scope.
    editionScope = EditionScope::GLOBAL;

    // Set default transformation type.
    transformationType = TransformationType::NONE;

    checkOpenGL( "ComoApp constructor - before creating OpenGL context and scene" );

    // Set default OpenGL context.
    //oglContext = openGLEngine.createOpenGLContext();

    // Set default scene.
    scene = ScenePtr( new PublicScene( log_ ) );

    log_->debug( "OpenGL context is valid?: ", getScene()->getOpenGLContext()->isValid(), "\n" );
    checkOpenGL( "ComoApp constructor - after creating OpenGL context and scene" );
}


ComoApp::~ComoApp()
{
    log_->debug( "Destroying instance of ComoApp\n" );
}

/*
ComoApp* ComoApp::getInstance()
{
    if( singlentonInstance == nullptr ){
        singlentonInstance = new ComoApp;
    }
    return singlentonInstance;
}

void ComoApp::destroy()
{
    delete singlentonInstance;
}
*/

/***
 * 2. Getters
 ***/

AppMode ComoApp::getAppMode() const
{
    return appMode;
}


EditionScope ComoApp::getEditionScope() const
{
    return editionScope;
}


TransformationMode ComoApp::getTransformationMode() const
{
    return transformationMode;
}


TransformationType ComoApp::getTransformationType() const
{
    return transformationType;
}


ScenePtr ComoApp::getScene() const
{
    return scene;
}


LogPtr ComoApp::getLog() const
{
    return log_;
}


/***
 * 3. Setters (slots)
 ***/

void ComoApp::setAppMode( AppMode appMode )
{
    // Change the app mode.
    this->appMode = appMode;

    transformationType = TransformationType::NONE;

    // Emit a signal with the index of the new app mode.
    emit appModeIndexChanged( static_cast< int >( appMode ) );
}


void ComoApp::setTransformationMode( TransformationMode transformationMode )
{
    // Change the transformation mode.
    this->transformationMode = transformationMode;

    // Emit signal.
    emit transformationModeIndexChanged( static_cast< int >( transformationMode ) );

    scene->renderNeeded();
}


void ComoApp::setEditionScope( EditionScope editionScope )
{
    this->editionScope = editionScope;
}


void ComoApp::setTransformationType( TransformationType transformationType )
{
    this->transformationType = transformationType;
}

} // namespace como
