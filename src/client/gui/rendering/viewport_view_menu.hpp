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

#ifndef VIEWPORT_VIEW_MENU_HPP
#define VIEWPORT_VIEW_MENU_HPP

#include <QMenu>
#include <client/gui/viewport.hpp>

namespace como {

class ViewportViewMenu : public QMenu
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        ViewportViewMenu( Viewport* viewport );
        ViewportViewMenu() = delete;
        ViewportViewMenu( const ViewportViewMenu& ) = delete;
        ViewportViewMenu( ViewportViewMenu&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ViewportViewMenu() = default;


        /***
         * 3. Setters
         ***/
    public slots:
        void setView( View view );


        /***
         * 4. Operators
         ***/
    public:
        ViewportViewMenu& operator = ( const ViewportViewMenu& ) = delete;
        ViewportViewMenu& operator = ( ViewportViewMenu&& ) = delete;
};

} // namespace como

#endif // VIEWPORT_VIEW_MENU_HPP
