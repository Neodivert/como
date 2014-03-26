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

#ifndef DIRECTIONAL_LAMP_HPP
#define DIRECTIONAL_LAMP_HPP

#include "lamp.hpp"

namespace como {

/*!
 * \class DirectionalLamp
 *
 * \brief A far, far away lamp.
 */
class DirectionalLamp : public Lamp
{
    public:
        /***
         * 1. Construction
         ***/

        /*!
         * \brief Constructs a directional lamp at the given position and with
         * the specified light color
         */
        DirectionalLamp( glm::vec3 position, glm::vec3 lightColor );

        /*! \brief Copy constructor */
        DirectionalLamp( const DirectionalLamp& ) = default;

        /*! \brief Move constructor */
        DirectionalLamp( DirectionalLamp&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~DirectionalLamp() = default; // TODO: Destroy lamp in shader.


        /***
         * 3. Setters
         ***/

        /*! \brief Move Lamp to the given position */
        virtual void setPosition( const glm::vec3& position );


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        Lamp& operator=( const Lamp& ) = delete ;

        /*! \brief Move assignment operator */
        Lamp& operator=( Lamp&& ) = delete;
};

} // namespace como

#endif // DIRECTIONAL_LAMP_HPP
