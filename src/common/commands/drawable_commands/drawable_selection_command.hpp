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

#ifndef DRAWABLE_SELECTION_COMMAND_HPP
#define DRAWABLE_SELECTION_COMMAND_HPP

#include "drawable_command.hpp"

namespace como {

class DrawableSelectionCommand : public DrawableCommand
{
    private:
        PackableUint8< std::uint8_t > addToSelection_;

    public:
        /***
         * 1. Construction
         ***/
        DrawableSelectionCommand();
        DrawableSelectionCommand( UserID userID, PackableDrawableID drawableID, bool addToSelection );
        DrawableSelectionCommand( const DrawableSelectionCommand& b );
        DrawableSelectionCommand( DrawableSelectionCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DrawableSelectionCommand() = default;


        /***
         * 3. Getters
         ***/
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
