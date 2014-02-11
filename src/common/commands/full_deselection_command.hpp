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

#ifndef FULL_DESELECTION_COMMAND_HPP
#define FULL_DESELECTION_COMMAND_HPP

#include "scene_command.hpp"

namespace como {

class FullDeselectionCommand : public SceneCommand
{
    public:
        /***
         * 1. Construction
         ***/
        FullDeselectionCommand() = default;
        FullDeselectionCommand( const UserDisconnectionCommand& ) = default;
        FullDeselectionCommand( SceneCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~FullDeselectionCommand() = default;


        /***
         * 3. Getters
         ***/
        virtual SceneCommandType getType() const ;


        /***
         * 4. Operators
         ***/
        FullDeselectionCommand& operator=( const FullDeselectionCommand& ) = delete;
        FullDeselectionCommand& operator=( FullDeselectionCommand&& ) = delete;
};

} // namespace como

#endif // FULL_DESELECTION_COMMAND_HPP
