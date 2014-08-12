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
#include <client/managers/selections/resources/resources_selection.hpp>

namespace como {

class LightsSelection : public virtual ResourcesSelection<LightProperties>, public ContainerObserver<ResourceID>, public AbstractLight
{
    public:
        /***
         * 1. Construction
         ***/
        LightsSelection() = default;
        LightsSelection( DrawablesSelectionPtr drawablesSelection );
        LightsSelection( const LightsSelection& ) = delete;
        LightsSelection( LightsSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsSelection();


        /***
         * 3. Getters
         ***/
        virtual PackableColor getLightColor() const;
        virtual float getAmbientCoefficient() const;


        /***
         * 4. Setters
         ***/
        virtual void setLightColor( const PackableColor& color );
        virtual void setAmbientCoefficient( float coefficient );


        /***
         * 5. Updating (Observer pattern).
         ***/
        virtual void update( ContainerAction lastContainerAction, ResourceID lastElementModified );


        /***
         * 6. Operators
         ***/
        LightsSelection& operator = ( const LightsSelection& ) = delete;
        LightsSelection& operator = ( LightsSelection&& ) = delete;


    private:
        DrawablesSelectionPtr drawablesSelection_;
};

} // namespace como

#endif // LIGHT_SSELECTION_HPP
