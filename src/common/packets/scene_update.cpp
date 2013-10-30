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

#include "scene_update.hpp"
#include <iostream>

namespace como {


/***
 * 1. Initialization and destruction
 ***/
SceneUpdate::SceneUpdate() :
    Packet( PacketType::SCENE_UPDATE ),
    lastCommandSent_( 0 ),
    nUnsyncCommands_( 0 )
{
    bodySize_ =
            sizeof( lastCommandSent_ ) +
            sizeof( nUnsyncCommands_ ) +
            sizeof( std::uint8_t ); // The number of commands in the packet is encoded as a uint8.
}


/***
 * 2. Packing and unpacking
 ***/

char* SceneUpdate::packBody( char* buffer ) const
{
    unsigned int i = 0;

    // Pack the packet's body.
    packer::pack( lastCommandSent_, buffer );
    packer::pack( nUnsyncCommands_, buffer );
    packer::pack( (std::uint8_t)( commands_.size() ), buffer );
    for( ; i<commands_.size(); i++ ){
        buffer = commands_[i]->pack( buffer );
    }

    // Return the updated buffer ptr.
    return buffer;
}


const char* SceneUpdate::unpackBody( const char* buffer )
{
    unsigned int i = 0;
    std::uint8_t nCommands = 0;
    SceneCommandPtr sceneCommandPtr;

    // Pack the packet's body.
    packer::unpack( lastCommandSent_, buffer );
    packer::unpack( nUnsyncCommands_, buffer );
    packer::unpack( nCommands, buffer );
    commands_.clear();
    commands_.reserve( nCommands );

    for( ; i<nCommands; i++ ){
        switch( SceneCommand::getType( buffer ) ){
            case SceneCommandType::USER_CONNECTED:
                sceneCommandPtr =  SceneCommandPtr( new UserConnected );
            break;
        }
        sceneCommandPtr->unpack( buffer );
        commands_.push_back( sceneCommandPtr );
    }

    // Return the updated buffer ptr.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint16_t SceneUpdate::getPacketSize() const
{
    unsigned int i = 0;
    std::uint16_t packetSize = 0;

    packetSize += sizeof( lastCommandSent_ );
    packetSize += sizeof( nUnsyncCommands_ );
    packetSize += sizeof( std::uint8_t ); // commands_.size() is packed as a std::uint8_t.

    for( i=0; i<commands_.size(); i++ )
    {
        packetSize += commands_[i]->getPacketSize();
    }

    return packetSize;
}


std::uint32_t SceneUpdate::getLasCommandSent() const
{
    return lastCommandSent_;
}


std::uint32_t SceneUpdate::getUnsyncCommands() const
{
    return nUnsyncCommands_;
}


const std::vector< SceneCommandConstPtr >* SceneUpdate::getCommands()
{
    return &commands_;
}


bool SceneUpdate::expectedType() const
{
    return ( Packet::getType() == PacketType::SCENE_UPDATE );
}


/***
 * 4. Setters
 ***/

void SceneUpdate::addCommands(
                            const CommandsList* commandsHistoric,
                            const std::uint32_t& firstCommand,
                            const std::uint8_t& maxCommands )
{
    // Validity check: maxCommands can't be zero.
    if( !maxCommands ){
        throw std::runtime_error( std::string( "ERROR at SceneUpdate::addCommands - maxCommands can't be zero" ) );
    }

    std::list< SceneCommandConstPtr >::const_iterator it = commandsHistoric->begin();
    std::advance( it, firstCommand );
    unsigned int i = 0;


    while( ( i < maxCommands ) && ( it != commandsHistoric->end() ) ){
        commands_.push_back( *it );
        bodySize_ += (*it)->getPacketSize();

        it++;
        i++;
    }

    // Update the SCENE_UPDATE packet's header.
    lastCommandSent_ = firstCommand + maxCommands - 1;
    nUnsyncCommands_ = commandsHistoric->size() - (lastCommandSent_ + 1);
}


void SceneUpdate::clear()
{
    commands_.clear();
    bodySize_ =
            sizeof( lastCommandSent_ ) +
            sizeof( nUnsyncCommands_ ) +
            sizeof( std::uint8_t ); // The number of commands in the packet is encoded as a uint8.
}

} // namespace como
