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

#ifndef CHANGEABLE_HPP
#define CHANGEABLE_HPP

namespace como {

class Changeable
{
    private:
        bool changedSinceLastQuery_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        Changeable( bool changed = true );

        /*! \brief Copy constructor */
        Changeable( const Changeable& ) = default;

        /*! \brief Move constructor */
        Changeable( Changeable&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~Changeable() = default;


        /***
         * 3. Getters
         ***/

        /*!
         * \brief returns true if setChanged() has been called since last
         * call hasChangedSinceLastQuery().
         */
        bool hasChangedSinceLastQuery();


        /***
         * 4. Setters
         ***/

        /*!
         * \brief sets this object's internal state to "changed since last
         * query".
         */
        void setChanged();


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        Changeable& operator = ( const Changeable& ) = delete;

        /*! \brief Move assignment operator */
        Changeable& operator = ( Changeable&& ) = delete;
};

} // namespace como

#endif // CHANGEABLE_HPP
