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
#include <client/managers/selections/lights/local_lights_selection.hpp>
#include <client/gui/utilities/color_button.hpp>
#include <QDoubleSpinBox>

namespace como {

class LightsMenu : public QFrame, public Observer
{
    Q_OBJECT

    private:
        LocalLightsSelection* lights_;

        ColorButton* lightColorButton_;
        QDoubleSpinBox* lightAmbientCoefficientSpinBox_;

    public:
        /***
         * 1. Construction
         ***/
        LightsMenu( LocalLightsSelection* lights );
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
         * 4. Updating (Observer pattern)
         ***/
        virtual void update();
};

} // namespace como

#endif // LIGHTS_MENU_HPP
