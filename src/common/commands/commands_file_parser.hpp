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

#ifndef COMMANDS_FILE_PARSER_HPP
#define COMMANDS_FILE_PARSER_HPP

#include <fstream>
#include <common/commands/packable_commands_list.hpp> // for createEmtpyCommandFromBuffer() static method.

namespace como {

class CommandsFileParser
{
    public:
        /***
         * 1. Construction
         ***/
        CommandsFileParser( const std::string unpackingDirPath );
        CommandsFileParser() = delete;
        CommandsFileParser( const CommandsFileParser& ) = delete;
        CommandsFileParser( CommandsFileParser&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~CommandsFileParser() = default;


        /***
         * 3. File parsing
         ***/
        CommandPtr readNextCommand( std::ifstream& file );
        void writeCommand( const Command& command, std::ofstream& file );


        /***
         * 4. Operators
         ***/
        CommandsFileParser& operator = ( const CommandsFileParser& ) = delete;
        CommandsFileParser& operator = ( CommandsFileParser&& ) = delete;


    private:
        std::string unpackingDirPath_;
};

} // namespace como

#endif // COMMANDS_FILE_PARSER_HPP
