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

#ifndef LIGHT_PANEL_HPP
#define LIGHT_PANEL_HPP

#include <QFrame>
#include <QLineEdit>
#include <client/managers/lights/light_handler.hpp>
#include <client/gui/utilities/color_button.hpp>

namespace como {

class LightPanel : public QFrame
{
    Q_OBJECT

    private:
        LightHandlerPtr currentLight_;

        ColorButton* lightColorButton_;

    public:
        /***
         * 1. Construction
         ***/
        LightPanel();
        LightPanel( const LightPanel& ) = delete;
        LightPanel( LightPanel&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightPanel() = default;


        /***
         * 3. Operators
         ***/
        LightPanel& operator = ( const LightPanel& ) = delete;
        LightPanel& operator = ( LightPanel&& ) = delete;


        /***
         * 4. Refreshing
         ***/
        void refresh();


        /***
         * 5. Slots
         ***/
    public slots:
        void openLight( LightHandlerPtr light );
};

} // namespace como

#endif // LIGHT_PANEL_HPP
