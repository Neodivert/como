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

#ifndef VIEW_PORT_TOOLBAR_HPP
#define VIEW_PORT_TOOLBAR_HPP

#include <client/gui/viewport.hpp>
#include <QToolBar>

namespace como {

class ViewportToolBar : public QToolBar
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        ViewportToolBar() = delete;
        ViewportToolBar( Viewport* viewport, View currentView );
        ViewportToolBar( const ViewportToolBar& ) = delete;
        ViewportToolBar( ViewportToolBar&& ) = delete;


        /***
         * 2. Initialization
         ***/
        QComboBox* createViewSelector( Viewport* viewport, View currentView ) const;


        /***
         * 3. Destruction
         ***/
        ~ViewportToolBar() = default;


        /***
         * 4. Operators
         ***/
        ViewportToolBar& operator = ( const ViewportToolBar& ) = delete;
        ViewportToolBar& operator = ( ViewportToolBar&& ) = delete;


        /***
         * 5. Signals
         ***/
    signals:
        void viewFrameMaximizationRequested();
        void viewFrameMinimizationRequested();
};

} // namespace como

#endif // VIEW_PORT_TOOLBAR_HPP
