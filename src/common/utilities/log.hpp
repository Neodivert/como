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

/***
 * Code for the methods write() taken and adapted from this url:
 * http://en.wikipedia.org/wiki/Variadic_template
 ***/

#ifndef LOG_HPP
#define LOG_HPP

#include <common/utilities/lockable.hpp>
#include <iostream>
#include <memory> // std::shared_ptr

namespace como {

class Log;
typedef std::shared_ptr< Log > LogPtr;


class Log : public Lockable
{
    private:
        std::ostream& out_;

    public:
        /***
         * 1. Initialization
         ***/
        Log() : out_( std::cout ) {}
        Log( const Log& ) = delete;
        Log( Log&& ) = delete;


    private:
        /***
         * 3. Main writting methods
         ***/
        template< class T >
        void write( T value );

        template< class T, class... Args >
        void write( T value, Args... args );

    public:
        /***
         * 4. Writting methods (debug)
         ***/
        template< class T >
        void debug( T value );

        template< class T, class... Args >
        void debug( T value, Args... args );


        /***
         * 5. Writting methods (warnings)
         ***/
        template< class T >
        void warning( T value );

        template< class T, class... Args >
        void warning( T value, Args... args );


        /***
         * 6. Writting methods (errors)
         ***/
        template< class T >
        void error( T value );

        template< class T, class... Args >
        void error( T value, Args... args );


        /***
         * 6. Operators
         ***/
        Log& operator = (const Log& ) = delete;
        Log& operator = ( Log&& ) = delete;
};


/***
 * 3. Main writting methods
 ***/

template< class T>
void Log::write( T value )
{
    out_ << value;
    out_.flush();
}


template< class T, class... Args >
void Log::write( T value, Args... args )
{
    out_ << value;
    write( args... );
}


/***
 * 4. Writting methods (debug)
 ***/

template< class T>
void Log::debug( T value )
{
    LOCK
    write( "[DEBUG] ", value );
}


template< class T, class... Args >
void Log::debug( T value, Args... args )
{
    LOCK
    write( "[DEBUG] ", value, args... );
}


/***
 * 5. Writting methods (warnings)
 ***/

template< class T>
void Log::warning( T value )
{
    LOCK
    write( "[WARNING] ", value );
}


template< class T, class... Args >
void Log::warning( T value, Args... args )
{
    LOCK
    write( "[WARNING] ", value, args... );
}


/***
 * 6. Writting methods (errors)
 ***/

template< class T >
void Log::error( T value )
{
    LOCK
    write( "[ERROR] ", value );
}


template< class T, class... Args >
void Log::error( T value, Args... args )
{
    LOCK
    write( "[ERROR] ", value, args... );
}


} // namespace como

#endif // LOG_HPP
