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

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP


/***
 * Includes
 ***/

#include <QWindow>
#include "../managers/como_app.hpp"
#include "../models/3d/camera.hpp"

namespace como {

enum class Projection {
   ORTHO,
   PERSPECTIVE
};
const unsigned int N_PROJECTIONS = 2;

typedef std::array< Projection, N_PROJECTIONS > ProjectionModes;
extern ProjectionModes projectionModes;

typedef std::array< QString, N_PROJECTIONS > ProjectionModeStrings;
extern ProjectionModeStrings projectionModeStrings;


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
        Camera camera;

        // Location for shader uniform variables.
        static GLint viewProjectionMatrixLocation;

        //
        static GLuint guideRectVAO;
        static GLuint guideRectVBO;


        // Window's Width and height inverses.
        // In order to make user transformations relatives to viewport's dimensions, we
        // multiply the transformation magnitude by the inverse of viewport's dimensions,
        // instead of making the division (more expensive).
        float widthInverse;
        float heightInverse;

        glm::vec2 lastMousePos;

        glm::mat4 projectionMatrix;

        bool focus;

    public:
        /***
         * 1. Initialization and destruction
         ***/

        //explicit Viewport( QWidget *parent = 0);
        // add "explicit"?
        Viewport( shared_ptr< ComoApp > comoApp );
        ~Viewport();
        static void initGuideRect();

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
        void sendViewProjectionMatrixToShader( const glm::mat4& vpMatrix ) const ;
        virtual void render();
        void renderGuideRect();

        /***
         * 4. Signals
         ***/
    signals:
        void viewIndexChanged( int viewIndex );

        /***
         * 5. Slots
         ***/
    public slots:
        void setView( View view );
        void setProjection( Projection projection );

        /***
         * 6. Auxiliar methods
         ***/
        glm::vec2 getNormalizedMousePos( const int& x, const int& y ) const ;
        void traceRay( const GLfloat& x, const GLfloat& y, glm::vec3& rayOrigin, glm::vec3& rayDirection ) const ;
        void updateGuideRect( const GLfloat& x, const GLfloat& y );
};

} // namespace como

#endif // VIEWPORT_HPP
