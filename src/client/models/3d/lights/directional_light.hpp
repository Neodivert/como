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

#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

#include "light.hpp"

namespace como {

/*!
 * \class DirectionalLight
 *
 * \brief A far, far away Light.
 */
class DirectionalLight : public Light
{
    private:
        /*! Location of the light vector in GLSL shader. */
        GLint lightVectorLocation_;

        /*! Location of the half vector in GLSL shader. */
        GLint halfVectorLocation_;

    public:
        /***
         * 1. Construction
         ***/

        /*!
         * \brief Constructs a directional Light with the specified light
         * color.
         */
        DirectionalLight( const PackableColor& meshColor, const PackableColor& lightColor );

        /*! \brief Copy constructor */
        // TODO: Define a proper copy constructor when inplementing multiple lights
        // ("location attributes" would point to different places from light to
        // light).
        DirectionalLight( const DirectionalLight& ) = default;

        /*! \brief Move constructor */
        DirectionalLight( DirectionalLight&& ) = delete;


        virtual DrawablePtr clone();


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~DirectionalLight() = default; // TODO: Destroy Light in shader.


        /***
         * 3. Setters
         ***/

    private:
        void update();
    private:


        /***
         * 3. Operators
         ***/

        /*! \brief Copy assignment operator */
        DirectionalLight& operator=( const DirectionalLight& ) = delete ;

        /*! \brief Move assignment operator */
        DirectionalLight& operator=( DirectionalLight&& ) = delete;
};

typedef shared_ptr< DirectionalLight > DirectionalLightPtr;

} // namespace como

#endif // DIRECTIONAL_LIGHT_HPP
