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
        virtual ~Lockable() = 0;


        /***
         * 3. Operators
         ***/
        Lockable& operator = ( const Lockable& ) = default;
        Lockable& operator = ( Lockable&& ) = default;


    protected:
        /***
         * 4. Locking / unlocking
         ***/
        void lock() const;
        void unlock() const;


    private:
        mutable std::recursive_mutex mutex_;
};

} // namespace como

#endif // MONITOR_HPP
