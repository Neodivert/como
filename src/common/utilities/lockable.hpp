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

#ifndef MONITOR_HPP
#define MONITOR_HPP

#include <mutex>
#include <memory> // std::unique_ptr

// Yes, I know this is ugly, but I tried to make a method Lockable::lock()
// returning a std::unique_guard,
// a std::unique_ptr< std::unique_lock< std::recursive_mutex > >,
// a std::unique_ptr< std::lock_guard< std::recursive_mutex > > and it
// didn't work.
#define LOCK \
    std::lock_guard< std::mutex > lock( this->mutex_ );


namespace como {

class Lockable
{
    public:
        /***
         * 1. Construction
         ***/
        Lockable() = default;
        Lockable( const Lockable& ) = default;
        Lockable( Lockable&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~Lockable() = default;


        /***
         * 3. Operators
         ***/
        Lockable& operator = ( const Lockable& ) = default;
        Lockable& operator = ( Lockable&& ) = default;


    protected:
        mutable std::mutex mutex_;
};

} // namespace como

#endif // MONITOR_HPP
