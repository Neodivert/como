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

CommandsFileParser::CommandsFileParser( const std::string &filePath, const std::string unpackingDirPath ) :
    file_( filePath.c_str() ),
    unpackingDirPath_( unpackingDirPath )
{
    if( !file_.is_open() ){
        throw FileNotOpenException( filePath );
    }
}


/***
 * 2. Destruction
 ***/

CommandsFileParser::~CommandsFileParser()
{
    file_.close();
}


/***
 * 3. File parsing
 ***/

CommandPtr CommandsFileParser::readNextCommand()
{
    PackablePacketSize commandSize;
    CommandPtr command = nullptr;
    char commandSizeBuffer[8] = {0}; // TODO: use [PackablePacketSize::packetSize()]
    char* commandBuffer;

    if( file_.eof() ){
        return nullptr;
    }

    // Read the command's size from the file.
    file_.read( commandSizeBuffer, PackablePacketSize::packetSize() );
    if( !file_ ){
        return nullptr;
    }
    commandSize.unpack( commandSizeBuffer );

    // Read the command's packed data from file.
    commandBuffer = new char[commandSize.getPacketSize()];
    command = PackableCommandsList::createEmtpyCommandFromBuffer( commandBuffer, unpackingDirPath_ );
    file_.read( commandBuffer, commandSize.getPacketSize() );
    if( !file_ ){
        throw std::runtime_error(
                    std::string( "Command couldn't be read from file " ) +
                    std::string( "(only " ) +
                    std::to_string( file_.gcount() ) +
                    std::string( " from " ) +
                    std::to_string( commandSize.getValue() ) +
                    std::string( " bytes could be read" ) );
    }
    delete [] commandBuffer;

    return command;
}


} // namespace como
