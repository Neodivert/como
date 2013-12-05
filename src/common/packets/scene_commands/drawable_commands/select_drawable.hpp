/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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

#ifndef SELECT_DRAWABLE_HPP
#define SELECT_DRAWABLE_HPP

#include "drawable_command.hpp"

namespace como {

class SelectDrawable : public DrawableCommand
{
    private:
        std::uint8_t addToSelection_;

    public:
        /***
         * 1. Initialization and destruction.
         ***/
        SelectDrawable( UserID userID, DrawableID drawableID, bool addToSelection );
        SelectDrawable( const SelectDrawable& b );

        virtual ~SelectDrawable(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        virtual SceneCommandType getType() const ;
        std::uint8_t getAddToSelection() const ;


        /***
         * 4. Setters
         ***/
        void setAddToSelection( std::uint8_t addToSelection );
};

} // namespace como

#endif // SELECT_DRAWABLE_HPP
