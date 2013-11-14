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

PublicUser::PublicUser( unsigned int id, const char* name, Socket socket,
            std::function<void (unsigned int)> removeUserCallback,
            CommandsHistoricConstPtr commandsHistoric,
            LogPtr log ) :
    id_( id ),
    socket_( SocketPtr( new Socket( std::move( socket ) ) ) ),
    removeUserCallback_( removeUserCallback ),
    nextCommand_( 0 ),
    synchronizing_( false ),
    commandsHistoric_( commandsHistoric ),
    log_( log )
{
    strncpy( name_, name, 64 );
    log_->debug( "Session (", id_, ") created\n" );

    readSceneUpdate();

    sync();
}


PublicUser::~PublicUser()
{
    log_->debug( "Session (", id_, ") destroyed\n" );
}


/***
 * 2. Getters
 ***/

unsigned int PublicUser::getId()
{
    unsigned int id;

    mutex_.lock();
    id = id_;
    mutex_.unlock();

    return id;
}


const char* PublicUser::getName()
{
    const char* name;

    mutex_.lock();
    name = name_;
    mutex_.unlock();

    return name;
}


/***
 * 3. Setters
 ***/


/***
 * 4.
 ***/

void PublicUser::sync()
{
    mutex_.lock();
    if( !synchronizing_ && needsSceneUpdate() ){
        sendNextSceneUpdate();
    }
    mutex_.unlock();
}


/***
 * 5. Socket reading
 ***/

void PublicUser::readSceneUpdate()
{
    mutex_.lock();
    log_->debug( "Waiting for SCENE_UPDATE from user (", id_, ")\n"  );
    sceneUpdatePacketFromUser_.asyncRecv( socket_, boost::bind( &PublicUser::onReadSceneUpdate, this, _1, _2 ) );
    mutex_.unlock();
}


void PublicUser::onReadSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet )
{
    mutex_.lock();
    if( errorCode ){
        log_->error( "ERROR reading SCENE_UPDATE packet: ", errorCode.message(), "\n" );
        removeUserCallback_( id_ );
    }else{
        // FIXME: Make use of the packet.
        log_->debug( "SCENE_UPDATE received\n" );
        readSceneUpdate();
    }
    mutex_.unlock();
}


/***
 * 6. Socket writing
 ***/

bool PublicUser::needsSceneUpdate() const
{
    bool res;

    mutex_.lock();
    res = ( nextCommand_ < commandsHistoric_->getSize() );
    mutex_.unlock();

    return res;
}


void PublicUser::sendNextSceneUpdate()
{
    mutex_.lock();

    // Create and prepare a SCENE_UPDATE packet.
    outSceneUpdatePacket_.clear();
    commandsHistoric_->fillSceneUpdatePacket( outSceneUpdatePacket_, nextCommand_, MAX_COMMANDS_PER_PACKET );

    //outSceneUpdatePacket_.addCommands( commandsHistoric, nextCommand_, MAX_COMMANDS_PER_PACKET );

    // Get the number of commands in the packet.
    nCommandsInLastPacket_ = (std::uint8_t)( outSceneUpdatePacket_.getCommands()->size() );

    // Pack the previous packet and send it to the client.
    outSceneUpdatePacket_.asyncSend( socket_, boost::bind( &PublicUser::onWriteSceneUpdate, this, _1, _2 ) );

    // Signal that the user is being synchronized (permorming an async send).
    synchronizing_ = true;

    mutex_.unlock();
}


void PublicUser::onWriteSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet )
{
    mutex_.lock();

    if( errorCode ){
        // FIXME: If there are an async read and an async write on the socket
        // at the same time, could it lead to errors (like deleting the user twice)?.
        log_->error( "ERROR writting SCENE_UPDATE packet: ", errorCode.message(), "\n" );
        removeUserCallback_( id_ );
    }else{
        // FIXME: Make use of the packet?.

        // Update the nextCommand_ index for the next SCENE_UPDATE packet.
        nextCommand_ += nCommandsInLastPacket_;

        log_->debug( "SCENE_UPDATE sended (nextCommand_: ", (int)nextCommand_, ")\n" );

        if( needsSceneUpdate() ){
            sendNextSceneUpdate();
        }else{
            synchronizing_ = false;
        }
    }

    mutex_.unlock();
}

} // namespace como
