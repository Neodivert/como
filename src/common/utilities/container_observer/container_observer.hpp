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

#ifndef CONTAINER_OBSERVER_HPP
#define CONTAINER_OBSERVER_HPP

#include <common/utilities/container_observer/observable_container.hpp>
#include <common/utilities/observer_pattern/observer.hpp>

namespace como {

template <class ElementID>
class ContainerObserver : public Observer {
    private:
        InformerContainer<ElementID>* observedContainer_;

    public:
        /***
         * 1. Construction
         ***/
        ContainerObserver() = delete;
        ContainerObserver( ContainerObserver& ) = delete;
        ContainerObserver( ContainerObserver&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ContainerObserver() = default;


        /***
         * 3. Updating (Observer pattern)
         ***/
        virtual void update();


        /***
         * 4. Operators
         ***/
        ContainerObserver& operator = ( const ContainerObserver& ) = delete;
        ContainerObserver& operator = ( ContainerObserver&& ) = delete;


    protected:
        /***
         * 5. Updating (Container observer)
         ***/
        virtual void onElementInsertion( ElementID ) = 0;
        virtual void onElementDeletion( ElementID ) = 0;
        virtual void onContainerUpdate() = 0;
};


/***
 * 3. Updating (Observer pattern)
 ***/

template <class ElementID>
void ContainerObserver::update()
{
    ElementID elementID;
    ContainerAction elementAction;

    observedContainer_->getLastElementAction( elementID, elementAction );
    switch( elementAction ){
        case ContainerAction::ELEMENT_INSERTION:
            onElementInsertion( elementID );
            onContainerUpdate();
        break;
        case ContainerAction::ELEMENT_DELETION:
            onElementDeletion( elementID );
            onContainerUpdate();
        break;
        case ContainerAction::CONTAINER_UPDATE:
            onContainerUpdate();
        break;
    }
}


} // namespace como

#endif // CONTAINER_OBSERVER_HPP
