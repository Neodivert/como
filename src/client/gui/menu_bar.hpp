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

#ifndef MENU_BAR_HPP
#define MENU_BAR_HPP

#include <QMenuBar>
#include <client/managers/drawables_manager/drawables_manager.hpp>

namespace como {

class MenuBar : public QMenuBar
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        MenuBar() = delete;
        MenuBar( DrawablesManagerPtr drawablesManager );
        MenuBar( const MenuBar& ) = delete;
        MenuBar( MenuBar&& ) = delete;


        /***
         * 2. Initialization
         ***/
    private:
        QMenu* createViewMenu( DrawablesManager* drawablesManager );
        QMenu* createDisplayEdgesMenu( DrawablesManager* drawablesManager );
        QMenu* createDisplayVertexNormalsMenu( DrawablesManager* drawablesManager );
    public:

        /***
         * 2. Destruction
         ***/
        ~MenuBar() = default;


        /***
         * 3. Operators
         ***/
        MenuBar& operator = ( const MenuBar& ) = delete;
        MenuBar& operator = ( MenuBar&& ) = delete;
};

} // namespace como

#endif // MENU_BAR_HPP
