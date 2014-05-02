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

#ifndef REMOTE_USER_HPP
#define REMOTE_USER_HPP

#include "client_user.hpp"
#include "../../../common/commands/user_commands/user_connection_command.hpp"
#include "../../managers/drawables_selection/drawables_selection.hpp"

namespace como {

// TODO: Convert into a monitor.
class RemoteUser : public ClientUser
{
    private:
        /*! Drawables selection owned by this user. */
        DrawablesSelection drawablesSelection_;

    public:
        /***
         * 1. Construction
         ***/

        /*!
         * \brief Constructor. Constructs a new RemoteUser from a
         * UserConnection command.
         */
        RemoteUser( const UserConnectionCommand& userConnectionCommand );

        /*! \brief Default constructor */
        RemoteUser() = delete;

        /*! \brief Copy constructor */
        RemoteUser( const RemoteUser& ) = delete;

        /*! \brief Move constructor */
        RemoteUser( RemoteUser&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~RemoteUser() = default;


        /***
         * 3. Getters
         ***/

        DrawablesSelection const * getDrawablesSelection() const ;


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        RemoteUser& operator=( const RemoteUser& ) = delete ;

        /*! \brief Move assignment operator */
        RemoteUser& operator=( RemoteUser&& ) = delete;
};

} // namespace como

#endif // REMOTE_USER_HPP
