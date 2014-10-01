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

#ifndef CAMERA_COMMAND_HPP
#define CAMERA_COMMAND_HPP

#include <common/commands/type_command.hpp>
#include <common/packables/array/packable_array.hpp>

namespace como {

enum class CameraCommandType : std::uint8_t {
    CAMERA_CREATION
};

class CameraCommand : public TypeCommand< CameraCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        CameraCommand( CameraCommandType commandType, ResourceID cameraID, UserID userID );
        CameraCommand() = delete;
        CameraCommand( const CameraCommand& );
        CameraCommand( CameraCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~CameraCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID cameraID() const;


        /***
         * 4. Operators
         ***/
        CameraCommand& operator = ( const CameraCommand& ) = delete;
        CameraCommand& operator = ( CameraCommand&& ) = delete;


    private:
        PackableResourceID cameraID_;
};

} // namespace como

#endif // CAMERA_COMMAND_HPP
