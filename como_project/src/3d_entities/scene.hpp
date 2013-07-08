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

#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include <memory>
#include "drawable_entity.hpp"
#include "cube.hpp"

typedef std::list< std::shared_ptr< DrawableEntity> > DrawablesList;

class Scene
{
    private:
        // Scene's drawables not selected by user.
        DrawablesList nonSelectedDrawables;

        // Scene's drawables selected by user.
        DrawablesList selectedDrawables;

    public:
        /***
         * 1. Drawables administration
         ***/
        void addDrawable( shared_ptr<DrawableEntity> drawable );
        void addCube( Cube* cube );


        /***
         * 2. Drawables selection.
         ***/
        void selectAll();

        /***
         * 3. Transformations
         ***/
        void translateSelectedDrawables( const GLfloat& tx, const GLfloat& ty, const GLfloat& tz );

        /***
         * 4. Drawing
         ***/
        void draw();
};

#endif // SCENE_HPP
