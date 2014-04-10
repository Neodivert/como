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

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "../mesh.hpp"

namespace como {

/*!
 * \class Light
 *
 * \brief Base class for all type of Lights.
 */
class Light : public Mesh
{
    private:
        /*! Light position */
        glm::vec3 position_;

        /*! Light's light color */
        glm::vec3 lightColor_;

    public:
        /***
         * 1. Construction
         ***/

        /*!
         * \brief Constructs a Light at the given position and with the
         * specified light color
         */
        Light( glm::vec3 position, glm::vec3 lightColor );

        /*! \brief Copy constructor */
        Light( const Light& ) = default;

        /*! \brief Move constructor */
        Light( Light&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~Light() = default; // TODO: Destroy Light in shader.


        /***
         * 3. Getters
         ***/

        /*! \brief Get Light's position */
        glm::vec3 getPosition();

        /*! \brief Get Light's light color */
        glm::vec3 getLightColor();


        /***
         * 4. Setters
         ***/

        // TODO: Use Mesh::translate.
        /*! \brief Move Light to the given position */
        virtual void setPosition( const glm::vec3& position );


        /*! \brief Set Light's light color */
        void setLightColor( const glm::vec3& color );

        /*! \brief Set Ambient light */
        static void setAmbientLight( const glm::vec3& color );


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        Light& operator=( const Light& ) = delete ;

        /*! \brief Move assignment operator */
        Light& operator=( Light&& ) = delete;
};

} // namespace como

#endif // LIGHT_HPP
