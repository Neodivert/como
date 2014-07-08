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

#include "scene_update_packet.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

SceneUpdatePacket::SceneUpdatePacket( const std::string& unpackingDirPath ) :
    Packet( PacketType::SCENE_UPDATE ),
    nUnsyncCommands_( 0 ),
    commands_( unpackingDirPath )
{
    addPackable( &nUnsyncCommands_ );
    addPackable( &commands_ );
}


SceneUpdatePacket::SceneUpdatePacket( const SceneUpdatePacket& b ) :
    Packet( b ),
    nUnsyncCommands_( b.nUnsyncCommands_ ),
    commands_( b.commands_ )
{
    addPackable( &nUnsyncCommands_ );
    addPackable( &commands_ );
}


Packet* SceneUpdatePacket::clone() const
{
    return new SceneUpdatePacket( *this );
}


/***
 * 3. Getters
 ***/

std::uint32_t SceneUpdatePacket::getUnsyncCommands() const
{
    return nUnsyncCommands_.getValue();
}


const CommandsList* SceneUpdatePacket::getCommands() const
{
    return commands_.getCommands();
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
    commands_.addCommand( command );
}


void SceneUpdatePacket::addCommand( CommandConstPtr command,
                              const std::uint32_t& commandIndex,
                              const std::uint32_t& historicSize )
{
    // Push back the given command.
    commands_.addCommand( command );

    nUnsyncCommands_ = historicSize - (commandIndex + 1);
}


void SceneUpdatePacket::clear()
{
    commands_.clear();
}

} // namespace como
