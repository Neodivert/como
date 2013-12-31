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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef TOOLS_MENU_HPP
#define TOOLS_MENU_HPP

#include <QFrame>
#include <QColorDialog>
#include "../managers/como_app.hpp"

namespace como {

class ToolsMenu : public QFrame
{
    Q_OBJECT

    protected:
        // Pointer to app's current state.
        shared_ptr< ComoApp > comoApp;

        // Current color (used when creating new drawables)
        QColor currentColor_;

        // Label indicating the current color.
        QLabel* currentColorLabel_;

        /***
         * 1. Initialization and destruction
         ***/
    public:
        ToolsMenu( QWidget* parent, shared_ptr< ComoApp > comoApp );

    protected:
        QGroupBox* createPrimitiveCreationMenu();
        QGroupBox* createPivotPointModeSelector();
        QFrame* createColorSelector();


        /***
         * 2. Getters
         ***/
    public:
        QColor getCurrentColor() const ;


        /***
         * 3. Auxiliar methods
         ***/
    protected:
        void changeCurrentColor();

    signals:
        void currentColorChanged( QColor newColor );
};

} // namespace como

#endif // TOOLS_MENU_HPP
