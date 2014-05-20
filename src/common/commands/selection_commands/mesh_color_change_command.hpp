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

#ifndef MESH_COLOR_CHANGE_COMMAND_HPP
#define MESH_COLOR_CHANGE_COMMAND_HPP

#include "selection_command.hpp"

namespace como {

class MeshColorChangeCommand : public SelectionCommand
{
    private:
        PackableRGBA meshColor_;

    public:
        /***
         * 1. Construction
         ***/
        MeshColorChangeCommand();
        MeshColorChangeCommand( UserID userID, const std::uint8_t* color );
        MeshColorChangeCommand( const MeshColorChangeCommand& b ) = delete;
        MeshColorChangeCommand( MeshColorChangeCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MeshColorChangeCommand() = default;


        /***
         * 3. Getters
         ***/
        const std::uint8_t* getMeshColor() const;


        /***
         * 4. Operators
         ***/
        MeshColorChangeCommand& operator=( const MeshColorChangeCommand& ) = delete;
        MeshColorChangeCommand& operator=( MeshColorChangeCommand&& ) = delete;
};

} // namespace como

#endif // MESH_COLOR_CHANGE_COMMAND_HPP
