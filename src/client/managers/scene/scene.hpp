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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <map>
#include <list>
#include "../../models/3d/cube.hpp"
#include "../../models/3d/camera.hpp"
#include "../../models/users/public_user.hpp"
#include "../../../common/utilities/log.hpp"
#include "../../../common/packets/scene_commands/scene_commands.hpp"
#include "../server_interface/server_interface.hpp"
#include "../../models/utilities/msl/src/shader_loader.hpp"
#include "../../models/3d/drawables_selection.hpp"
#include <queue>

Q_DECLARE_METATYPE( como::SceneCommandConstPtr )

namespace como {

// Available drawable types.
enum class DrawableType
{
    CUBE = 0
};

const unsigned int N_DRAWABLE_TYPES = 1;

// Available drawable types (strings for GUI output).
const char drawableTypeStrings[N_DRAWABLE_TYPES][16] =
{
    "Cube"
};


enum LinesBufferOffset {
    WORLD_AXIS = 0,
    GUIDE_AXIS,
    TRANSFORM_GUIDE_LINE,

    N_LINES_BUFFER_OFFSETS
};

typedef std::map< UserID, PublicUser > UsersMap;

class Scene : public QOffscreenSurface
{
    Q_OBJECT

    protected:
        // Users sharing this scene.
        UsersMap users_;

        // Log
        LogPtr log_;

        // Interface with the server.
        ServerInterface server_;

        // Local user's ID.
        UserID localUserID_;
        DrawableIndex localUserNextDrawableIndex_;

        std::queue< DrawableID > localUserPendingSelections_;

        shared_ptr< QOpenGLContext > oglContext_;
    private:
        // Scene's non selected drawables.
        DrawablesSelection nonSelectedDrawables;

        // Lines VAO, VBO and offsets.
        GLuint linesVAO;
        GLuint linesVBO;
        GLuint linesBufferOffsets[N_LINES_BUFFER_OFFSETS];

        GLuint uniformColorLocation;
        GLfloat defaultContourColor[4];


    public:
        /***
         * 1. Initialization and destruction
         ***/
        Scene( LogPtr log );
        ~Scene();

        void initOpenGL();

        virtual void connect( const char* host, const char* port, const char* userName ) = 0;

        void initLinesBuffer();


        /***
         * 2. Users administration
         ***/
        virtual void addUser( std::shared_ptr< const UserConnected > userConnectedCommand ) = 0;
        void removeUser( UserID userID );


        /***
         * 3. Getters
         ***/
        glm::vec3 getPivotPoint() const ;
        glm::vec3 getPivotPoint( UserID userID ) const ;
        shared_ptr< QOpenGLContext > getOpenGLContext() const ;
    private:
        DrawablesSelection* getUserSelection();
        DrawablesSelection* getUserSelection( UserID userID );
        const DrawablesSelection* getUserSelection() const ;
        const DrawablesSelection* getUserSelection( UserID userID ) const ;


        /***
         * 4. Setters
         ***/
    public:
        void setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a );
        void setTransformGuideLine( glm::vec3 origin, glm::vec3 destiny );
        void setPivotPointMode( PivotPointMode pivotPointMode );
        void setPivotPointMode( PivotPointMode pivotPointMode, UserID userID );


        /***
         * 5. Drawables administration
         ***/
    private:
        void addDrawable( DrawablePtr drawable, DrawableID drawableID );
        void takeOpenGLContext();
    public:
        void addCube( QColor color );
        void addCube( const std::uint8_t* color );
        void addCube( const std::uint8_t* color, DrawableID drawableID );

        //void addDrawable( DrawableType drawableType );
        //void addDrawable( DrawableType drawableType, DrawableID drawableID );


        /***
         * 6. Drawables selection.
         ***/
    public:
        void selectDrawable( DrawableID drawableID );
        void selectDrawable( DrawableID drawableID, UserID userID );
        //void selectAll();

        void unselectAll();
        void unselectAll( const unsigned int& userId );

        DrawableID selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection );


        /***
         * 7. Transformations
         ***/
        void translateSelection( glm::vec3 direction );
        void translateSelection( glm::vec3 direction, UserID userID );

        void rotateSelection( GLfloat angle, glm::vec3 axis );
        void rotateSelection( GLfloat angle, glm::vec3 axis, UserID userID );

        void scaleSelection( glm::vec3 scaleFactors );
        void scaleSelection( glm::vec3 scaleFactors, UserID userID );
        //void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot );

        void deleteSelection();
        void deleteSelection( const unsigned int& userId );


        /***
         * 8. Drawing
         ***/
        void draw( const int& drawGuideRect = -1 ) const ;
        void drawWorldAxis() const ;
        void drawTransformGuideLine() const ;


        /***
         * 9. Signals
         ***/
        void emitRenderNeeded();
    signals:
        void renderNeeded();
        void userConnected( UserConnectedConstPtr command );
        void userDisconnected( UserID userID );


        /***
         * 10. Slots
         ***/
    public slots:
        void executeRemoteCommand( SceneCommandConstPtr command );


        /***
         * 11. Auxiliar methods
         ***/
    public:
        void roundTransformationMagnitude( float& vx, float& vy, float& vz );
        void roundTransformationMagnitude( float& angle, float& vx, float& vy, float& vz );
};

typedef std::shared_ptr< Scene > ScenePtr;

} // namespace como

#endif // SCENE_HPP