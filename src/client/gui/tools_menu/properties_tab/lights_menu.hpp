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

#ifndef LIGHTS_MENU_HPP
#define LIGHTS_MENU_HPP

#include <QFrame>
#include <QLineEdit>
#include <client/managers/lights/lights_manager.hpp>
#include <client/managers/lights/light_handler.hpp>
#include <client/gui/utilities/color_button.hpp>
#include <common/utilities/observable_container/container_observer.hpp>

namespace como {

class LightsMenu : public QFrame, ContainerObserver<ResourceID>, Observer
{
    Q_OBJECT

    private:
        LightsManagerPtr lightsManager_;

        LightHandlerPtr currentLight_;

        ColorButton* lightColorButton_;
        QDoubleSpinBox* lightAmbientCoefficientSpinBox_;

    public:
        /***
         * 1. Construction
         ***/
        LightsMenu( LightsManagerPtr lightsManager );
        LightsMenu( const LightsMenu& ) = delete;
        LightsMenu( LightsMenu&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsMenu() = default;


        /***
         * 3. Operators
         ***/
        LightsMenu& operator = ( const LightsMenu& ) = delete;
        LightsMenu& operator = ( LightsMenu&& ) = delete;


        /***
         * 4. Updating
         ***/
        virtual void update( ContainerAction lastContainerAction, ResourceID lastElementModified );
        virtual void update();


        /***
         * 5. Light opening / closing
         ***/
    private:
        void openLight( LightHandlerPtr light );
        void closeLight();
};

} // namespace como

#endif // LIGHTS_MENU_HPP
