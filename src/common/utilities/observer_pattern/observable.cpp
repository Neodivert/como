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

#include "observable.hpp"

namespace como {

/***
 * 3. Observers management
 ***/

bool Observable::addObserver( ObserverPtr observer )
{
    std::pair< std::unordered_set< ObserverPtr >::iterator, bool > res = observers_.insert( observer );

    return res.second;
}


void Observable::removeObserver( ObserverPtr observerToBeRemoved )
{
    observers_.erase( observerToBeRemoved );
}


/***
 * 4. Observers notification
 ***/

void Observable::notifyObservers()
{
    for( auto currentObserver : observers_ ){
        currentObserver->update();
    }
}




} // namespace como
