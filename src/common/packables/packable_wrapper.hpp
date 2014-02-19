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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef PACKABLE_WRAPPER_HPP
#define PACKABLE_WRAPPER_HPP

#include "packable.hpp"

template <class T>
class PackableWrapper : public Packable {
    protected:
        T* value_;

    public:
        /***
         * 1. Construction
         ***/
        PackableWrapper( T* value ) : value_( value ){}
        PackableWrapper( const PackableWrapper& ) = default;
        PackableWrapper( PackableWrapper&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableWrapper() = default;


        /***
         * 3. Getters
         ***/
        T getValue() const { return *value_; }


        /***
         * 4. Setters
         ***/
        void setValue( T value ){ *value_ = value; }


        /***
         * 5. Operators
         ***/
        PackableWrapper& operator = (const PackableWrapper& b) = delete;
        PackableWrapper& operator = ( PackableWrapper&& ) = delete;
};


#endif // PACKABLE_WRAPPER_HPP
