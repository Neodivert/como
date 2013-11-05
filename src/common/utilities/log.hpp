/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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

class Log
{
    private:
        std::recursive_mutex mutex_;
        std::ostream& out_;

    public:
        Log() : out_( std::cout ) {}

        template< class T >
        void write( T value );

        template< class T, class... Args >
        void write( T value, Args... args );
};


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

    /*7
    while( *str ){
        if( *str == '%' ){
            if( *( str + 1 ) == '%' ){
                ++str;
            }else{
                out_ << value;
                write( str + 2, args... );
                mutex_.unlock();
                return;
            }
        }
        out_ << *str;
        str++;
    }
    mutex_.unlock();
    throw std::logic_error( "ERROR in Log::write - extra arguments provided" );
    */
}

typedef std::shared_ptr< Log > LogPtr;

} // namespace como

#endif // LOG_1_HPP
