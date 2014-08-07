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
#include "observer.hpp"

namespace como {

enum class ContainerElementAction {
    INSERTION,
    DELETION,
    MODIFICATION
};

template <class IDType>
class ObservableContainer {
    private:
        IDType lastElementModified_;
        ContainerElementAction lastElementAction_;

        std::unordered_set< Observer* > observers_;

    public:
        /***
         * 1. Construction
         ***/
        ObservableContainer() = default;
        ObservableContainer( const ObservableContainer<IDType>& ) = delete;
        ObservableContainer( ObservableContainer<IDType>&& ) = delete;


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
         * 4. Getters
         ***/
        void getLastAction( IDType& id, ContainerElementAction& action ) const;


        /***
         * 5. Operators
         ***/
        ObservableContainer& operator = ( const ObservableContainer<IDType>& ) = delete;
        ObservableContainer& operator = ( ObservableContainer<IDType>&& ) = delete;


    protected:
        /***
         * 6. Notifications
         ***/
        void notifyElementInsertion( IDType id );
        void notifyElementDeletion( IDType id );
        void notifyElementModification( IDType id );


    private:
        /***
         * 7. Auxiliar methods
         ***/
        void notifyElementAction( IDType id, ContainerElementAction action );
};

/***
 * 3. Observables management
 ***/

template <class IDType>
void ObservableContainer<IDType>::addObserver( Observer* observer )
{
    observers_.insert( observer );
}


template <class IDType>
void ObservableContainer<IDType>::removeObserver( Observer* observer )
{
    observers_.erase( observer );
}


/***
 * 4. Getters
 ***/

template <class IDType>
void ObservableContainer<IDType>::getLastAction( IDType& id, ContainerElementAction& action ) const
{
    id = lastElementModified_;
    action = lastElementAction_;
}


/***
 * 6. Notifications
 ***/

template <class IDType>
void ObservableContainer<IDType>::notifyElementInsertion( IDType id )
{
    notifyElementAction( id, ContainerElementAction::INSERTION );
}


template <class IDType>
void ObservableContainer<IDType>::notifyElementDeletion( IDType id )
{
    notifyElementAction( id, ContainerElementAction::DELETION );
}


template <class IDType>
void ObservableContainer<IDType>::notifyElementModification( IDType id )
{
    notifyElementAction( id, ContainerElementAction::MODIFICATION );
}


/***
 * 7. Auxiliar methods
 ***/

template <class IDType>
void ObservableContainer<IDType>::notifyElementAction( IDType id, ContainerElementAction action )
{
    lastElementModified_ = id;
    lastElementAction_ = action;

    for( auto observer : observers_ ){
        observer->update();
    }
}

} // namespace como

#endif // OBSERVABLE_CONTAINER_HPP
