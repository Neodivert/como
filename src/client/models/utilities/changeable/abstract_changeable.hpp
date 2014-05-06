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

#ifndef ABSTRACT_CHANGEABLE_HPP
#define ABSTRACT_CHANGEABLE_HPP

namespace como {

class AbstractChangeable
{
    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        AbstractChangeable() = default;

        /*! \brief Copy constructor */
        AbstractChangeable( const AbstractChangeable& ) = default;

        /*! \brief Move constructor */
        AbstractChangeable( AbstractChangeable&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~AbstractChangeable() = default;


        /***
         * 3. Getters
         ***/

        /*!
         * \brief returns true if setChanged() has been called since last
         * call hasChangedSinceLastQuery().
         */
        virtual bool hasChangedSinceLastQuery() = 0;


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        AbstractChangeable& operator = ( const AbstractChangeable& ) = delete;

        /*! \brief Move assignment operator */
        AbstractChangeable& operator = ( AbstractChangeable&& ) = delete;
};

} // namespace como

#endif // ABSTRACT_CHANGEABLE_HPP
