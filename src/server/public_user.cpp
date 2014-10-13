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

#include "public_user.hpp"


namespace como {

/***
 * 1. Construction
 ***/

PublicUser::PublicUser( UserID id, const char* name,
            std::shared_ptr< boost::asio::io_service > io_service,
            Socket socket,
            ProcessSceneUpdatePacketCallback processSceneUpdatePacketCallback,
            std::function<void (UserID)> removeUserCallback,
            CommandsHistoricPtr commandsHistoric,
            LogPtr log,
            std::uint32_t color,
            const std::string& unpackingDirPath ) :
    User( id, name ),
    io_service_( io_service ),
    socket_( std::move( socket ) ),
    processSceneUpdatePacketCallback_( processSceneUpdatePacketCallback ),
    removeUserCallback_( removeUserCallback ),
    nextCommand_( 0 ),
    sceneUpdatePacketFromUser_( unpackingDirPath ),
    outSceneUpdatePacketPacket_( unpackingDirPath ),
    synchronizing_( false ),
    commandsHistoric_( commandsHistoric ),
    log_( log ),
    updateRequested_( false ),
    color_( color )
{
    readSceneUpdatePacket();

    //sync();

    requestUpdate();
}


/***
 * 3. User updating
 ***/

void PublicUser::requestUpdate()
{
    lock();

    if( !updateRequested_ && needsSceneUpdatePacket() ){
        updateRequested_ = true;
        io_service_->post( std::bind( &PublicUser::sendNextSceneUpdatePacket, this ) );
    }
}

/*
void PublicUser::update()
{
    mutex_.lock();
    if( !synchronizing_ && needsSceneUpdatePacket() ){
        sendNextSceneUpdatePacket();
    }
    mutex_.unlock();
}
*/


/***
 * 4. Socket reading
 ***/

void PublicUser::readSceneUpdatePacket()
{
    lock();
    sceneUpdatePacketFromUser_.clear();
    sceneUpdatePacketFromUser_.asyncRecv( socket_, boost::bind( &PublicUser::onReadSceneUpdatePacket, this, _1, _2 ) );
}


void PublicUser::onReadSceneUpdatePacket( const boost::system::error_code& errorCode, PacketPtr packet )
{
    // Call to the processing callback in the server.
    processSceneUpdatePacketCallback_( errorCode, getID(), *( std::dynamic_pointer_cast<const SceneUpdatePacket>( packet ) ) );
}


/***
 * 5. Socket writing
 ***/

bool PublicUser::needsSceneUpdatePacket() const
{
    lock();
    return ( nextCommand_ < commandsHistoric_->getSize() ) ||
            ( pendingResponseCommands_.size() );
}


void PublicUser::sendNextSceneUpdatePacket()
{
    lock();

    updateRequested_ = false;

    // Create and prepare a SCENE_UPDATE packet.
    outSceneUpdatePacketPacket_.clear();

    // If there is any response command to be sent to the user, add it to the
    // scene update packet.
    if( pendingResponseCommands_.size() ){
        outSceneUpdatePacketPacket_.addCommand( std::move( pendingResponseCommands_.front() ) );
        pendingResponseCommands_.pop();
    }

    nextCommand_ = commandsHistoric_->fillSceneUpdatePacketPacket( outSceneUpdatePacketPacket_, nextCommand_, MAX_COMMANDS_PER_PACKET, getID() );
    log_->debug( "Sending scene update - nextCommand: (", (int)nextCommand_, ")\n" );

    //outSceneUpdatePacketPacket_.addCommands( commandsHistoric, nextCommand_, MAX_COMMANDS_PER_PACKET );

    // Get the number of commands in the packet.
    nCommandsInLastPacket_ = (std::uint8_t)( outSceneUpdatePacketPacket_.getCommands()->size() );

    if( nCommandsInLastPacket_ ){
        // Pack the previous packet and send it to the client.
        outSceneUpdatePacketPacket_.asyncSend( socket_, boost::bind( &PublicUser::onWriteSceneUpdatePacket, this, _1, _2 ) );

        // Signal that the user is being synchronized (permorming an async send).
        synchronizing_ = true;
    }
//std::dynamic_pointer_cast<const SceneUpdatePacket>( packet )
}


void PublicUser::onWriteSceneUpdatePacket( const boost::system::error_code& errorCode, PacketPtr packet )
{
    lock();

    if( errorCode ){
        // FIXME: If there are an async read and an async write on the socket
        // at the same time, could it lead to errors (like deleting the user twice)?.
        log_->error( "ERROR writting SCENE_UPDATE packet: ", errorCode.message(), "\n" );
        removeUserCallback_( getID() );
    }else{
        log_->debug( "SCENE_UPDATE sent - commands(",
                     dynamic_cast< const SceneUpdatePacket* >( packet.get() )->getCommands()->size(),
                     ") - nextCommand_(",
                     (int)nextCommand_, ")\n" );

        if( needsSceneUpdatePacket() ){
            requestUpdate();
            //sendNextSceneUpdatePacket();
        }else{
            updateRequested_ = false;
            synchronizing_ = false;
        }
    }
}


/***
 * 6. Selection responses
 ***/


void PublicUser::addResponseCommand( CommandConstPtr responseCommand)
{
    lock();
    pendingResponseCommands_.push( std::move( responseCommand ) );
    requestUpdate();
}


/***
 * 7. Getters
 ***/

std::uint32_t PublicUser::getColor()
{
    lock();
    return color_;
}

} // namespace como
