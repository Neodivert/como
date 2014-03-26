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

#ifndef LAMP_HPP
#define LAMP_HPP

#include "../mesh.hpp"

namespace como {

/*!
 * \class Lamp
 *
 * \brief Base class for all type of lamps.
 */
class Lamp : public Mesh
{
    private:
        /*! Lamp position */
        glm::vec3 position_;

        /*! Lamp's light color */
        glm::vec3 lightColor_;

    public:
        /***
         * 1. Construction
         ***/

        /*!
         * \brief Constructs a lamp at the given position and with the
         * specified light color
         */
        Lamp( glm::vec3 position, glm::vec3 lightColor );

        /*! \brief Copy constructor */
        Lamp( const Lamp& ) = default;

        /*! \brief Move constructor */
        Lamp( Lamp&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~Lamp() = default; // TODO: Destroy lamp in shader.


        /***
         * 3. Getters
         ***/

        /*! \brief Get Lamp's position */
        glm::vec3 getPosition();

        /*! \brief Get Lamp's light color */
        glm::vec3 getLightColor();


        /***
         * 4. Setters
         ***/

        /*! \brief Move Lamp to the given position */
        virtual void setPosition( const glm::vec3& position );


        /*! \brief Set Lamp's light color */
        void setLightColor( const glm::vec3& color );


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        Lamp& operator=( const Lamp& ) = delete ;

        /*! \brief Move assignment operator */
        Lamp& operator=( Lamp&& ) = delete;
};

} // namespace como

#endif // LAMP_HPP
