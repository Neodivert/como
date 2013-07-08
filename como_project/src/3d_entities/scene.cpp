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

void Scene::selectAll()
{
    selectedDrawables = nonSelectedDrawables;
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

void Scene::draw()
{
    DrawablesList::iterator it = nonSelectedDrawables.begin();

    for( ; it != nonSelectedDrawables.end(); it++ )
    {
        (*it)->draw();
    }
}
