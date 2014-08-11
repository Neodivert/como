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

#ifndef LIGHT_SSELECTION_HPP
#define LIGHT_SSELECTION_HPP

#include <client/managers/drawables_selection/drawables_selection.hpp>
#include <client/models/3d/lights/light_properties.hpp>
#include <map>

namespace como {

class LightsSelection : public ContainerObserver<ResourceID>
{
    public:
        /***
         * 1. Construction
         ***/
        LightsSelection() = delete;
        LightsSelection( DrawablesSelectionPtr drawablesSelection );
        LightsSelection( const LightsSelection& ) = delete;
        LightsSelection( LightsSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsSelection();


        /***
         * 3. Updating (Observer pattern).
         ***/
        virtual void update( ContainerAction lastContainerAction, ResourceID lastElementModified );


        /***
         * 3. Operators
         ***/
        LightsSelection& operator = ( const LightsSelection& ) = delete;
        LightsSelection& operator = ( LightsSelection&& ) = delete;


    private:
        DrawablesSelectionPtr drawablesSelection_;

        std::map< ResourceID, LightPropertiesSharedPtr > lightsProperties_;
};

} // namespace como

#endif // LIGHT_SSELECTION_HPP
