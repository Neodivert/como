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

#ifndef CAMERA_CREATION_COMMAND_HPP
#define CAMERA_CREATION_COMMAND_HPP

#include "camera_command.hpp"
#include <common/packables/packable_float.hpp>

namespace como {

class CameraCreationCommand : public CameraCommand
{
    public:
        /***
         * 1. Construction
         ***/
        CameraCreationCommand();
        CameraCreationCommand( ResourceID cameraID,
                               const glm::vec3 cameraCenter,
                               const glm::vec3 cameraEye,
                               const glm::vec3 cameraUp );
        CameraCreationCommand( const CameraCreationCommand& );
        CameraCreationCommand( CameraCreationCommand&& ) = delete;
        COMMAND_CLONE_METHOD( CameraCreationCommand )


        /***
         * 2. Destruction
         ***/
        ~CameraCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        glm::vec3 cameraEye() const;
        glm::vec3 cameraCenter() const;
        glm::vec3 cameraUp() const;


        /***
         * 4. Operators
         ***/
        CameraCreationCommand& operator = ( const CameraCreationCommand& ) = delete;
        CameraCreationCommand& operator = ( CameraCreationCommand&& ) = delete;


    private:
        PackableArray< PackableFloat, float, 3 > cameraEye_;
        PackableArray< PackableFloat, float, 3 > cameraCenter_;
        PackableArray< PackableFloat, float, 3 > cameraUp_;
};

} // namespace como

#endif // CAMERA_CREATION_COMMAND_HPP
