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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef DRAWABLE_COMMAND_HPP
#define DRAWABLE_COMMAND_HPP

#include "../command.hpp"

namespace como {

enum class DrawableCommandType : std::uint8_t {
    CUBE_CREATION = 0,
    DRAWABLE_SELECTION
};

class DrawableCommand : public Command
{
    private:
        const DrawableCommandType commandType_;
        DrawableID drawableID_;

    public:
        /***
         * 1. Construction
         ***/
        DrawableCommand() = delete;
        DrawableCommand( DrawableCommandType drawableCommandType, UserID userID, DrawableID drawableID );
        DrawableCommand( const DrawableCommand& b );
        DrawableCommand( DrawableCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DrawableCommand() = default;


        /***
         * 3. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer );


        /***
         * 4. Getters
         ***/
        DrawableCommandType getType() const;
        virtual std::uint16_t getPacketSize() const ;
        DrawableID getDrawableID() const ;


        /***
         * 5. Buffer pre reading
         ***/
        static DrawableCommandType getType( const char* buffer );


        /***
         * 6. Setters
         ***/
        void setDrawableID( const DrawableID& drawableID );


        /***
         * 7. Operators
         ***/
        DrawableCommand& operator=( const DrawableCommand& ) = delete;
        DrawableCommand& operator=( DrawableCommand&& ) = delete;
};

typedef std::shared_ptr< const DrawableCommand > DrawableCommandConstPtr;

} // namespace como

#endif // DRAWABLE_COMMAND_HPP
