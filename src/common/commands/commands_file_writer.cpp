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

#include "commands_file_writer.hpp"
#include <boost/filesystem.hpp>
#include <common/packables/packable_packet_size.hpp>

namespace como {


/***
 * 1. Construction
 ***/

CommandsFileWriter::CommandsFileWriter( const std::string &filePath ) :
    file_( filePath, std::ios_base::trunc )
{
    if( boost::filesystem::exists( filePath ) ){
        throw std::runtime_error( "Commands file already exists - " + filePath );
    }

    file_.open( filePath.c_str() );
    if( !file_.is_open() ){
        throw FileNotOpenException( filePath );
    }
}


/***
 * 2. Destruction
 ***/

CommandsFileWriter::~CommandsFileWriter()
{
    file_.close();
}


/***
 * 3. File parsing
 ***/

void CommandsFileWriter::writeCommand( const Command &command )
{
    char commandSizeBuffer[8];
    const PackablePacketSize commandSize( command.getPacketSize() );
    char* commandBuffer;

    // Write command's size to file.
    commandSize.pack( commandSizeBuffer );
    file_.write( commandSizeBuffer, commandSize.getPacketSize() );

    // Write command to file.
    commandBuffer = new char[commandSize.getPacketSize()];
    command.pack( commandBuffer );
    file_.write( commandBuffer, commandSize.getPacketSize() );

    delete [] commandBuffer;
}

} // namespace como
