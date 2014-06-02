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

#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <client/models/3d/mesh.hpp>
#include <common/commands/drawable_commands/light_creation_command.hpp>

namespace como {

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
         * TODO: Complete
         */
        Light( LightType type, const char* meshPath, GLuint lightIndex, MaterialConstPtr material, PackableColor lightColor );

        /*! \brief Copy constructor */
        // TODO: Define a right copy constructor when inplementing multiple lights
        // (colorLocation_ attribute would point to different places from light to
        // light).
        Light( const Light& ) = default;

        /*! \brief Move constructor */
        Light( Light&& ) = delete;

        virtual DrawablePtr clone() = 0;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~Light() = default; // TODO: Destroy Light in shader.


        /***
         * 3. Getters
         ***/

        /*! \brief Get Light's light color */
        glm::vec3 getLightColor() const; // TODO: Return a integer vector.

        /*! \brief Get the light type */
        LightType getType() const;


        /***
         * 4. Setters
         ***/

        /*! \brief Set Light's light color */
        void setLightColor( PackableColor color );


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
