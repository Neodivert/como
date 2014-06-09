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

#ifndef DIRECTIONAL_LIGHT_PROPERTIES_HPP
#define DIRECTIONAL_LIGHT_PROPERTIES_HPP

#include "light_properties.hpp"

namespace como {

class DirectionalLightProperties : public LightProperties
{
    private:
        /*! Location of the light index in GLSL shader. */
        GLint lightIndexLocation_;

        /*! Location of the light vector in GLSL shader. */
        GLint lightVectorLocation_;

        /*! Location of the half vector in GLSL shader. */
        GLint halfVectorLocation_;

    public:
        /***
         * 1. Construction
         ***/
        DirectionalLightProperties() = delete;
        DirectionalLightProperties( GLuint directionalLightIndex, GLint lightIndex, const PackableColor& lightColor, const glm::vec3& lightVector );
        DirectionalLightProperties( const DirectionalLightProperties& ) = delete;
        DirectionalLightProperties( DirectionalLightProperties&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DirectionalLightProperties() = default;


        /***
         * 3. Getters
         ***/
        glm::vec3 getLightVector() const;
        glm::vec3 getHalfVector() const;


        /***
         * 4. Setters
         ***/
        void setLightVector( const glm::vec3& lightVector );


        /***
         * 5. Operators
         ***/
        DirectionalLightProperties& operator = ( const DirectionalLightProperties& ) = delete;
        DirectionalLightProperties& operator = ( DirectionalLightProperties&& ) = delete;
};

} // namespace como

#endif // DIRECTIONAL_LIGHT_PROPERTIES_HPP
