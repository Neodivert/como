/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef PACKABLE_MATERIAL_ID_HPP
#define PACKABLE_MATERIAL_ID_HPP

#include <common/ids/material_id.hpp>
#include <common/packables/composite_packable.hpp>

namespace como {

// TODO: Create a AbstractPackableWrapper which works also with pointers
// (PackableWrapper<MaterialID>( &materialID_ ) ) and inherit this from
// CompositePackable too?
// TODO: Create and use a Wrapper class.
class PackableMaterialID : public CompositePackable//, public virtual AbstractPackableWrapper< MaterialID >
{
    private:
        PackableUserID creatorID_;
        PackableMaterialIndex materialIndex_;


    public:
        /***
         * 1. Construction
         ***/

        PackableMaterialID();

        /*! \brief Default constructor */
        PackableMaterialID( const MaterialID& materialID );

        /*! \brief Copy constructor */
        PackableMaterialID( const PackableMaterialID& b );

        /*! \brief Move constructor */
        PackableMaterialID( PackableMaterialID&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PackableMaterialID() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief Returns the valued held by this PackableMaterialID */
        virtual MaterialID getValue() const;


        /***
         * 4. Setters
         ***/

        /*! \brief Set this PackableMaterialID's inner value */
        virtual void setValue( MaterialID materialID );


        /***
         * 5. Operators
         ***/

        /*! \brief Assigns the given value to this PackableMaterialID */
        PackableMaterialID& operator = ( const MaterialID& materialID );

        /*! \brief Copy assignment operator */
        PackableMaterialID& operator = ( const PackableMaterialID& ) = delete;

        /*! \brief Move assignment operator */
        PackableMaterialID& operator = ( PackableMaterialID&& ) = delete;
};

} // namespace como

#endif // PACKABLE_MATERIAL_ID_HPP
