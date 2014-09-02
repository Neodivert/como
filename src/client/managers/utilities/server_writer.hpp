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

#ifndef SERVER_WRITER_HPP
#define SERVER_WRITER_HPP

#include <client/managers/server_interface/server_interface.hpp>

namespace como {

class ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        ServerWriter( ServerInterfacePtr server );
        ServerWriter() = delete;
        ServerWriter( const ServerWriter& ) = delete;
        ServerWriter( ServerWriter&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~ServerWriter() = default;


        /***
         * 3. Operators
         ***/
        ServerWriter& operator = ( const ServerWriter& ) = delete;
        ServerWriter& operator = ( ServerWriter&& ) = delete;


    protected:
        /***
         * 4. Protected getters
         ***/
        UserID localUserID() const;
        ResourceID newResourceID();
        ServerInterfacePtr server() const; // TODO: Make this unnecessary.


        /***
         * 5. Server communication
         ***/
        void sendCommandToServer( CommandConstPtr command );


    private:
        ServerInterfacePtr server_;
};

} // namespace como

#endif // SERVER_WRITER_HPP
