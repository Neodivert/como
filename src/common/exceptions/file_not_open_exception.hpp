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

#ifndef FILE_NOT_OPEN_EXCEPTION_HPP
#define FILE_NOT_OPEN_EXCEPTION_HPP

#include <stdexcept>
#include <iostream>

namespace como {

class FileNotOpenException : public std::runtime_error
{
    public:
        /***
         * 1. Construction
         ***/
        FileNotOpenException() = delete;
        FileNotOpenException( std::string filePath );
        FileNotOpenException( const FileNotOpenException& ) = default;
        FileNotOpenException( FileNotOpenException&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~FileNotOpenException() = default;


        /***
         * 3. Operators
         ***/
        FileNotOpenException& operator = ( const FileNotOpenException& ) = delete;
        FileNotOpenException& operator = ( FileNotOpenException&& ) = delete;
};

} // namespace como

#endif // FILE_NOT_OPEN_EXCEPTION_HPP
