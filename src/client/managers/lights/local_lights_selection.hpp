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

#ifndef LOCAL_LIGHTS_SELECTION_HPP
#define LOCAL_LIGHTS_SELECTION_HPP

#include <client/managers/lights/lights_selection.hpp>
#include <client/managers/selections/resources/local_resources_selection.hpp>
#include <client/managers/utilities/server_writer.hpp>

namespace como {

class LocalLightsSelection : public LightsSelection, public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        LocalLightsSelection( ServerInterfacePtr server );
        LocalLightsSelection() = delete;
        LocalLightsSelection( const LocalLightsSelection& ) = delete;
        LocalLightsSelection( LocalLightsSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LocalLightsSelection() = default;


        /***
         * 3. Lights management
         ***/
        ResourceID addResource( std::unique_ptr<DirectionalLight> resource );


        /***
         * 4. Operators
         ***/
        LocalLightsSelection& operator = ( const LocalLightsSelection& ) = delete;
        LocalLightsSelection& operator = ( LocalLightsSelection&& ) = delete;
};

} // namespace como

#endif // LOCAL_LIGHTS_SELECTION_HPP
