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

#ifndef PUBLIC_USER_HPP
#define PUBLIC_USER_HPP

#include "../3d/drawables_selection.hpp"
#include "../../../common/packets/scene_commands/scene_commands.hpp"

namespace como {

class PublicUser
{
    public:
        UserID id;
        char name[NAME_SIZE];
        GLfloat color[4];
        DrawablesSelection selection;

        /***
         * 1. Initialization and destruction
         ***/
        PublicUser( const UserConnected* userConnectedPacket );
};

} // namespace como

#endif // PUBLIC_USER_HPP
