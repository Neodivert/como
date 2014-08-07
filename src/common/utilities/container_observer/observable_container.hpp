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
#include <common/utilities/observer_pattern/observer.hpp>
#include <common/utilities/container_observer/informer_container.hpp>

namespace como {


template <class ElementID>
class ObservableContainer : public InformerContainer< ElementID > {
    private:
        std::unordered_set< Observer* > observers_;

    public:
        /***
         * 1. Construction
         ***/
        ObservableContainer() = default;
        ObservableContainer( const ObservableContainer<ElementID>& ) = delete;
        ObservableContainer( ObservableContainer<ElementID>&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ObservableContainer() = default;


        /***
         * 3. Observables management
         ***/
        void addObserver( Observer* observer );
        void removeObserver( Observer* observer );


        /***
         * 4. Operators
         ***/
        ObservableContainer& operator = ( const ObservableContainer<ElementID>& ) = delete;
        ObservableContainer& operator = ( ObservableContainer<ElementID>&& ) = delete;


    protected:
        /***
         * 5. Notifications
         ***/
        void notifyElementInsertion( ElementID id );
        void notifyElementDeletion( ElementID id );
        void notifyElementModification( ElementID id );


    private:
        /***
         * 6. Auxiliar methods
         ***/
        void notifyElementAction( ElementID id, ContainerElementAction action );
};

/***
 * 3. Observables management
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::addObserver( Observer* observer )
{
    observers_.insert( observer );
}


template <class ElementID>
void ObservableContainer<ElementID>::removeObserver( Observer* observer )
{
    observers_.erase( observer );
}


/***
 * 5. Notifications
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::notifyElementInsertion( ElementID id )
{
    notifyElementAction( id, ContainerElementAction::INSERTION );
}


template <class ElementID>
void ObservableContainer<ElementID>::notifyElementDeletion( ElementID id )
{
    notifyElementAction( id, ContainerElementAction::DELETION );
}


template <class ElementID>
void ObservableContainer<ElementID>::notifyElementModification( ElementID id )
{
    notifyElementAction( id, ContainerElementAction::UPDATE );
}


/***
 * 7. Auxiliar methods
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::notifyElementAction( ElementID id, ContainerElementAction action )
{
    this->setLastElementAction( id, action );

    for( auto observer : observers_ ){
        observer->update();
    }
}

} // namespace como

#endif // OBSERVABLE_CONTAINER_HPP
