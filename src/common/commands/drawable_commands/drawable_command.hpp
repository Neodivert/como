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

#ifndef DRAWABLE_COMMAND_HPP
#define DRAWABLE_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class DrawableCommandType : std::uint8_t {
    DRAWABLE_SELECTION
};

typedef PackableUint8< DrawableCommandType > PackableDrawableCommandType;

class DrawableCommand : public TypeCommand< DrawableCommandType >
{
    private:
        PackableResourceID drawableID_;

    public:
        /***
         * 1. Construction
         ***/
        DrawableCommand() = delete;
        DrawableCommand( DrawableCommandType drawableCommandType, UserID userID, ResourceID drawableID );
        DrawableCommand( const DrawableCommand& b );
        DrawableCommand( DrawableCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DrawableCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getResourceID() const ;


        /***
         * 4. Setters
         ***/
        void setResourceID( const ResourceID& drawableID );


        /***
         * 5. Operators
         ***/
        DrawableCommand& operator=( const DrawableCommand& ) = delete;
        DrawableCommand& operator=( DrawableCommand&& ) = delete;
};

typedef std::shared_ptr< const DrawableCommand > DrawableCommandConstPtr;

} // namespace como

#endif // DRAWABLE_COMMAND_HPP
