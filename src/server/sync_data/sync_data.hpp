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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SYNC_DATA_HPP
#define SYNC_DATA_HPP

#include <list>
#include <common/commands/command.hpp>

namespace como {

class SyncData
{
    public:
        /***
         * 1. Construction
         ***/
        SyncData( const Command* creationCommand );
        SyncData() = delete;
        SyncData( const SyncData& ) = default;
        SyncData( SyncData&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~SyncData() = default;


        /***
         * 3. Getters
         ***/
        CommandConstPtr getCreationCommand() const;
        virtual std::list< CommandConstPtr > generateUpdateCommands() const = 0;


        /***
         * 4. Updating
         ***/
        virtual void processCommand( const Command& command ) = 0;


        /***
         * 5. Operators
         ***/
        SyncData& operator = ( const SyncData& ) = default;
        SyncData& operator = ( SyncData&& ) = default;


    private:
        CommandConstPtr creationCommand_;
};

} // namespace como

#endif // SYNC_DATA_HPP
