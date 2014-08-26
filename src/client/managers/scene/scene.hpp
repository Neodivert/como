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

#ifndef SCENE_HPP
#define SCENE_HPP

#include <map>
#include <list>
#include <client/models/3d/camera.hpp>
#include <common/utilities/log.hpp>
#include <common/commands/commands.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <queue>
#include <client/models/3d/lights/lights.hpp>
#include <client/managers/materials/materials_manager.hpp>
#include <common/users/user.hpp>
#include <common/utilities/paths.hpp>
#include <client/managers/primitives/client_primitives_manager.hpp>
#include <common/scene/basic_scene.hpp>
#include <client/managers/entities_manager.hpp>
#include <client/managers/users/users_manager.hpp>

Q_DECLARE_METATYPE( como::CommandConstPtr )

namespace como {

enum LinesBufferOffset {
    WORLD_AXIS = 0,
    GUIDE_AXIS,
    TRANSFORM_GUIDE_LINE,

    N_LINES_BUFFER_OFFSETS
};

typedef std::map< UserID, UserPtr > UsersMap;

class Scene : public QOffscreenSurface, public BasicScene, public Observer, public Observable
{
    Q_OBJECT

    private:
        // Users sharing this scene.
        UsersManagerPtr usersManager_;

        ClientPrimitivesManagerPtr primitivesManager_;

        MaterialsManagerPtr materialsManager_;

        EntitiesManagerPtr entitiesManager_;

        OpenGLPtr openGL_;

        shared_ptr< QOpenGLContext > oglContext_;

        // Interface with the server.
        ServerInterfacePtr server_;

        // Lines VAO, VBO and offsets. // TODO: Move to Viewport (x4) / RenderPanel class?
        GLuint linesVAO;
        GLuint linesVBO;
        GLuint linesBufferOffsets[N_LINES_BUFFER_OFFSETS];

        GLint uniformColorLocation;

        UserConnectionCommandConstPtr localUserConnectionCommand_;

    public:
        /***
         * 1. Construction.
         ***/
        Scene() = delete;
        Scene( const Scene&  ) = delete;
        Scene( Scene&& ) = delete;
        Scene( const char* host, const char* port, const char* userName, LogPtr log );


        /***
         * 2. Destruction
         ***/
        virtual ~Scene();


        /***
         * 3. Initialization
         ***/

        void initOpenGL();
        void initLinesBuffer();
        void initManagers( const UserAcceptancePacket& userAcceptancePacket );


        /***
         * 4. Users administration
         ***/
        void addUser( std::shared_ptr< const UserConnectionCommand > userConnectedCommand );
        void removeUser( UserID userID );


        /***
         * 5. Getters
         ***/
        shared_ptr< QOpenGLContext > getOpenGLContext() const ;
        UsersManagerPtr getUsersManager() const;
        MeshesManagerPtr getMeshesManager() const;
        MaterialsManagerPtr getMaterialsManager() const;
        LightsManagerPtr getLightsManager() const;
        ClientPrimitivesManagerPtr getPrimitivesManager() const;
        EntitiesManagerPtr getEntitiesManager() const;
        OpenGLPtr getOpenGL() const;


        /***
         * 6. Setters
         ***/
    public:
        void setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a );
        void setTransformGuideLine( glm::vec3 origin, glm::vec3 destiny );

    protected:
        void createSceneDirectory();


        /***
         * 7. Drawables administration
         ***/
    private:
        void takeOpenGLContext();
    public:

        /***
         * 10. Drawing
         ***/
        void draw( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const int& drawGuideRect = -1 ) const ;
        void drawWorldAxis() const ;
        void drawTransformGuideLine() const ;


        /***
         * 11. Operators
         ***/
        Scene& operator=( const Scene& ) = delete ;
        Scene& operator=( Scene&& ) = delete;


        /***
         * 12. Signals
         ***/
        void emitRenderNeeded();
    signals:
        void renderNeeded();
        void userConnected( UserConnectionCommandConstPtr command );
        void userDisconnected( UserID userID );
        void primitiveAdded( QString primitiveName, ResourceID primitiveID );


        /***
         * 13. Slots
         ***/
    public slots:
        void executeRemoteCommand( CommandConstPtr command );

        /***
         * 14. Auxiliar methods
         ***/
    public:
        void run();

        /***
         * 15. Updating (Observer pattern)
         ***/
        virtual void update();
};

typedef std::shared_ptr< Scene > ScenePtr;

} // namespace como

#endif // SCENE_HPP
