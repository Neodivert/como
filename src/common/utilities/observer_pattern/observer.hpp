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

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <memory>

namespace como {

class Observer
{
    public:
        /***
         * 1. Construction
         ***/
        Observer() = default;
        Observer( const Observer& ) = default;
        Observer( Observer&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~Observer() = default;


        /***
         * 3. Updating
         ***/
        virtual void update() = 0;


        /***
         * 4. Operators
         ***/
        Observer& operator = ( const Observer& ) = default;
        Observer& operator = ( Observer&& ) = default;
};

typedef std::shared_ptr< Observer > ObserverPtr;
typedef std::shared_ptr< const Observer > ObserverConstPtr;

}

#endif // OBSERVER_HPP
