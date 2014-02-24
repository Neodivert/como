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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "scene_update_packet.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

SceneUpdatePacket::SceneUpdatePacket() :
    Packet( PacketType::SCENE_UPDATE ),
    nUnsyncCommands_( 0 )
{
    bodySize_ =
            sizeof( nUnsyncCommands_ ) +
            sizeof( std::uint8_t ); // The number of commands in the packet is encoded as a uint8.
}


SceneUpdatePacket::SceneUpdatePacket( const SceneUpdatePacket& b ) :
    Packet( b ),
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
    CommandPtr commandPtr;

    // Unpack the packet's body.
    packer::unpack( nUnsyncCommands_, buffer );
    packer::unpack( nCommands, buffer );
    commands_.clear();
    commands_.reserve( nCommands );

    for( i=0; i<nCommands; i++ ){
        switch( Command::getTarget( buffer ) ){
            // User commands
            case CommandTarget::USER:
                switch( UserCommand::getType( buffer+1 ) ){ // TODO: buffer+1 is ugly.
                    case UserCommandType::USER_CONNECTION:
                        commandPtr = CommandPtr( new UserConnectionCommand );
                    break;
                    case UserCommandType::USER_DISCONNECTION:
                        commandPtr =  CommandPtr( new UserDisconnectionCommand );
                    break;
                    case UserCommandType::PARAMETER_CHANGE:
                        commandPtr = CommandPtr( new ParameterChangeCommand );
                    break;
                }
            break;

            // Drawable commands
            case CommandTarget::DRAWABLE:
                switch( DrawableCommand::getType( buffer+1 ) ){ // TODO: buffer+1 is ugly.
                    case DrawableCommandType::CUBE_CREATION:
                        commandPtr =  CommandPtr( new CubeCreationCommand );
                    break;
                    case DrawableCommandType::DRAWABLE_SELECTION:
                        commandPtr = CommandPtr( new DrawableSelectionCommand );
                    break;
                }
            break;

            // Selection commands
            case CommandTarget::SELECTION:
                switch( SelectionCommand::getType( buffer+1 ) ){ // TODO: buffer+1 is ugly.
                    case SelectionCommandType::SELECTION_RESPONSE:
                        commandPtr = CommandPtr( new SelectionResponseCommand );
                    break;
                    case SelectionCommandType::FULL_DESELECTION:
                        commandPtr =  CommandPtr( new FullDeselectionCommand );
                    break;
                    case SelectionCommandType::SELECTION_DELETION:
                        commandPtr = CommandPtr( new SelectionDeletionCommand );
                    break;
                    case SelectionCommandType::SELECTION_TRANSFORMATION:
                        commandPtr = CommandPtr( new SelectionTransformationCommand );
                    break;
                }
            break;
        }
        buffer = commandPtr->unpack( buffer );
        commands_.push_back( commandPtr );
    }

    // Return the updated buffer ptr.
    return buffer;
}


/***
 * 3. Getters
 ***/

std::uint32_t SceneUpdatePacket::getUnsyncCommands() const
{
    return nUnsyncCommands_;
}


const std::vector< CommandConstPtr >* SceneUpdatePacket::getCommands() const
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

void SceneUpdatePacket::addCommand( CommandConstPtr command )
{
    // Push back the given command.
    commands_.push_back( command );

    // Update the SCENE_UPDATE packet's header.
    bodySize_ += command->getPacketSize();
}


void SceneUpdatePacket::addCommand( CommandConstPtr command,
                              const std::uint32_t& commandIndex,
                              const std::uint32_t& historicSize )
{
    // Push back the given command.
    commands_.push_back( command );

    // Update the SCENE_UPDATE packet's header.
    bodySize_ += command->getPacketSize();
    nUnsyncCommands_ = historicSize - (commandIndex + 1);
}


void SceneUpdatePacket::clear()
{
    commands_.clear();
    bodySize_ =
            sizeof( nUnsyncCommands_ ) +
            sizeof( std::uint8_t ); // The number of commands in the packet is encoded as a uint8.
}

} // namespace como
