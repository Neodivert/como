/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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
#include "gui/opengl_engine.hpp"

ComoApp* ComoApp::singlentonInstance = nullptr;

/***
 * 1. Initialization and destruction
 ***/

ComoApp::ComoApp()
{
    OpenGLEngine openGLEngine;

    // Set default app mode.
    appMode = MODE_EDITION;

    // Set default edition scope.
    editionScope = GLOBAL;

    // Set default edition submode.
    editionSubMode = TRANSLATION;

    // Set default OpenGL context.
    oglContext = openGLEngine.createOpenGLContext();

    // Set default scene.
    scene = openGLEngine.createScene( oglContext.get() );
}

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

/***
 * 2. Setters and getters
 ***/

void ComoApp::setAppMode( AppMode appMode )
{
    this->appMode = appMode;
}

AppMode ComoApp::getAppMode() const
{
    return appMode;
}

void ComoApp::setEditionScope( EditionScope editionScope )
{
    this->editionScope = editionScope;
}

EditionScope ComoApp::getEditionScope() const
{
    return editionScope;
}

void ComoApp::setEditionSubMode( EditionSubMode editionSubMode )
{
    this->editionSubMode = editionSubMode;
}

EditionSubMode ComoApp::getEditionSubMode() const
{
    return editionSubMode;
}

shared_ptr< Scene > ComoApp::getScene() const
{
    return scene;
}

shared_ptr< QOpenGLContext > ComoApp::getOpenGLContext() const
{
    return oglContext;
}
