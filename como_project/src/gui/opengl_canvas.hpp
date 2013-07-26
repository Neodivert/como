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

#ifndef OPENGL_CANVAS_HPP
#define OPENGL_CANVAS_HPP


/***
 * Includes
 ***/

#include <QWindow>
#include "../managers/como_app.hpp"
#include "../models/3d/camera.hpp"


namespace como {

/***
 * File main class
 ***/

class OpenGLCanvas : public QWindow
{
    Q_OBJECT

    private:
        // Pointer to app's current state.
        shared_ptr< ComoApp > comoApp;

        // Camera associated with the OpenGL canvas.
        // TODO: Move cameras to comoApp (for better management and signaling)
        // and save and identifier here.
        Camera camera;

        // Window's Width and height inverses.
        // In order to make user transformations relatives to canvas' dimensions, we
        // multiply the transformation magnitude by the inverse of canvas' dimensions,
        // instead of making the division (more expensive).
        float widthInverse;
        float heightInverse;

        int lastMouseX;
        int lastMouseY;

    public:
        /***
         * 1. Initialization and destruction
         ***/

        //explicit OpenGLCanvas( QWidget *parent = 0);
        // add "explicit"?
        OpenGLCanvas( shared_ptr< ComoApp > comoApp );

        /***
         * 2. Events
         ***/
        virtual bool event( QEvent *event );
        virtual void exposeEvent( QExposeEvent* event );
        virtual void resizeEvent( QResizeEvent* event );
        virtual void mouseMoveEvent( QMouseEvent* mouseMoveEvent );
        virtual void mousePressEvent( QMouseEvent* mousePressEvent );
        void keyPressEvent( QKeyEvent *e );

        /***
         * 3. Updating and drawing
         ***/
        virtual void render();

    signals:
};

} // namespace como

#endif // OPENGL_CANVAS_HPP
