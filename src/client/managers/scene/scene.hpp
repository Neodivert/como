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

#include <common/commands/commands.hpp>
#include <map>
#include <list>
#include <common/utilities/log.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <queue>
#include <client/managers/managers/materials/materials_manager.hpp>
#include <common/users/user.hpp>
#include <client/managers/managers/primitives/client_primitives_manager.hpp>
#include <common/scene/basic_scene.hpp>
#include <client/managers/managers/entities/entities_manager.hpp>
#include <client/managers/managers/users/users_manager.hpp>
#include <client/managers/managers/primitives/system_primitives_factory.hpp>
#include <client/managers/managers/textures/texture_walls_manager.hpp>
#include <client/models/3d/auxiliar_lines_renderer.hpp>
#include <memory> // std::shared_ptr
#define GL_GLEXT_PROTOTYPES
#include <QOpenGLContext>


namespace como {

class Scene : public QOffscreenSurface, public BasicScene, public Observer, public Observable
{
    Q_OBJECT

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
         * 3. Getters
         ***/
        std::shared_ptr< QOpenGLContext > getOpenGLContext() const ;
        UsersManagerPtr getUsersManager() const;
        MeshesManagerPtr getMeshesManager() const;
        MaterialsManagerPtr getMaterialsManager() const;
        LightsManagerPtr getLightsManager() const;
        ClientPrimitivesManagerPtr getPrimitivesManager() const;
        EntitiesManagerPtr getEntitiesManager() const;
        SystemPrimitivesFactoryPtr getSystemPrimitivesFactory() const;
        TextureWallsManager *getTextureWallsManager() const;
        TexturesManager* getTexturesManager() const;
        OpenGLPtr getOpenGL() const;
        AuxiliarLinesRenderer* linesRenderer() const;


        /***
         * 4. Setters
         ***/
        void setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a ) const;


        /***
         * 5. Drawing
         ***/
        void draw( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const ;


        /***
         * 6. Main loop
         ***/
        void run();


        /***
         * 7. Updating (Observer pattern)
         ***/
        virtual void update();


        /***
         * 8. Operators
         ***/
        Scene& operator=( const Scene& ) = delete ;
        Scene& operator=( Scene&& ) = delete;

    public slots:
        /***
         * 9. Slots
         ***/
        // TODO: Pass command by reference. By now it must be passed using
        // a std::shared_ptr because of Qt signal / slot mechanism.
        void executeRemoteCommand( std::shared_ptr< const Command > command );

    private:
        /***
         * 10. Initialization
         ***/
        void initOpenGL();
        void initManagers( const UserAcceptancePacket& userAcceptancePacket );


    private:
        // Users and resources managers.
        UsersManagerPtr usersManager_;
        ClientPrimitivesManagerPtr primitivesManager_;
        MaterialsManagerPtr materialsManager_;
        TextureWallsManagerPtr textureWallsManager_;
        TexturesManagerPtr texturesManager_;
        EntitiesManagerPtr entitiesManager_;
        SystemPrimitivesFactoryPtr systemPrimitivesFactory_;

        // OpenGL support.
        OpenGLPtr openGL_; // TODO: Initialize this class outside and pass it to the constructor.
        std::shared_ptr< QOpenGLContext > oglContext_; // TODO: Initialize this inside of OpenGL constructor?

        // Interface with the server.
        ServerInterfacePtr server_;

        // Auxiliar class for rendering lines on the scene.
        std::unique_ptr< AuxiliarLinesRenderer > linesRenderer_;
};

typedef std::shared_ptr< Scene > ScenePtr;

} // namespace como

#endif // SCENE_HPP
