/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
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
#include <QMouseEvent>
#include <client/managers/como_app.hpp>
#include <client/models/3d/camera.hpp>
#include <client/managers/managers/cameras/cameras_manager.hpp>

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

        LocalEntitiesSelection* localEntitiesSelection_;

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

        glm::mat4 projectionMatrix;

        View view_;
        Projection projection_;

        bool forceRender_;

        glm::vec3 lastMouseWorldPos_;

        CamerasManager* camerasManager_;

    public:
        /***
         * 1. Construction
         ***/
        Viewport() = delete;
        Viewport( const Viewport& ) = delete;
        Viewport( Viewport&& ) = delete;
        Viewport( View view, Projection projection, shared_ptr< ComoApp > comoApp );


        /***
         * 2. Destruction
         ***/
        ~Viewport();


        /***
         * 3. Events
         ***/
        virtual bool event( QEvent *event );
        virtual void exposeEvent( QExposeEvent* event );
        virtual void resizeEvent( QResizeEvent* event );
        virtual void mouseMoveEvent( QMouseEvent* mouseMoveEvent );
        virtual void mousePressEvent( QMouseEvent* mousePressEvent );
        virtual void wheelEvent( QWheelEvent* ev );
        void keyPressEvent( QKeyEvent *e );


        /***
         * 4. Getters
         ***/
        View getView() const;
        Projection getProjection() const;


        /***
         * 5. Updating and drawing
         ***/
        virtual void renderIfNeeded();
        virtual void render();
        void renderGuideRect();


        /***
         * 6. Signals
         ***/
    signals:
        void viewIndexChanged( int viewIndex );


        /***
         * 7. Slots
         ***/
    public slots:
        void setView( View view );
        void setProjection( Projection projection );
        void forceRender();


        /***
         * 8. Auxiliar methods
         ***/
    private:
        glm::vec2 getNormalizedMousePos( const int& x, const int& y ) const ;
        void traceRay( const GLfloat& x, const GLfloat& y, glm::vec3& rayOrigin, glm::vec3& rayDirection ) const ;
        void updateTransformGuideLine( const GLfloat& x, const GLfloat& y );


        /***
         * 9. Operators
         ***/
    public:
        Viewport& operator=( const Viewport& ) = delete ;
        Viewport& operator=( Viewport&& ) = delete;
};

} // namespace como

#endif // VIEWPORT_HPP
