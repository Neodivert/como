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

#ifndef OBSERVABLE_CONTAINER_HPP
#define OBSERVABLE_CONTAINER_HPP

#include <unordered_set>
#include "container_observer.hpp"
#include <common/utilities/observer_pattern/observable.hpp>

namespace como {

template <class ElementID>
class ObservableContainer : public Observable {
    private:
        std::unordered_set< ContainerObserver<ElementID>* > observers_;

    public:
        /***
         * 1. Construction
         ***/
        ObservableContainer() = default;
        ObservableContainer( const ObservableContainer<ElementID>& ) = default;
        ObservableContainer( ObservableContainer<ElementID>&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~ObservableContainer() = default;


        /***
         * 3. Observers management
         ***/
        void addObserver( ContainerObserver< ElementID>* observer );
        void removeObserver( ContainerObserver< ElementID>* observerToBeRemoved );


        /***
         * 4. Operators
         ***/
        ObservableContainer& operator = ( const ObservableContainer<ElementID>& ) = default;
        ObservableContainer& operator = ( ObservableContainer<ElementID>&& ) = default;


    protected:
        /***
         * 5. Notifications
         ***/
        void notifyElementInsertion( ElementID id );
        void notifyElementUpdate( ElementID id );
        void notifyElementDeletion( ElementID id );
        void notifyContainerUpdate();


    private:
        /***
         * 6. Auxiliar methods
         ***/
        void notifyElementAction( ElementID id, ContainerAction action );
};


/***
 * 3. Observers management
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::addObserver( ContainerObserver<ElementID>* observer )
{
    observers_.insert( observer );
}

template <class ElementID>
void ObservableContainer<ElementID>::removeObserver( ContainerObserver<ElementID>* observer )
{
    observers_.erase( observer );
}


/***
 * 5. Notifications
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::notifyElementInsertion( ElementID id )
{
    notifyElementAction( id, ContainerAction::ELEMENT_INSERTION );
}


template <class ElementID>
void ObservableContainer<ElementID>::notifyElementUpdate( ElementID id )
{
    notifyElementAction( id, ContainerAction::ELEMENT_UPDATE );
}


template <class ElementID>
void ObservableContainer<ElementID>::notifyElementDeletion( ElementID id )
{
    notifyElementAction( id, ContainerAction::ELEMENT_DELETION );
}


template <class ElementID>
void ObservableContainer<ElementID>::notifyContainerUpdate()
{
    notifyElementAction( ElementID(), ContainerAction::CONTAINER_UPDATE );
}


/***
 * 6. Auxiliar methods
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::notifyElementAction( ElementID id, ContainerAction action )
{
    for( auto observer : observers_ ){
        observer->update( action, id );
    }

    // Also notify "plain observers" (those who don't care about the last
    // container action).
    notifyObservers();
}

} // namespace como

#endif // OBSERVABLE_CONTAINER_HPP
