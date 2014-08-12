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

#ifndef TOOLS_MENU_HPP
#define TOOLS_MENU_HPP

#include <QFrame>
#include <QTabWidget>
#include <QColorDialog>
#include <QDir>
#include <client/managers/como_app.hpp>
#include <client/gui/materials/materials_editor.hpp>


namespace como {

class ToolsMenu : public QTabWidget
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        ToolsMenu() = delete;
        ToolsMenu( const ToolsMenu& ) = delete;
        ToolsMenu( ToolsMenu&& ) = delete;
        ToolsMenu( QWidget* parent, shared_ptr< ComoApp > comoApp );


        /***
         * 2. Destruction
         ***/
        ~ToolsMenu() = default;


        /***
         * 3. Operators
         ***/
        ToolsMenu& operator = ( const ToolsMenu& ) = delete;
        ToolsMenu& operator = ( ToolsMenu&& ) = delete;


    private:
        shared_ptr< ComoApp > comoApp_;
};

} // namespace como

#endif // TOOLS_MENU_HPP
