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

#ifndef BASIC_USER_HPP
#define BASIC_USER_HPP

#include "../utilities/ids.hpp"
#include <memory>

namespace como {

class BasicUser : public std::enable_shared_from_this< BasicUser >
{
    private:
        // User's id and name.
        UserID id_;
        char name_[64];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        BasicUser() = delete;
        BasicUser( const BasicUser& ) = delete;
        BasicUser( BasicUser&& ) = delete;
        BasicUser( UserID id, const char* name );

        ~BasicUser() = default;


        /***
         * 2. Getters
         ***/
        UserID getID() const ;
        const char* getName() const ;


        /***
         * 3. Operators
         ***/
        BasicUser& operator = (const BasicUser& ) = delete;
        BasicUser& operator = ( BasicUser&& ) = delete;
};

} // namespace como

#endif // BASIC_USER_HPP
