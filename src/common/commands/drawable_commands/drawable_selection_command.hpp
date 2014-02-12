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

#ifndef DRAWABLE_SELECTION_COMMAND_HPP
#define DRAWABLE_SELECTION_COMMAND_HPP

#include "drawable_command.hpp"

namespace como {

class DrawableSelectionCommand : public DrawableCommand
{
    private:
        std::uint8_t addToSelection_;

    public:
        /***
         * 1. Initialization and destruction.
         ***/
        DrawableSelectionCommand();
        DrawableSelectionCommand( UserID userID, DrawableID drawableID, bool addToSelection );
        DrawableSelectionCommand( const DrawableSelectionCommand& b );
        DrawableSelectionCommand( DrawableSelectionCommand&& ) = delete;

        ~DrawableSelectionCommand() = default;


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        std::uint8_t getAddToSelection() const ;


        /***
         * 4. Setters
         ***/
        void setAddToSelection( std::uint8_t addToSelection );


        /***
         * 5. Operators
         ***/
        DrawableSelectionCommand& operator=( const DrawableSelectionCommand& ) = delete;
        DrawableSelectionCommand& operator=( DrawableSelectionCommand&& ) = delete;
};

} // namespace como

#endif // DRAWABLE_SELECTION_COMMAND_HPP
