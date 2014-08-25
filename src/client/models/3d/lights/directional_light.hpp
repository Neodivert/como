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

#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include <client/models/3d/lights/light.hpp>
#include "directional_light.hpp"

namespace como {

/*!
 * \class DirectionalLight
 *
 * \brief A far, far away light.
 */
class DirectionalLight : public Light
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
        DirectionalLight() = delete;
        DirectionalLight( GLuint directionalLightIndex, GLint lightIndex, const PackableColor& lightColor, const glm::vec3& lightVector, MaterialConstPtr meshMaterial );
        DirectionalLight( const DirectionalLight& ) = default; // TODO: Implement (or remove clone()).
        DirectionalLight( DirectionalLight&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~DirectionalLight();


        /***
         * 3. Getters
         ***/
        glm::vec3 getLightVector() const;
        glm::vec3 getHalfVector() const;
        GLint getLightIndex() const;


        /***
         * 4. Setters
         ***/
        void setLightVector( const glm::vec3& lightVector );



    private:
        void update();
    public:


        /***
         * 5. Lights management
         ***/
        static unsigned int getMaxLights();


        /***
         * 6. Operators
         ***/

        /*! \brief Copy assignment operator */
        DirectionalLight& operator=( const DirectionalLight& ) = delete ;

        /*! \brief Move assignment operator */
        DirectionalLight& operator=( DirectionalLight&& ) = delete;


        /***
         * 7. Auxiliar methods
         ***/
        virtual bool containsProperty( const void* property ) const;
};

typedef shared_ptr< DirectionalLight > DirectionalLightPtr;

} // namespace como

#endif // DIRECTIONAL_LIGHT_HPP
