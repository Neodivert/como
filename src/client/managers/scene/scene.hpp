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

#include <map>
#include <list>
#include "../../models/3d/cube.hpp"
#include "../../models/3d/camera.hpp"
#include "../../models/users/public_user.hpp"
#include "../../models/server/server_interface.hpp"
#include "../../../common/utilities/log.hpp"
#include "../../../common/packets/scene_commands/scene_commands.hpp"

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

// Available pivot point modes.
enum class PivotPointMode
{
    MEDIAN_POINT = 0,
    INDIVIDUAL_CENTROIDS,
    WORLD_ORIGIN
};
const unsigned int N_PIVOT_POINT_MODES = 3;

// Available pivot point modes (strings for GUI output).
const char pivotPointModeStrings[N_PIVOT_POINT_MODES][32] =
{
     "Median Point",
     "Individual Centroid",
     "World origin"
};

enum LinesBufferOffset {
    WORLD_AXIS = 0,
    GUIDE_AXIS,
    TRANSFORM_GUIDE_LINE,

    N_LINES_BUFFER_OFFSETS
};

typedef std::map< UserID, PublicUser > UsersMap;

class Scene : public QObject
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

    private:
        // Scene's non selected drawables.
        DrawablesSelection nonSelectedDrawables;

        // Lines VAO, VBO and offsets.
        GLuint linesVAO;
        GLuint linesVBO;
        GLuint linesBufferOffsets[N_LINES_BUFFER_OFFSETS];

        GLuint uniformColorLocation;
        glm::vec4 selectionCentroid;

        GLfloat defaultContourColor[4];


    public:
        /***
         * 1. Initialization and destruction
         ***/
        Scene( LogPtr log );
        ~Scene();

        virtual void connect( const char* host, const char* port, const char* userName ) = 0;

        void initLinesBuffer();


        /***
         * 2. Users administration
         ***/
        virtual void addUser( std::shared_ptr< const UserConnected > userConnectedCommand ) = 0;
        void removeUser( UserID userID );


        /***
         * 3. Getters and setters
         ***/
        void setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a );
        void setTransformGuideLine( glm::vec3 origin, glm::vec3 destiny );
        glm::vec3 getPivotPoint( const PivotPointMode& pivotPointMode );


        /***
         * 4. Drawables administration
         ***/
    private:
        void addDrawable( DrawablePtr drawable, DrawableID drawableID );
    public:
        void addCube( const std::uint8_t* color );
        void addCube( const std::uint8_t* color, DrawableID drawableID );

        //void addDrawable( DrawableType drawableType );
        //void addDrawable( DrawableType drawableType, DrawableID drawableID );


        /***
         * 5. Drawables selection.
         ***/
    public:
        void selectDrawable( DrawableID drawableID );
        void selectDrawable( DrawableID drawableID, UserID userID );
        //void selectAll();

        void unselectAll();
        void unselectAll( const unsigned int& userId );

        DrawableID selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection );

        glm::vec4 getSelectionCentroid() const ;


        /***
         * 6. Transformations
         ***/
        void translateSelection( const glm::vec3& direction );
        void translateSelection( const glm::vec3& direction, const unsigned int& userId );

        void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const PivotPointMode& pivotPointMode );
        void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const PivotPointMode& pivotPointMode, const unsigned int& userId );

        void scaleSelection( const glm::vec3& scaleFactors, const PivotPointMode& pivotPointMode );
        void scaleSelection( const glm::vec3& scaleFactors, const PivotPointMode& pivotPointMode, const unsigned int& userId );
        //void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot );

        void deleteSelection();
        void deleteSelection( const unsigned int& userId );

        void executeRemoteCommand( const SceneCommand* command );


        /***
         * 7. Updating
         ***/
        void updateSelectionCentroid( const unsigned int& userId );


        /***
         * 8. Drawing
         ***/
        void draw( const int& drawGuideRect = -1 ) const ;
        void drawWorldAxis() const ;
        void drawTransformGuideLine() const ;


        /***
         * 9. Signals
         ***/
    signals:
        void renderNeeded();
        void userConnected( UserConnectedConstPtr command );
        void userDisconnected( UserID userID );
};

typedef std::shared_ptr< Scene > ScenePtr;

} // namespace como

#endif // SCENE_HPP
