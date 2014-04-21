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

enum class LightType : std::uint8_t
{
    DIRECTIONAL_LIGHT = 0
};

/*!
 * \class Light
 *
 * \brief Base class for all type of Lights.
 */
class Light : public Mesh
{
    private:
        /*! Light type */
        const LightType type_;

        /*!
         * Location of the GLSL shader uniform variable keeping this light's
         * color.
         */
        GLint colorLocation_;

    public:
        /***
         * 1. Construction
         ***/

        /*!
         * \brief Constructs a light from the given data.
         * \param type light type
         * \param primitivePath path to the file containing the specification
         * for the light's associated mesh.
         * \param lightIndex index of this light's data into the array of
         * lights in GLSL shader.
         * \param color light color.
         */
        Light( LightType type, const char* meshPath, GLuint lightIndex, glm::vec3 color );

        /*! \brief Copy constructor */
        Light( const Light& ) = delete;

        /*! \brief Move constructor */
        Light( Light&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~Light() = default; // TODO: Destroy Light in shader.


        /***
         * 3. Getters
         ***/

        /*! \brief Get Light's light color */
        glm::vec3 getLightColor() const;

        /*! \brief Get the light type */
        LightType getType() const;


        /***
         * 4. Setters
         ***/

        /*! \brief Set Light's light color */
        void setLightColor( const glm::vec3& color );


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
