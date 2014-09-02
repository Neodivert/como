/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef USER_HPP
#define USER_HPP

#include <common/ids/user_id.hpp>
#include <memory>

namespace como {

// TODO: Convert into a monitor.
class User : public std::enable_shared_from_this< User >
{
    private:
        // User's id and name.
        UserID id_;
        std::string name_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        User( UserID id, std::string name );
        User() = delete;
        User( const User& ) = default;
        User( User&& ) = default;

        virtual ~User() = default;


        /***
         * 2. Getters
         ***/
        UserID getID() const ;
        std::string getName() const ;


        /***
         * 3. Operators
         ***/
        User& operator = (const User& ) = delete;
        User& operator = ( User&& ) = delete;
};

typedef std::shared_ptr< User > UserPtr;

} // namespace como

#endif // USER_HPP
