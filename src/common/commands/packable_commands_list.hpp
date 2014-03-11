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

#ifndef PACKABLE_COMMANDS_LIST_HPP
#define PACKABLE_COMMANDS_LIST_HPP

#include "commands.hpp"
#include "../packables/packable.hpp"
#include <list>

namespace como {

typedef std::list< CommandConstPtr > CommandsList;

class PackableCommandsList : public Packable
{
    private:
        CommandsList commands_;

    public:
        /***
         * 1. Construction.
         ***/
        PackableCommandsList() = default;
        PackableCommandsList( const PackableCommandsList& b );
        PackableCommandsList( PackableCommandsList&& b ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~PackableCommandsList(){}


        /***
         * 3. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 4. Getters
         ***/
        virtual std::uint16_t getPacketSize() const;
        const CommandsList* getCommands() const;


        /***
         * 5. Commands management
         ***/
        void addCommand( CommandConstPtr command );
        void clear();


        /***
         * 6. Operators
         ***/
        PackableCommandsList& operator = (const PackableCommandsList& b) = delete;
        PackableCommandsList& operator = ( PackableCommandsList&& ) = delete;
};

}// namespace como

#endif // PACKABLE_COMMANDS_LIST_HPP
