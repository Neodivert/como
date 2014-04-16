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

#ifndef LIGHTING_MANAGER_HPP
#define LIGHTING_MANAGER_HPP

#include "directional_light.hpp"

namespace como {

/*!
 * \class LightingManager
 *
 * \brief Container class which holds all the lights associated to a scene.
 */
class LightingManager
{
    private:
        DirectionalLightPtr directionalLight_;

    public:

        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        LightingManager();

        /*! \brief Copy constructor. */
        LightingManager( const LightingManager& ) = delete;

        /*! \brief Move constructor. */
        LightingManager( LightingManager&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~LightingManager() = default;


        /***
         * 3. Getters
         ***/

        /*!
         * \brief Returns a pointer to the directional light held by this
         * manager.
         */
        DirectionalLightPtr getDirectionalLight();


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        LightingManager& operator=( const LightingManager& ) = delete ;

        /*! \brief Move assignment operator */
        LightingManager& operator=( LightingManager&& ) = delete;

};

} // namespace como

#endif // LIGHTING_MANAGER_HPP
