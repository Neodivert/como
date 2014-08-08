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

#ifndef INFORMER_CONTAINER_HPP
#define INFORMER_CONTAINER_HPP

namespace como {

enum class ContainerElementAction {
    INSERTION,
    DELETION,
    UPDATE
};

template <class ElementID >
class InformerContainer
{
    private:
        ElementID lastElementModified_;
        ContainerElementAction lastElementAction_;

    public:
        /***
         * 1. Construction
         ***/
        InformerContainer() = default;
        InformerContainer( const InformerContainer& ) = default;
        InformerContainer( InformerContainer&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~InformerContainer() = default;


        /***
         * 3. Getters
         ***/
        void getLastElementAction( ElementID& id, ContainerElementAction& actionType ) const;


        /***
         * 4. Operators
         ***/
        InformerContainer& operator = ( const InformerContainer& ) = default;
        InformerContainer& operator = ( InformerContainer&& ) = default;


    protected:
        /***
         * 5. Setters
         ***/
        void setLastElementAction( ElementID id, ContainerElementAction actionType );
};


/***
 * 3. Getters
 ***/

template <class ElementID>
void InformerContainer<ElementID>::getLastElementAction( ElementID& id, ContainerElementAction& actionType ) const
{
    id = lastElementModified_;
    actionType = lastElementAction_;
}


/***
 * 5. Setters
 ***/

template <class ElementID>
void InformerContainer<ElementID>::setLastElementAction( ElementID id, ContainerElementAction actionType )
{
    lastElementModified_ = id;
    lastElementAction_ = actionType;
}

} // namespace como

#endif // INFORMER_CONTAINER_HPP
