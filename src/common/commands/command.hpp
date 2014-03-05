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

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "../packables/packables.hpp"
#include "../utilities/ids.hpp"
#include <memory>
#include <stdexcept>

namespace como {

enum class CommandTarget : std::uint8_t
{
    USER = 0,
    DRAWABLE,
    SELECTION
};
typedef PackableUint8<CommandTarget> PackableCommandTarget;


const char commandTargetStrings[][32]
{
    "USER",
    "DRAWABLE",
    "SELECTION"
};

class Command : public CompositePackable
{
    private:
        const PackableCommandTarget commandTarget_;
        PackableUserID userID_;


    public:
        /***
         * 1. Construction
         ***/
        Command() = delete;
        Command( CommandTarget commandTarget, UserID userID = 0 );
        Command( const Command& b );
        Command( Command&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~Command() = default;


        /***
         * 3. Getters
         ***/
        CommandTarget getTarget() const ;
        UserID getUserID() const ;


        /***
         * 4. Buffer pre reading
         ***/
        static CommandTarget getTarget( const void* buffer );


        /***
         * 5. Setters
         ***/
        void setUserID( const UserID& userID );


        /***
         * 6. Operators
         ***/
        CommandTarget& operator=( const CommandTarget& ) = delete;
        CommandTarget& operator=( CommandTarget&& ) = delete;
};

typedef std::shared_ptr< Command > CommandPtr;
typedef std::shared_ptr< const Command > CommandConstPtr;

} // namespace como

#endif // COMMAND_HPP
