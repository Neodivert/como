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

// NOTE / FIXME: Although this is a cameraS manager, it currently uses only one
// active and undeletable camera.
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
        const Camera* activeCamera() const;
        glm::mat4 activeCameraViewMatrix() const;


        /***
         * 4. Commands execution
         ***/
        void executeRemoteCommand( const CameraCommand& command );


        /***
         * 5. Operators
         ***/
        CamerasManager& operator = ( const CamerasManager& ) = delete;
        CamerasManager& operator = ( CamerasManager&& ) = delete;


    protected:
        /***
         * 6. Resources management
         ***/
        virtual void clearResourcesSelection( UserID currentOwner );


    private:
        /***
         * 7. Remote camera creation
         ***/
        void createCamera( const ResourceID& cameraID,
                           const glm::vec3& cameraCenter,
                           const glm::vec3& cameraEye,
                           const glm::vec3& cameraUp );


    private:
        OpenGL* openGL_;

        const Camera* activeCamera_;
};

} // namespace como

#endif // CAMERAS_MANAGER_HPP
