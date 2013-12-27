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

#include "public_user.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

PublicUser::PublicUser( const UserConnected* userConnectedPacket ) :
    BasicUser( userConnectedPacket->getUserID(), userConnectedPacket->getName() )
{
    unsigned int i = 0;

    // Retrieve user's selection color.
    for( i=0; i<4; i++ ){
        color[i] = ( userConnectedPacket->getSelectionColor() ) [i] / 255.0f;
    }
}

} // namespace como
