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

#include <client/models/3d/mesh.hpp>
#include "directional_light_properties.hpp"

namespace como {

/*!
 * \class DirectionalLight
 *
 * \brief A far, far away light.
 */
class DirectionalLight : public Mesh
{
    private:
        DirectionalLightPropertiesSharedPtr lightProperties_;


    public:
        /***
         * 1. Construction
         ***/

        DirectionalLight() = delete;
        DirectionalLight( DirectionalLightPropertiesSharedPtr lightProperties, MaterialConstPtr meshMaterial );

        /*! \brief Copy constructor */
        // TODO: Define a proper copy constructor and a clone() method when
        // inplementing multiple lights
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
        ~DirectionalLight() = default;


        /***
         * 3. Setters
         ***/

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
};

typedef shared_ptr< DirectionalLight > DirectionalLightPtr;

} // namespace como

#endif // DIRECTIONAL_LIGHT_HPP
