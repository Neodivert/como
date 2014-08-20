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

#ifndef COLOURED_USER_HPP
#define COLOURED_USER_HPP

#include <common/users/user.hpp>
#include <common/commands/user_commands/user_connection_command.hpp>

namespace como {

class ColouredUser : public User
{
    public:
        /***
         * 1. Construction
         ***/
        ColouredUser( const UserConnectionCommand& command );
        ColouredUser() = delete;
        ColouredUser( const ColouredUser& ) = default;
        ColouredUser( ColouredUser&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~ColouredUser() = default;


        /***
         * 3. Getters
         ***/
        glm::vec4 color() const;


        /***
         * 4. Operators
         ***/
        ColouredUser& operator = ( const ColouredUser& ) = delete;
        ColouredUser& operator = ( ColouredUser&& ) = delete;


    private:
        glm::vec4 color_;
};

} // namespace como

#endif // COLOURED_USER_HPP
