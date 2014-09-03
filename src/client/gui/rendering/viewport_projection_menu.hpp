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

#ifndef VIEWPORT_PROJECTION_MENU_HPP
#define VIEWPORT_PROJECTION_MENU_HPP

#include <QMenu>
#include <client/gui/viewport.hpp>

namespace como {

class ViewportProjectionMenu : public QMenu
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        ViewportProjectionMenu( Viewport* viewport, QWidget* parent );
        ViewportProjectionMenu() = delete;
        ViewportProjectionMenu( const ViewportProjectionMenu& ) = delete;
        ViewportProjectionMenu( ViewportProjectionMenu&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ViewportProjectionMenu() = default;


        /***
         * 3. Setters
         ***/
    public slots:
        void setProjection( Projection projection );


        /***
         * 4. Operators
         ***/
    public:
        ViewportProjectionMenu& operator = ( const ViewportProjectionMenu& ) = delete;
        ViewportProjectionMenu& operator = ( ViewportProjectionMenu&& ) = delete;
};

} // namespace como

#endif // VIEWPORT_PROJECTION_MENU_HPP
