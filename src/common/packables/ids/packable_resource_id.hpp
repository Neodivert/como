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

#ifndef PACKABLE_RESOURCE_ID_HPP
#define PACKABLE_RESOURCE_ID_HPP

#include <common/ids/resource_id.hpp>
#include <common/packables/composite_packable.hpp>

namespace como {

// TODO: Create a AbstractPackableWrapper which works also with pointers
// (PackableWrapper<ResourceID>( &ResourceID_ ) ) and inherit this from
// CompositePackable too?
// TODO: Create and use a Wrapper class.
class PackableResourceID : public CompositePackable//, public virtual AbstractPackableWrapper< ResourceID >
{
    private:
        PackableUserID creatorID_;
        PackableResourceIndex resourceIndex_;


    public:
        /***
         * 1. Construction
         ***/

        PackableResourceID();

        /*! \brief Default constructor */
        PackableResourceID( const ResourceID& resourceID );

        /*! \brief Copy constructor */
        PackableResourceID( const PackableResourceID& b );

        /*! \brief Move constructor */
        PackableResourceID( PackableResourceID&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        virtual ~PackableResourceID() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief Returns the valued held by this PackableResourceID */
        virtual ResourceID getValue() const;


        /***
         * 4. Setters
         ***/

        /*! \brief Set this PackableResourceID's inner value */
        virtual void setValue( ResourceID resourceID );


        /***
         * 5. Operators
         ***/

        /*! \brief Assigns the given value to this PackableResourceID */
        PackableResourceID& operator = ( const ResourceID& resourceID );

        /*! \brief Copy assignment operator */
        PackableResourceID& operator = ( const PackableResourceID& ) = delete;

        /*! \brief Move assignment operator */
        PackableResourceID& operator = ( PackableResourceID&& ) = delete;
};

} // namespace como

#endif // PACKABLE_MATERIAL_ID_HPP
