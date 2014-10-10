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

#ifndef COMMANDS_FILE_WRITER_HPP
#define COMMANDS_FILE_WRITER_HPP

#include <fstream>
#include <common/commands/command.hpp>
#include <common/exceptions/file_not_open_exception.hpp>

namespace como {

class CommandsFileWriter
{
    public:
        /***
         * 1. Construction
         ***/
        CommandsFileWriter( const std::string& filePath );
        CommandsFileWriter() = delete;
        CommandsFileWriter( const CommandsFileWriter& ) = delete;
        CommandsFileWriter( CommandsFileWriter&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~CommandsFileWriter();


        /***
         * 3. File parsing
         ***/
        void writeCommand( const Command& command );


        /***
         * 4. Operators
         ***/
        CommandsFileWriter& operator = ( const CommandsFileWriter& ) = delete;
        CommandsFileWriter& operator = ( CommandsFileWriter&& ) = delete;


    private:
        std::ofstream file_;
};

} // namespace como

#endif // COMMANDS_FILE_WRITER_HPP
