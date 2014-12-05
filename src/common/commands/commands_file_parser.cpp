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

#include "commands_file_parser.hpp"
#include <common/exceptions/file_not_open_exception.hpp>
#include <common/packables/packable_packet_size.hpp>

namespace como {

/***
 * 1. Construction
 ***/

CommandsFileParser::CommandsFileParser( const std::string unpackingDirPath ) :
    unpackingDirPath_( unpackingDirPath )
{}


/***
 * 3. File parsing
 ***/

CommandPtr CommandsFileParser::readNextCommand( std::ifstream& file )
{
    PackablePacketSize commandSize;
    CommandPtr command = nullptr;
    char commandSizeBuffer[8] = {0};
    std::vector<char> commandBuffer;

    if( file.eof() ){
        throw std::runtime_error( "FILE.EOF" );
        return nullptr;
    }

    // Read the command's size from the file.
    file.read( commandSizeBuffer, PackablePacketSize::packetSize() );
    if( !file ){
        return nullptr;
    }
    commandSize.unpack( commandSizeBuffer );

    // Read the command's packed data from file.
    commandBuffer.resize( commandSize.getValue() );
    file.read( commandBuffer.data(), commandSize.getValue() );
    command = PackableCommandsList::createEmtpyCommandFromBuffer( commandBuffer.data(), unpackingDirPath_ );
    if( !file ){
        throw std::runtime_error(
                    std::string( "Command couldn't be read from file " ) +
                    std::string( "(only " ) +
                    std::to_string( file.gcount() ) +
                    std::string( " from " ) +
                    std::to_string( commandSize.getValue() ) +
                    std::string( " bytes could be read" ) );
    }
    command->unpack( commandBuffer.data() );

    return command;
}


void CommandsFileParser::writeCommand( const Command &command, std::ofstream &file )
{
    char commandSizeBuffer[8];
    const PackablePacketSize commandSize( command.getPacketSize() );
    std::vector< char > commandBuffer;

    // Write command's size to file.
    commandSize.pack( commandSizeBuffer );
    file.write( commandSizeBuffer, commandSize.getPacketSize() );

    // Write command to file.
    commandBuffer.resize( commandSize.getValue() );
    command.pack( commandBuffer.data() );
    file.write( commandBuffer.data(), commandSize.getValue() );
}

} // namespace como
