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
        std::unordered_set< ContainerObserver< IDType > > insertionObservers_;
        std::unordered_set< ContainerObserver< IDType > > modificationObservers_;
        std::unordered_set< ContainerObserver< IDType > > deletionObservers_;

    public:
        /***
         * 1. Construction
         ***/
        ObservableContainer() = default;
        ObservableContainer( const ObservableContainer& ) = delete;
        ObservableContainer( ObservableContainer&& ) = delete;


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
        ObservableContainer& operator = ( const ObservableContainer& ) = delete;
        ObservableContainer& operator = ( ObservableContainer&& ) = delete;
};

/***
 * 3. Observables management
 ***/

template <class IDType>
void ObservableContainer<IDType>::addObserver( ContainerObserver<como::IDType> *observer, ContainerActionType observedAction )
{
    switch( observedAction ){
        case ContainerActionType::ELEMENT_INSERTION:
            insertionObservers_.insert( observer );
        break;
        case ContainerActionType::ELEMENT_DELETION:
            deletionObservers_.insert( observer );
        break;
        case ContainerActionType::ELEMENT_MODIFICATION:
            modificationObservers_.insert( observer );
        break;
        case ContainerActionType::ALL:
            insertionObservers_.insert( observer );
            deletionObservers_.insert( observer );
            modificationObservers_.insert( observer );
        break;
    }
}


template <class IDType>
void ObservableContainer<IDType>::removeObserver(ContainerObserver<como::IDType> *observer, ContainerActionType observedAction)
{
    switch( observedAction ){
        case ContainerActionType::ELEMENT_INSERTION:
            insertionObservers_.erase( observer );
        break;
        case ContainerActionType::ELEMENT_DELETION:
            deletionObservers_.erase( observer );
        break;
        case ContainerActionType::ELEMENT_MODIFICATION:
            modificationObservers_.erase( observer );
        break;
        case ContainerActionType::ALL:
            insertionObservers_.erase( observer );
            deletionObservers_.erase( observer );
            modificationObservers_.erase( observer );
        break;
    }
}


/***
 * 4. Notifications
 ***/

void ObservableContainer<IDType>::notifyElementInsertion( IDType id ) const
{
    for( auto observer : insertionObservers_ ){
        observer->onElementInsertion( id );
    }
}


template <class IDType>
void ObservableContainer<IDType>::notifyElementDeletion( IDType id ) const
{
    for( auto observer : deletionObservers_ ){
        observer->onElementDeletion( id );
    }
}


template <class IDType>
void ObservableContainer<IDType>::notifyElementModification( IDType id) const
{
    for( auto observer : modificationObservers_ ){
        observer->onElementModification( id );
    }
}

} // namespace como

#endif // OBSERVABLE_CONTAINER_HPP
