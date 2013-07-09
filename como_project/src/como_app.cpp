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

ComoApp* ComoApp::singlentonInstance = nullptr;

/***
 * 1. Initialization and destruction
 ***/

ComoApp::ComoApp()
{
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
