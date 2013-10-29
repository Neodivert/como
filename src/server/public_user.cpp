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

PublicUser::PublicUser( unsigned int id, const char* name, Socket socket, std::function<void (unsigned int)> removeUserCallback ) :
    id_( id ),
    socket_( std::move( socket ) ),
    removeUserCallback_( removeUserCallback ),
    nextCommand_( 0 )
{
    strncpy( name_, name, 64 );
    std::cout << "Session created (id: " << id_ << ")" << std::endl;
    read();
}


PublicUser::~PublicUser()
{
    std::cout << "Session (" << id_ << ") destroyed" << std::endl;
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


void PublicUser::read()
{
    socket_.async_read_some( boost::asio::buffer( buffer_, BUFFER_SIZE ), boost::bind( &PublicUser::onRead, this, _1, _2 ) );
}


void PublicUser::onRead( const boost::system::error_code& errorCode, std::size_t length )
{
    if( errorCode ){
        if( errorCode == boost::asio::error::eof ){
            std::cout << "User disconnected" << std::endl;
        }else{
            std::cout << "onRead - ERROR: " << errorCode.message() << std::endl;
        }
        removeUserCallback_( id_ );
    }else{
        std::cout << "Client " << id_ << " : ";
        std::cout.write( buffer_, length );

        read();
    }
}


/***
 * 5. Socket writing
 ***/
/*
boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
*/

bool PublicUser::needsSceneUpdate( const CommandsList* commandsHistoric ) const
{
    return ( nextCommand_ < commandsHistoric->size() );
}


void PublicUser::sendNextSceneUpdate( const CommandsList* commandsHistoric )
{
    // Create and prepare a SCENE_UPDATE packet.
    SceneUpdate sceneUpdatePacket;
    sceneUpdatePacket.addCommands( commandsHistoric, nextCommand_, MAX_COMMANDS_PER_PACKET );

    // Get the number of commands in the packet.
    nCommandsInLastPacket_ = (std::uint8_t)( sceneUpdatePacket.getCommands()->size() );

    // Pack the previous packet and send it to the client.
    sceneUpdatePacket.pack( outBuffer_ );
    std::cout << "Sending " << (int)nCommandsInLastPacket_ << " commands to client [" << name_ << "]" << std::endl;
    boost::asio::async_write( socket_,
                              boost::asio::buffer( outBuffer_, sceneUpdatePacket.getPacketSize() ),
                              boost::bind( &PublicUser::onWrite, this, _1, _2 ) );
}


void PublicUser::onWrite( const boost::system::error_code& errorCode, std::size_t length )
{
    if( errorCode ){
        std::cout << "ERROR Writing to socket" << std::endl;
    }else{
        std::cout << "PERFECT! (" << length << ") bytes wrote to socket (" << (int)nCommandsInLastPacket_ << " commands in packet)" << std::endl;
        nextCommand_ += nCommandsInLastPacket_;
    }
}

} // namespace como
