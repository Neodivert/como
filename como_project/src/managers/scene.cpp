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

namespace como {



DrawableTypes drawableTypes =
{
    DrawableType::CUBE
};


DrawableTypeStrings drawableTypeStrings =
{
    QString::fromUtf8( "Cube" )
};


/***
 * 1. Drawables administration
 ***/

void Scene::addDrawable( shared_ptr<Drawable> drawable )
{
    //unselectAll();

    nonSelectedDrawables.push_back( drawable );

    emit renderNeeded();
}

void Scene::addCube( Cube* cube )
{
    DrawablePtr cubePtr( cube );

    addDrawable( cubePtr );
}

void Scene::addDrawable( DrawableType drawableType )
{
    DrawablePtr drawable;
    Cube* cube;

    switch( drawableType ){
        case DrawableType::CUBE:
            cube = new Cube;
            drawable = DrawablePtr( cube );
        break;
    }

    addDrawable( drawable );
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

    emit renderNeeded();
}

void Scene::selectAll()
{
    selectedDrawables.splice( selectedDrawables.end(), nonSelectedDrawables );

    emit renderNeeded();
}

void Scene::unselectAll()
{
    nonSelectedDrawables.splice( nonSelectedDrawables.end(), selectedDrawables );

    emit renderNeeded();
}


int Scene::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    float t = MAX_T;
    unsigned int currentObject = 0;
    unsigned int closestObject = -1;

    r1 = glm::normalize( r1 );

    cout << "Scene::selectDrawableByRayPicking" << endl
         << "\tr0 : (" << r0.x << ", " << r0.y << ", " << r0.z << ")" << endl
         << "\tr1 : (" << r1.x << ", " << r1.y << ", " << r1.z << ")" << endl;


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
        (*it)->intersects( r0, r1, t );
        if( ( t >= 0.0f ) && (t < minT ) ){
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minT = t;
        }
    }

    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    for( it = selectedDrawables.begin(); it != selectedDrawables.end(); it++, currentObject++ ){
        (*it)->intersects( r0, r1, t );
        if( ( t > 0.0f ) && (t < minT ) ){
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minT = t;
            cout << "RETURN 0" << endl;
            emit renderNeeded();
            return 0;
        }
    }

    // If there were intersections, select the closest one.
    if( minT < MAX_T ){
        cout << "FINAL CLOSEST OBJECT: " << closestObject << endl
             << "\t min t: " << minT << ")" << endl
             << "\t min distance: " << glm::distance( glm::vec3( 0.0f, 0.0f, 0.0f ), r1 * t ) << endl;
        selectDrawable( closestObject );
    }else{
        cout << "NO CLOSEST OBJECT. Unselecting all" << endl;
        unselectAll();
    }

    emit renderNeeded();

    return closestObject;
}


glm::vec4 Scene::getSelectionCentroid() const
{
    glm::vec4 selectionCentroid;
    DrawablesList::const_iterator it = selectedDrawables.begin();

    cout << "Getting selection centroid" << endl;

    if( selectedDrawables.size() ){
        for( ; it != selectedDrawables.end(); it++ )
        {
            selectionCentroid += (*it)->getCentroid();
        }
        selectionCentroid /= selectedDrawables.size();
    }

    return selectionCentroid;
}

/***
 * 3. Transformations
 ***/

void Scene::translateSelection( const glm::vec3& direction )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->translate( direction );
    }
    emit renderNeeded();
}


void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->rotate( angle, axis );
    }
    emit renderNeeded();
}


void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->rotate( angle, axis, pivot );
    }
    emit renderNeeded();
}

void Scene::deleteSelection()
{
    // Delete selected drawables.
    selectedDrawables.clear();

    emit renderNeeded();
}

/***
 * 4. Drawing
 ***/

void Scene::draw()
{
    DrawablesList::iterator it = nonSelectedDrawables.begin();

    for( ; it != nonSelectedDrawables.end(); it++ )
    {
        (*it)->draw();
    }
    for( it = selectedDrawables.begin(); it != selectedDrawables.end(); it++ )
    {
        (*it)->draw( true );
    }
}

} // namespace como
