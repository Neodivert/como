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
#include "opengl_engine.hpp"

#include <iostream>
using namespace std;

ComoApp* ComoApp::singlentonInstance = nullptr;

std::map<QString, AppMode> appModeStrings =
{
    { QString::fromUtf8( "Creation" ), AppMode::CREATION },
    { QString::fromUtf8( "Edition" ), AppMode::EDITION }
};


std::map<EditionScope, std::string> editionScopeStrings =
{
    { EditionScope::LOCAL, "Local" },
    { EditionScope::GLOBAL, "Global" }
};

/***
 * 1. Initialization and destruction
 ***/

ComoApp::ComoApp()
{
    // Create an OpenGL engine (used for creating and initializing
    // an OpenGL context and a scene.
    OpenGLEngine openGLEngine;

    // Set default app mode.
    appMode = AppMode::EDITION;

    // Set default edition scope.
    editionScope = EditionScope::GLOBAL;

    // Set default edition submode.
    editionSubMode = EditionSubMode::TRANSLATION;

    // Set default OpenGL context.
    oglContext = openGLEngine.createOpenGLContext();

    // Set default scene.
    scene = openGLEngine.createScene( oglContext.get() );
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


/***
 * 4. Setters (slots)
 ***/

void ComoApp::setAppMode( AppMode appMode )
{
    cout << "Changing app mode" << endl;
    this->appMode = appMode;
}



void ComoApp::setEditionScope( EditionScope editionScope )
{
    this->editionScope = editionScope;
}



void ComoApp::setEditionSubMode( EditionSubMode editionSubMode )
{
    this->editionSubMode = editionSubMode;
}




