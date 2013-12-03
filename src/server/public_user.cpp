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

PublicUser::PublicUser( UserID id, const char* name,
            std::shared_ptr< boost::asio::io_service > io_service,
            Socket socket,
            std::function<void (UserID)> removeUserCallback,
            std::function<void ()> broadcastCallback,
            CommandsHistoricPtr commandsHistoric,
            LogPtr log ) :
    BasicUser( id, name ),
    io_service_( io_service ),
    socket_( SocketPtr( new Socket( std::move( socket ) ) ) ),
    removeUserCallback_( removeUserCallback ),
    broadcastCallback_( broadcastCallback ),
    nextCommand_( 0 ),
    synchronizing_( false ),
    commandsHistoric_( commandsHistoric ),
    log_( log ),
    updateRequested_( false )
{
    log_->debug( "Session (", getID(), ") created\n" );

    readSceneUpdate();

    //sync();

    requestUpdate();
}


PublicUser::~PublicUser()
{
    log_->debug( "Session (", getID(), ") destroyed\n" );
}


/***
 * 2. User updating
 ***/

void PublicUser::requestUpdate()
{
    mutex_.lock();

    log_->debug( "PublicUser (", getID(), ") - Requesting update\n" );

    if( !updateRequested_ && needsSceneUpdate() ){
        updateRequested_ = true;
        io_service_->post( std::bind( &PublicUser::sendNextSceneUpdate, this ) );
        log_->debug( "PublicUser (", getID(), ") - Update requested\n" );
    }else{
        log_->debug( "PublicUser (", getID(), ") - Updated not needed (", updateRequested_, ", ", needsSceneUpdate(), "\n" );
    }

    mutex_.unlock();
}

/*
void PublicUser::update()
{
    mutex_.lock();
    if( !synchronizing_ && needsSceneUpdate() ){
        sendNextSceneUpdate();
    }
    mutex_.unlock();
}
*/


/***
 * 3. Socket reading
 ***/

void PublicUser::readSceneUpdate()
{
    mutex_.lock();
    log_->debug( "Waiting for SCENE_UPDATE from user (", getID(), ") - 1 ...\n"  );
    sceneUpdatePacketFromUser_.asyncRecv( socket_, boost::bind( &PublicUser::onReadSceneUpdate, this, _1, _2 ) );
    log_->debug( "Waiting for SCENE_UPDATE from user (", getID(), ") - 2 ...\n"  );
    mutex_.unlock();
}


void PublicUser::onReadSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet )
{
    unsigned int i;
    const std::vector< SceneCommandConstPtr >* commands = nullptr;

    mutex_.lock();
    if( errorCode ){
        log_->error( "ERROR reading SCENE_UPDATE packet: ", errorCode.message(), "\n" );
        removeUserCallback_( getID() );

        mutex_.unlock();
    }else{
        // FIXME: Make use of the packet.

        // Get the commands from the packet.
        commands = ( dynamic_cast< const SceneUpdate *>( packet.get() ) )->getCommands();

        log_->debug( "SCENE_UPDATE received with (", commands->size(), ") commands\n" );

        // Add the commands to the historic.
        for( i=0; i<commands->size(); i++ ){
            log_->debug( "SCENE_UPDATE received with (", commands->size(), ") commands - adding command to historic [", i, "] ...\n" );
            commandsHistoric_->addCommand( (*commands)[i] );
            log_->debug( "SCENE_UPDATE received with (", commands->size(), ") commands - adding command to historic [", i, "] ...OK\n" );
        }

        mutex_.unlock();

        //broadcastCallback_();
        readSceneUpdate();
    }
}


/***
 * 4. Socket writing
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

    updateRequested_ = false;

    // Create and prepare a SCENE_UPDATE packet.
    outSceneUpdatePacket_.clear();
    nextCommand_ = commandsHistoric_->fillSceneUpdatePacket( outSceneUpdatePacket_, nextCommand_, MAX_COMMANDS_PER_PACKET, getID() );
    log_->debug( "Sending scene update - nextCommand: (", (int)nextCommand_, ")\n" );

    //outSceneUpdatePacket_.addCommands( commandsHistoric, nextCommand_, MAX_COMMANDS_PER_PACKET );

    // Get the number of commands in the packet.
    nCommandsInLastPacket_ = (std::uint8_t)( outSceneUpdatePacket_.getCommands()->size() );

    if( nCommandsInLastPacket_ ){
        // Pack the previous packet and send it to the client.
        outSceneUpdatePacket_.asyncSend( socket_, boost::bind( &PublicUser::onWriteSceneUpdate, this, _1, _2 ) );

        // Signal that the user is being synchronized (permorming an async send).
        synchronizing_ = true;
    }

    mutex_.unlock();
}


void PublicUser::onWriteSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet )
{
    mutex_.lock();

    if( errorCode ){
        // FIXME: If there are an async read and an async write on the socket
        // at the same time, could it lead to errors (like deleting the user twice)?.
        log_->error( "ERROR writting SCENE_UPDATE packet: ", errorCode.message(), "\n" );
        removeUserCallback_( getID() );
    }else{
        // FIXME: Make use of the packet?.

        // Update the nextCommand_ index for the next SCENE_UPDATE packet.
        //nextCommand_ = outSceneUpdatePacket_.getLasCommandSent() + 1;

        log_->debug( "SCENE_UPDATE sended (nextCommand_: ", (int)nextCommand_, ")\n" );

        if( needsSceneUpdate() ){
            requestUpdate();
            //sendNextSceneUpdate();
        }else{
            updateRequested_ = false;
            synchronizing_ = false;
        }
    }

    mutex_.unlock();
}

} // namespace como
