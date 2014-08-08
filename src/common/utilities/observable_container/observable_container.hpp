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

#include <common/utilities/observer_pattern/observable.hpp>
#include <common/utilities/observable_container/informer_container.hpp>

namespace como {


template <class ElementID>
class ObservableContainer : public InformerContainer< ElementID >, public Observable {
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
         * 3. Operators
         ***/
        ObservableContainer& operator = ( const ObservableContainer<ElementID>& ) = delete;
        ObservableContainer& operator = ( ObservableContainer<ElementID>&& ) = delete;


    protected:
        /***
         * 4. Notifications
         ***/
        void notifyElementInsertion( ElementID id );
        void notifyElementUpdate( ElementID id );
        void notifyElementDeletion( ElementID id );


    private:
        /***
         * 5. Auxiliar methods
         ***/
        void notifyElementAction( ElementID id, ContainerElementAction action );
};


/***
 * 4. Notifications
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::notifyElementInsertion( ElementID id )
{
    notifyElementAction( id, ContainerElementAction::INSERTION );
}


template <class ElementID>
void ObservableContainer<ElementID>::notifyElementUpdate( ElementID id )
{
    notifyElementAction( id, ContainerElementAction::UPDATE );
}


template <class ElementID>
void ObservableContainer<ElementID>::notifyElementDeletion( ElementID id )
{
    notifyElementAction( id, ContainerElementAction::DELETION );
}


/***
 * 5. Auxiliar methods
 ***/

template <class ElementID>
void ObservableContainer<ElementID>::notifyElementAction( ElementID id, ContainerElementAction action )
{
    this->setLastElementAction( id, action );

    notifyObservers();
}

} // namespace como

#endif // OBSERVABLE_CONTAINER_HPP
