/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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

#include "public_user.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

PublicUser::PublicUser( unsigned int id, const char* name, Socket socket, std::function<void (unsigned int)> removeUserCallback, LogPtr log ) :
    id_( id ),
    socket_( SocketPtr( new Socket( std::move( socket ) ) ) ),
    removeUserCallback_( removeUserCallback ),
    nextCommand_( 0 ),
    log_( log )
{
    strncpy( name_, name, 64 );
    log_->write( "Session (", id_, ") created\n" );

    readSceneUpdate();
}


PublicUser::~PublicUser()
{
    log_->write( "Session (", id_, ") destroyed\n" );
}


/***
 * 2. Getters
 ***/

unsigned int PublicUser::getId()
{
    return id_;
}


const char* PublicUser::getName()
{
    return name_;
}


/***
 * 3. Setters
 ***/



/***
 * 4. Socket reading
 ***/

void PublicUser::readSceneUpdate()
{
    log_->write( "Waiting for SCENE_UPDATE from user (", id_, ")\n"  );
    sceneUpdatePacketFromUser_.asyncRecv( socket_, boost::bind( &PublicUser::onReadSceneUpdate, this, _1, _2 ) );
}


void PublicUser::onReadSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet )
{
    if( errorCode ){
        log_->write( "ERROR reading SCENE_UPDATE packet: ", errorCode.message(), "\n" );
        removeUserCallback_( id_ );
    }else{
        // FIXME: Make use of the packet.
        log_->write( "SCENE_UPDATE received\n" );
        readSceneUpdate();
    }
}


/***
 * 5. Socket writing
 ***/

bool PublicUser::needsSceneUpdate( const CommandsList* commandsHistoric ) const
{
    return ( nextCommand_ < commandsHistoric->size() );
}


void PublicUser::sendNextSceneUpdate( const CommandsList* commandsHistoric )
{
    // Create and prepare a SCENE_UPDATE packet.
    outSceneUpdatePacket_.clear();
    outSceneUpdatePacket_.addCommands( commandsHistoric, nextCommand_, MAX_COMMANDS_PER_PACKET );

    // Get the number of commands in the packet.
    nCommandsInLastPacket_ = (std::uint8_t)( outSceneUpdatePacket_.getCommands()->size() );

    // Pack the previous packet and send it to the client.
    outSceneUpdatePacket_.asyncSend( socket_, boost::bind( &PublicUser::onWriteSceneUpdate, this, _1, _2 ) );
}


void PublicUser::onWriteSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet )
{
    if( errorCode ){
        // FIXME: If there are an async read and an async write on the socket
        // at the same time, could it lead to errors (like deleting the user twice)?.
        log_->write( "ERROR writting SCENE_UPDATE packet: ", errorCode.message(), "\n" );
        removeUserCallback_( id_ );
    }else{
        // FIXME: Make use of the packet?.

        // Update the nextCommand_ index for the next SCENE_UPDATE packet.
        nextCommand_ += nCommandsInLastPacket_;

        log_->write( "SCENE_UPDATE sended (nextCommand_: ", (int)nextCommand_, "\n" );
    }
}

} // namespace como
