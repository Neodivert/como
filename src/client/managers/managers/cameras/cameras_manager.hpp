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

#ifndef CAMERAS_MANAGER_HPP
#define CAMERAS_MANAGER_HPP

#include <client/managers/managers/entities/specialized_entities_manager.hpp>
#include <client/managers/selections/cameras/local_cameras_selection.hpp>

namespace como {

class CamerasManager;
typedef std::unique_ptr< CamerasManager > CamerasManagerPtr;

class CamerasManager : public SpecializedEntitiesManager< Camera, CamerasSelection, LocalCamerasSelection >
{
    public:
        /***
         * 1. Construction
         ***/
        CamerasManager( OpenGL& openGL, ServerInterfacePtr server, LogPtr log );
        CamerasManager() = delete;
        CamerasManager( const CamerasManager& ) = delete;
        CamerasManager( CamerasManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~CamerasManager() = default;


        /***
         * 3. Getters
         ***/
        glm::mat4 activeCameraViewMatrix() const;


        /***
         * 4. Commands execution
         ***/
        void executeRemoteCommand( const CameraCommand& command );


        /***
         * 5. Shader communication
         ***/
        void sendActiveCameraToShader() const;


        /***
         * 6. Operators
         ***/
        CamerasManager& operator = ( const CamerasManager& ) = delete;
        CamerasManager& operator = ( CamerasManager&& ) = delete;


    protected:
        /***
         * 7. Resources management
         ***/
        virtual void clearResourcesSelection( UserID currentOwner );


    private:
        /***
         * 8. Remote camera creation
         ***/
        void createCamera( const ResourceID& cameraID,
                           const glm::vec3& cameraCenter,
                           const glm::vec3& cameraEye,
                           const glm::vec3& cameraUp );


        /****
         * 9. Private getters
         ***/
        CamerasSelection& activeCameraSelection() const;


    private:
        OpenGL* openGL_;

        ResourceID activeCameraID_;
};

} // namespace como

#endif // CAMERAS_MANAGER_HPP
