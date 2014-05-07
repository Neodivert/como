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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP


/***
 * Includes
 ***/

#include <QWindow>
#include <client/managers/como_app.hpp>
#include <client/models/3d/camera.hpp>

namespace como {

// Available projections.
enum class Projection {
   ORTHO,
   PERSPECTIVE
};
const unsigned int N_PROJECTIONS = 2;


// Available projections (strings for GUI output).
const char projectionModeStrings[N_PROJECTIONS][16] =
{
    "Ortho",
    "Perspective"
};


/***
 * File main class
 ***/

class Viewport : public QWindow
{
    Q_OBJECT

    private:
        // Pointer to app's current state.
        shared_ptr< ComoApp > comoApp;

        // Camera associated with the OpenGL canvas.
        Camera* camera;

        // Location for shader uniform variables.
        static GLint viewProjectionMatrixLocation;

        // Window's Width and height inverses.
        // In order to make user transformations relatives to viewport's dimensions, we
        // multiply the transformation magnitude by the inverse of viewport's dimensions,
        // instead of making the division (more expensive).
        float widthInverse;
        float heightInverse;

        glm::vec2 lastMousePos;

        glm::mat4 projectionMatrix;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Viewport() = delete;
        Viewport( const Viewport& ) = delete;
        Viewport( Viewport&& ) = delete;
        Viewport( View view, shared_ptr< ComoApp > comoApp );

        ~Viewport();


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
        virtual void renderIfNeeded();
        virtual void render();
        void renderGuideRect();


        /***
         * 4. Operators
         ***/
        Viewport& operator=( const Viewport& ) = delete ;
        Viewport& operator=( Viewport&& ) = delete;


        /***
         * 5. Signals
         ***/
    signals:
        void viewIndexChanged( int viewIndex );

        /***
         * 6. Slots
         ***/
    public slots:
        void setView( View view );
        void setProjection( Projection projection );

        /***
         * 7. Auxiliar methods
         ***/
        glm::vec2 getNormalizedMousePos( const int& x, const int& y ) const ;
        void traceRay( const GLfloat& x, const GLfloat& y, glm::vec3& rayOrigin, glm::vec3& rayDirection ) const ;
        void updateTransformGuideLine( const GLfloat& x, const GLfloat& y );
};

} // namespace como

#endif // VIEWPORT_HPP
