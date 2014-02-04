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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "public_user.hpp"


namespace como {

/***
 * 1. Construction
 ***/

PublicUser::PublicUser( UserID id, const char* name,
            std::shared_ptr< boost::asio::io_service > io_service,
            Socket socket,
            ProcessSceneUpdateCallback processSceneUpdateCallback,
            std::function<void (UserID)> removeUserCallback,
            CommandsHistoricPtr commandsHistoric,
            LogPtr log ) :
    BasicUser( id, name ),
    io_service_( io_service ),
    socket_( SocketPtr( new Socket( std::move( socket ) ) ) ),
    processSceneUpdateCallback_( processSceneUpdateCallback ),
    removeUserCallback_( removeUserCallback ),
    nextCommand_( 0 ),
    synchronizing_( false ),
    commandsHistoric_( commandsHistoric ),
    log_( log ),
    updateRequested_( false )
{
    selectionResponse_ = SelectionResponsePtr( new SelectionResponse );

    readSceneUpdate();

    //sync();

    requestUpdate();
}


/***
 * 3. User updating
 ***/

void PublicUser::requestUpdate()
{
    mutex_.lock();

    if( !updateRequested_ && needsSceneUpdate() ){
        updateRequested_ = true;
        io_service_->post( std::bind( &PublicUser::sendNextSceneUpdate, this ) );
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
 * 4. Socket reading
 ***/

void PublicUser::readSceneUpdate()
{
    mutex_.lock();
    sceneUpdatePacketFromUser_.clear();
    sceneUpdatePacketFromUser_.asyncRecv( socket_, boost::bind( &PublicUser::onReadSceneUpdate, this, _1, _2 ) );
    mutex_.unlock();
}


void PublicUser::onReadSceneUpdate( const boost::system::error_code& errorCode, PacketPtr packet )
{
    // Call to the processing callback in the server.
    processSceneUpdateCallback_( errorCode, getID(), std::dynamic_pointer_cast<const SceneUpdate>( packet ) );
}


/***
 * 5. Socket writing
 ***/

bool PublicUser::needsSceneUpdate() const
{
    bool res;

    mutex_.lock();
    res = ( nextCommand_ < commandsHistoric_->getSize() ) ||
            ( selectionResponse_->getNSelections() );
    mutex_.unlock();

    return res;
}


void PublicUser::sendNextSceneUpdate()
{
    mutex_.lock();

    updateRequested_ = false;

    // Create and prepare a SCENE_UPDATE packet.
    outSceneUpdatePacket_.clear();

    // If there is selection responses to be sent to the user, add it to the
    // scene update packet.
    if( selectionResponse_->getNSelections() ){
        outSceneUpdatePacket_.addCommand( selectionResponse_ );
    }

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
//std::dynamic_pointer_cast<const SceneUpdate>( packet )
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
        // Update the nextCommand_ index for the next SCENE_UPDATE packet.
        //nextCommand_ = outSceneUpdatePacket_.getLasCommandSent() + 1;

        selectionResponse_->clear();

        log_->debug( "SCENE_UPDATE sent - commands(",
                     dynamic_cast< const SceneUpdate* >( packet.get() )->getCommands()->size(),
                     ") - nextCommand_(",
                     (int)nextCommand_, ")\n" );

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


/***
 * 6. Selection responses
 ***/

void PublicUser::addSelectionResponse( bool selectionResponse )
{
    mutex_.lock();

    selectionResponse_->addSelectionConfirmation( selectionResponse );

    requestUpdate();

    mutex_.unlock();
}

} // namespace como
