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

#include "scene_update_packet.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

SceneUpdatePacket::SceneUpdatePacket() :
    Packet( PacketType::SCENE_UPDATE ),
    lastCommandSent_( 0 ),
    nUnsyncCommands_( 0 )
{
    bodySize_ =
            sizeof( lastCommandSent_ ) +
            sizeof( nUnsyncCommands_ ) +
            sizeof( std::uint8_t ); // The number of commands in the packet is encoded as a uint8.
}


SceneUpdatePacket::SceneUpdatePacket( const SceneUpdatePacket& b ) :
    Packet( b ),
    lastCommandSent_( b.lastCommandSent_ ),
    nUnsyncCommands_( b.nUnsyncCommands_ ),
    commands_( b.commands_ )
{
}


Packet* SceneUpdatePacket::clone() const
{
    return new SceneUpdatePacket( *this );
}

/***
 * 2. Packing and unpacking
 ***/

char* SceneUpdatePacket::packBody( char* buffer ) const
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


const char* SceneUpdatePacket::unpackBody( const char* buffer )
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

    for( i=0; i<nCommands; i++ ){
        switch( SceneCommand::getType( buffer ) ){
            case SceneCommandType::USER_CONNECTION:
                sceneCommandPtr =  SceneCommandPtr( new UserConnectionCommand );
            break;
            case SceneCommandType::USER_DISCONNECTION:
                sceneCommandPtr =  SceneCommandPtr( new SceneCommand( SceneCommandType::USER_DISCONNECTION ) );
            break;
            case SceneCommandType::CUBE_CREATION:
                sceneCommandPtr =  SceneCommandPtr( new CubeCreationCommand );
            break;
            case SceneCommandType::DRAWABLE_SELECTION:
                sceneCommandPtr = SceneCommandPtr( new DrawableSelectionCommand );
            break;
            case SceneCommandType::SELECTION_RESPONSE:
                sceneCommandPtr =  SceneCommandPtr( new SelectionResponseCommand );
            break;
            case SceneCommandType::FULL_DESELECTION:
                sceneCommandPtr =  SceneCommandPtr( new SceneCommand( SceneCommandType::FULL_DESELECTION ) );
            break;
            case SceneCommandType::SELECTION_DELETION:
                sceneCommandPtr = SceneCommandPtr( new SceneCommand( SceneCommandType::SELECTION_DELETION ) );
            break;
            case SceneCommandType::SELECTION_TRANSFORMATION:
                sceneCommandPtr = SceneCommandPtr( new SelectionTransformationCommand );
            break;
            case SceneCommandType::PARAMETER_CHANGE:
                sceneCommandPtr = SceneCommandPtr( new ParameterChangeCommand );
            break;
            default:
                throw std::runtime_error( "ERROR: Unknown command found while unpackin SCENE_UPDATE packet" );
            break;
        }
        buffer = sceneCommandPtr->unpack( buffer );
        commands_.push_back( sceneCommandPtr );
    }

    // Return the updated buffer ptr.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint32_t SceneUpdatePacket::getLasCommandSent() const
{
    return lastCommandSent_;
}


std::uint32_t SceneUpdatePacket::getUnsyncCommands() const
{
    return nUnsyncCommands_;
}


const std::vector< SceneCommandConstPtr >* SceneUpdatePacket::getCommands() const
{
    return &commands_;
}


bool SceneUpdatePacket::expectedType() const
{
    return ( Packet::getType() == PacketType::SCENE_UPDATE );
}


/***
 * 4. Setters
 ***/

void SceneUpdatePacket::addCommand( SceneCommandConstPtr command )
{
    // Push back the given command.
    commands_.push_back( command );

    // Update the SCENE_UPDATE packet's header.
    bodySize_ += command->getPacketSize();
}


void SceneUpdatePacket::addCommand( SceneCommandConstPtr command,
                              const std::uint32_t& commandIndex,
                              const std::uint32_t& historicSize )
{
    // Push back the given command.
    commands_.push_back( command );

    // Update the SCENE_UPDATE packet's header.
    bodySize_ += command->getPacketSize();
    lastCommandSent_ = commandIndex;
    nUnsyncCommands_ = historicSize - (commandIndex + 1);
}

/*
void SceneUpdatePacket::addCommands(
                            const CommandsList* commandsHistoric,
                            const std::uint32_t& firstCommand,
                            const std::uint8_t& maxCommands )
{
    // Validity check: maxCommands can't be zero.
    if( !maxCommands ){
        throw std::runtime_error( std::string( "ERROR at SceneUpdatePacket::addCommands - maxCommands can't be zero" ) );
    }

    std::list< SceneCommandConstPtr >::const_iterator it = commandsHistoric->begin();
    std::advance( it, firstCommand );
    unsigned int i = 0;


    while( ( i < maxCommands ) && ( it != commandsHistoric->end() ) ){
        //std::cout << "Adding command [" << (firstCommand + i) << " (" << static_cast< int >( (*it)->getType() ) << ")] to SCENE_UPDATE packet" << std::endl;
        commands_.push_back( *it );
        bodySize_ += (*it)->getPacketSize();

        it++;
        i++;
    }

    // Update the SCENE_UPDATE packet's header.
    lastCommandSent_ = commandIndex;
    nUnsyncCommands_ = commandsHistoric->size() - (commandIndex + 1);
}
*/

void SceneUpdatePacket::clear()
{
    commands_.clear();
    bodySize_ =
            sizeof( lastCommandSent_ ) +
            sizeof( nUnsyncCommands_ ) +
            sizeof( std::uint8_t ); // The number of commands in the packet is encoded as a uint8.
}

} // namespace como
