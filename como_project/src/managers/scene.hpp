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

#include "../models/3d/cube.hpp"
#include "../models/3d/camera.hpp"


namespace como {

typedef std::list< std::shared_ptr< Drawable > > DrawablesList;

class Scene : public QObject
{
    Q_OBJECT

    private:
        // Scene's drawables not selected by user.
        DrawablesList nonSelectedDrawables;

        // Scene's drawables selected by user.
        DrawablesList selectedDrawables;

    public:
        /***
         * 1. Drawables administration
         ***/
        void addDrawable( shared_ptr<Drawable> drawable );
        void addCube( Cube* cube );


        /***
         * 2. Drawables selection.
         ***/
    public:
        void selectDrawable( const unsigned int index );
        void selectAll();
        void unselectAll();

        int selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection = false );

        /***
         * 3. Transformations
         ***/
        void translateSelectedDrawables( const GLfloat& tx, const GLfloat& ty, const GLfloat& tz );

        /***
         * 4. Drawing
         ***/
        void draw( const glm::mat4& viewProjectionMatrix );

        /***
         * 5. Signals
         ***/
    signals:
        void renderNeeded();
};

} // namespace como

#endif // SCENE_HPP
