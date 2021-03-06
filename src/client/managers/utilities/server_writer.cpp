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

#include "server_writer.hpp"

namespace como {

/***
 * 1. Construction
 ***/

ServerWriter::ServerWriter( ServerInterfacePtr server ) :
    server_( server )
{}


/***
 * 4. Protected getters
 ***/

UserID ServerWriter::localUserID() const
{
    return server_->getLocalUserID();
}


ResourceID ServerWriter::reserveResourceIDs( unsigned int nIDs )
{
    return server_->reserveResourceIDs( nIDs );
}


ServerInterfacePtr ServerWriter::server() const
{
    return server_;
}


/***
 * 5. Server communication
 ***/

void ServerWriter::sendCommandToServer( CommandConstPtr command )
{
    server_->sendCommand( std::move( command ) );
}

} // namespace como
