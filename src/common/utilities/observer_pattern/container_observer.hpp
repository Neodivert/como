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

namespace como {

template <class IDType>
class ContainerObserver
{
    public:
        /***
         * 1. Construction
         ***/
        ContainerObserver() = default;
        ContainerObserver( const ContainerObserver& ) = delete;
        ContainerObserver( ContainerObserver&& ) = delete;


        /***
         * 2. Deletion
         ***/
        ~ContainerObserver() = default;

        /***
         * 3. Updating
         ***/
        virtual void onElementInsertion( IDtype element ) = 0;
        virtual void onElementDeletion( IDtype element ) = 0;
        virtual void onElementModification( IDtype element ) = 0;


        /***
         * 4. Operators
         ***/
        ContainerObserver& operator = ( const ContainerObserver& ) = delete;
        ContainerObserver& operator = ( ContainerObserver&& ) = delete;
};

} // namespace como

#endif // CONTAINER_OBSERVER_HPP
