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

// TODO: Convert into a monitor.
class ClientUser : public BasicUser
{
    private:
        /*!
         * \brief Color used for highlighting drawables selected by this
         * user.
         */
        GLfloat selectionColor_[4];

    public:
        /***
         * 1. Construction
         ***/
        ClientUser( UserID id, std::string name, const std::uint8_t* selectionColor );
        ClientUser( const ClientUser& ) = delete;
        ClientUser( ClientUser&& ) = delete;
        //ClientUser( const UserConnectionCommand* userConnectedPacket );


        /***
         * 2. Destruction
         ***/
        ~ClientUser() = default;


        /***
         * 3. Getters
         ***/
        const GLfloat* getSelectionColor() const;


        /***
         * 4. Operators
         ***/
        ClientUser& operator=( const ClientUser& ) = delete ;
        ClientUser& operator=( ClientUser&& ) = delete;
};

typedef std::shared_ptr< ClientUser > PublicUserPtr;

} // namespace como

#endif // CLIENT_USER_HPP
