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

//#include <client/models/3d/camera.hpp>
#include <common/commands/commands.hpp>
#include <map>
#include <list>
#include <common/utilities/log.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <queue>
#include <client/managers/managers/materials/materials_manager.hpp>
#include <common/users/user.hpp>
#include <common/utilities/paths.hpp>
#include <client/managers/managers/primitives/client_primitives_manager.hpp>
#include <common/scene/basic_scene.hpp>
#include <client/managers/managers/entities/entities_manager.hpp>
#include <client/managers/managers/users/users_manager.hpp>
#include <client/managers/managers/primitives/geometry_primitives_factory.hpp>
#include <client/managers/managers/textures/texture_walls_manager.hpp>
#include <client/models/3d/auxiliar_lines_renderer.hpp>
#include <memory> // std::shared_ptr
#define GL_GLEXT_PROTOTYPES
#include <QOpenGLContext>


namespace como {

class Scene : public QOffscreenSurface, public BasicScene, public Observer, public Observable
{
    Q_OBJECT

    private:
        // Users sharing this scene.
        UsersManagerPtr usersManager_;

        ClientPrimitivesManagerPtr primitivesManager_;

        MaterialsManagerPtr materialsManager_;

        TextureWallsManagerPtr textureWallsManager_;

        TexturesManagerPtr texturesManager_;

        EntitiesManagerPtr entitiesManager_;

        GeometricPrimitivesFactoryPtr geometricPrimitivesFactory_;

        OpenGLPtr openGL_; // TODO: Initialize this class outside and pass it to the constructor.

        std::shared_ptr< QOpenGLContext > oglContext_; // TODO: Initialize this inside of OpenGL constructor?

        // Interface with the server.
        ServerInterfacePtr server_;

        // Lines VAO, VBO and offsets. // TODO: Move to Viewport (x4) / RenderPanel class
        // // TODO: Move to Viewport (x4) / RenderPanel class
        std::unique_ptr< AuxiliarLinesRenderer > linesRenderer_;

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
        void initOpenGL(); // TODO: Make this private.
        void initManagers( const UserAcceptancePacket& userAcceptancePacket );


        /***
         * 5. Getters
         ***/
        std::shared_ptr< QOpenGLContext > getOpenGLContext() const ;
        UsersManagerPtr getUsersManager() const;
        MeshesManagerPtr getMeshesManager() const;
        MaterialsManagerPtr getMaterialsManager() const;
        LightsManagerPtr getLightsManager() const;
        ClientPrimitivesManagerPtr getPrimitivesManager() const;
        EntitiesManagerPtr getEntitiesManager() const;
        GeometricPrimitivesFactoryPtr getGeometricPrimitivesFactory() const;
        TextureWallsManager *getTextureWallsManager() const;
        TexturesManager* getTexturesManager() const;
        OpenGLPtr getOpenGL() const;
        AuxiliarLinesRenderer* linesRenderer() const;


        /***
         * 6. Setters
         ***/
    public:
        void setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a ) const;

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
        void draw( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const ;


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
        // TODO: Pass command by reference. By now it must be passed using
        // a std::shared_ptr because of Qt signal / slot mechanism.
        void executeRemoteCommand( std::shared_ptr< const Command > command );


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
