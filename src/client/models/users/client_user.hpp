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

#ifndef CLIENT_USER_HPP
#define CLIENT_USER_HPP

#include "../../managers/drawables_selection/drawables_selection.hpp"
#include "../../../common/commands/commands.hpp"
#include "../../../common/users/basic_user.hpp"

namespace como {

class ClientUser : public BasicUser
{
    // TODO: Change to private and implement getters and setters.
    public:
        GLfloat color[4];
        DrawablesSelectionPtr selection;


        /***
         * 1. Initialization and destruction
         ***/
        ClientUser( UserID id, std::string name );
        ClientUser( const ClientUser& ) = delete;
        ClientUser( ClientUser&& ) = delete;
        ClientUser( const UserConnectionCommand* userConnectedPacket );

        ~ClientUser() = default;


        /***
         * 2. Operators
         ***/
        ClientUser& operator=( const ClientUser& ) = delete ;
        ClientUser& operator=( ClientUser&& ) = delete;
};

typedef std::shared_ptr< ClientUser > PublicUserPtr;

} // namespace como

#endif // CLIENT_USER_HPP
