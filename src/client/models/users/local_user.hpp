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

#ifndef LOCAL_USER_HPP
#define LOCAL_USER_HPP

#include "client_user.hpp"

namespace como {

class LocalUser : public ClientUser
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        LocalUser() = delete;

        /*! \brief Copy constructor */
        LocalUser( const LocalUser& ) = delete;

        /*! \brief Move constructor */
        LocalUser( LocalUser&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~LocalUser() = default;


        /***
         * 3. Operators
         ***/

        /*! \brief Copy assignment operator */
        LocalUser& operator=( const LocalUser& ) = delete ;

        /*! \brief Move assignment operator */
        LocalUser& operator=( LocalUser&& ) = delete;
};

} // namespace como

#endif // LOCAL_USER_HPP
