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

/***
 * Code for the methods write() taken and adapted from this url:
 * http://en.wikipedia.org/wiki/Variadic_template
 ***/

#ifndef LOG_1_HPP
#define LOG_1_HPP

#include <thread>
#include <iostream>

namespace como {

enum class LogMessageType {
    DEBUG = 0,
    WARNING,
    ERROR
};


class Log
{
    private:
        std::recursive_mutex mutex_;
        std::ostream& out_;

    public:
        /***
         * 1. Initialization
         ***/
        Log() : out_( std::cout ) {}


        /***
         * 2. Mutex locking and unlocking
         ***/
        void lock(){ mutex_.lock(); }
        void unlock(){ mutex_.unlock(); }


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
};

typedef std::shared_ptr< Log > LogPtr;


/***
 * 3. Main writting methods
 ***/

template< class T>
void Log::write( T value )
{
    mutex_.lock();
    out_ << value;
    mutex_.unlock();
}


template< class T, class... Args >
void Log::write( T value, Args... args )
{
    mutex_.lock();
    out_ << value;
    write( args... );
    mutex_.unlock();
}


/***
 * 4. Writting methods (debug)
 ***/

template< class T>
void Log::debug( T value )
{
    write( "[DEBUG] ", value );
}


template< class T, class... Args >
void Log::debug( T value, Args... args )
{
    write( "[DEBUG] ", value, args... );
}


/***
 * 5. Writting methods (warnings)
 ***/

template< class T>
void Log::warning( T value )
{
    write( "[WARNING] ", value );
}


template< class T, class... Args >
void Log::warning( T value, Args... args )
{
    write( "[WARNING] ", value, args... );
}


/***
 * 6. Writting methods (errors)
 ***/

template< class T >
void Log::error( T value )
{
    write( "[ERROR] ", value );
}


template< class T, class... Args >
void Log::error( T value, Args... args )
{
    write( "[ERROR] ", value, args... );
}


} // namespace como

#endif // LOG_1_HPP