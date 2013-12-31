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

#ifndef DRAWABLE_COMMAND_HPP
#define DRAWABLE_COMMAND_HPP

#include "../scene_command.hpp"

namespace como {

class DrawableCommand : public SceneCommand
{
    private:
        DrawableID drawableID_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        DrawableCommand( SceneCommandType sceneCommandType );
        DrawableCommand( UserID userID, DrawableID drawableID, SceneCommandType sceneCommandType );
        DrawableCommand( const DrawableCommand& b );

        virtual ~DrawableCommand(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        //static SceneCommandType getType( const char* buffer );
        DrawableID getDrawableID() const ;


        /***
         * 4. Setters
         ***/
        void setDrawableID( const DrawableID& drawableID );
};

} // namespace como

#endif // DRAWABLE_COMMAND_HPP
