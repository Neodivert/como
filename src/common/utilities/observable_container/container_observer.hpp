#ifndef CONTAINER_ContainerObserver_HPP
#define CONTAINER_ContainerObserver_HPP

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

namespace como {

enum class ContainerAction {
    ELEMENT_INSERTION,
    ELEMENT_UPDATE,
    ELEMENT_DELETION,
    CONTAINER_UPDATE
};


template <class ElementID >
class ContainerObserver
{
    public:
        /***
         * 1. Construction
         ***/
        ContainerObserver() = default;
        ContainerObserver( const ContainerObserver& ) = default;
        ContainerObserver( ContainerObserver&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~ContainerObserver() = default;


        /***
         * 3. Updating
         ***/
        virtual void update( ContainerAction lastContainerAction, ElementID lastElementModified ) = 0;


        /***
         * 4. Operators
         ***/
        ContainerObserver& operator = ( const ContainerObserver& ) = default;
        ContainerObserver& operator = ( ContainerObserver&& ) = default;
};

} // namespace como

#endif // CONTAINER_ContainerObserver_HPP
