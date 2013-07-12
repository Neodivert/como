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

#include "scene.hpp"


/***
 * 1. Drawables administration
 ***/

void Scene::addCube( Cube* cube )
{
    shared_ptr< DrawableEntity> cubePtr( cube );

    addDrawable( cubePtr );
}

void Scene::addDrawable( shared_ptr<DrawableEntity> drawable )
{
    nonSelectedDrawables.push_back( drawable );
}


/***
 * 2. Drawables selection
 ***/

void Scene::selectDrawable( const unsigned int index )
{
    // TODO: In future versions, set a global unique id for each drawables,
    // whether they are selected or not.
    DrawablesList::iterator it = nonSelectedDrawables.begin();
    std::advance( it, index );

    selectedDrawables.splice( selectedDrawables.end(), nonSelectedDrawables, it );
}

void Scene::selectAll()
{
    selectedDrawables.splice( selectedDrawables.end(), nonSelectedDrawables );
    //selectedDrawables = nonSelectedDrawables;
}


int Scene::selectDrawableByRayPicking( glm::vec4 r0, glm::vec4 r1, bool addToSelection )
{
    const float MAX_DISTANCE = 9999999.0f;
    float distance;
    unsigned int currentObject = 0;
    unsigned int closestObject = 0;
    float minDistance = MAX_DISTANCE;

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        nonSelectedDrawables.splice( nonSelectedDrawables.end(), selectedDrawables );
    }

    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    DrawablesList::iterator it;
    for( it = nonSelectedDrawables.begin(); it != nonSelectedDrawables.end(); it++, currentObject++ ){
        distance = (*it)->intersects( r0, r1 );
        if( ( distance > 0.0f ) && (distance < minDistance ) ){
            cout << "selectDrawableByRayPicking - new closest object : " << distance << endl;
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minDistance = distance;
        }
    }

    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    for( it = selectedDrawables.begin(); it != selectedDrawables.end(); it++, currentObject++ ){
        distance = (*it)->intersects( r0, r1 );
        if( ( distance > 0.0f ) && (distance < minDistance ) ){
            cout << "selectDrawableByRayPicking - new closest object : " << distance << endl;
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minDistance = distance;
        }
    }

    // If there were intersections, select the closest one.
    if( minDistance < MAX_DISTANCE ){
        cout << "Final closest object: " << closestObject << endl;
        selectDrawable( closestObject );
    }
}


/***
 * 3. Transformations
 ***/

void Scene::translateSelectedDrawables( const GLfloat& tx, const GLfloat& ty, const GLfloat& tz )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->translate( tx, ty, tz );
    }
}


/***
 * 4. Drawing
 ***/

void Scene::draw( Camera* camera )
{
    DrawablesList::iterator it = nonSelectedDrawables.begin();

    for( ; it != nonSelectedDrawables.end(); it++ )
    {
        (*it)->draw( camera );
    }
    for( it = selectedDrawables.begin(); it != selectedDrawables.end(); it++ )
    {
        (*it)->draw( camera );
    }
}
