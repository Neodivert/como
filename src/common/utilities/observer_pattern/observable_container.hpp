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

namespace como {

enum class ContainerActionType {
    ELEMENT_INSERTION,
    ELEMENT_DELETION,
    ELEMENT_MODIFICATION,
    ALL
};

template <class IDType>
class ObservableContainer {
    private:
        std::unordered_set< ContainerObserver< IDType >* > insertionObservers_;
        std::unordered_set< ContainerObserver< IDType >* > modificationObservers_;
        std::unordered_set< ContainerObserver< IDType >* > deletionObservers_;

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
        void addObserver( ContainerObserver< IDType >* observer, ContainerActionType observedAction );
        void removeObserver( ContainerObserver< IDType >* observer, ContainerActionType observedAction );


        /***
         * 4. Notifications
         ***/
        void notifyElementInsertion( IDType id ) const;
        void notifyElementDeletion( IDType id ) const;
        void notifyElementModification( IDType id ) const;


        /***
         * 5. Operators
         ***/
        ObservableContainer& operator = ( const ObservableContainer<IDType>& ) = delete;
        ObservableContainer& operator = ( ObservableContainer<IDType>&& ) = delete;
};

/***
 * 3. Observables management
 ***/

template <class IDType>
void ObservableContainer<IDType>::addObserver( ContainerObserver<IDType> *observer, ContainerActionType observedAction )
{
    switch( observedAction ){
        case ContainerActionType::ELEMENT_INSERTION:
            this->insertionObservers_.insert( observer );
        break;
        case ContainerActionType::ELEMENT_DELETION:
            this->deletionObservers_.insert( observer );
        break;
        case ContainerActionType::ELEMENT_MODIFICATION:
            this->modificationObservers_.insert( observer );
        break;
        case ContainerActionType::ALL:
            this->insertionObservers_.insert( observer );
            this->deletionObservers_.insert( observer );
            this->modificationObservers_.insert( observer );
        break;
    }
}


template <class IDType>
void ObservableContainer<IDType>::removeObserver(ContainerObserver<IDType> *observer, ContainerActionType observedAction)
{
    switch( observedAction ){
        case ContainerActionType::ELEMENT_INSERTION:
            this->insertionObservers_.erase( observer );
        break;
        case ContainerActionType::ELEMENT_DELETION:
            this->deletionObservers_.erase( observer );
        break;
        case ContainerActionType::ELEMENT_MODIFICATION:
            this->modificationObservers_.erase( observer );
        break;
        case ContainerActionType::ALL:
            this->insertionObservers_.erase( observer );
            this->deletionObservers_.erase( observer );
            this->modificationObservers_.erase( observer );
        break;
    }
}


/***
 * 4. Notifications
 ***/

template <class IDType>
void ObservableContainer<IDType>::notifyElementInsertion( IDType id ) const
{
    for( auto observer : this->insertionObservers_ ){
        observer->onElementInsertion( id );
    }
}


template <class IDType>
void ObservableContainer<IDType>::notifyElementDeletion( IDType id ) const
{
    for( auto observer : this->deletionObservers_ ){
        observer->onElementDeletion( id );
    }
}


template <class IDType>
void ObservableContainer<IDType>::notifyElementModification( IDType id) const
{
    for( auto observer : this->modificationObservers_ ){
        observer->onElementModification( id );
    }
}

} // namespace como

#endif // OBSERVABLE_CONTAINER_HPP
