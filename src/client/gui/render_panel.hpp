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

#ifndef RENDER_PANEL_HPP
#define RENDER_PANEL_HPP

#include "view_frame.hpp"
#include <client/managers/como_app.hpp>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QSplitter)

namespace como {

class RenderPanel : public QFrame
{
    Q_OBJECT

    private:
        // Pointer to app's current state.
        shared_ptr< ComoApp > comoApp;

        //
        ViewFrame* viewFrames_[4];

    public:
        /***
         * 1. Initialization
         ***/
        RenderPanel() = delete;
        RenderPanel( const RenderPanel& ) = delete;
        RenderPanel( RenderPanel&& ) = delete;
        explicit RenderPanel( QWidget *parent, shared_ptr< ComoApp > comoApp );

        ~RenderPanel();


        /***
         * 2. Operators
         ***/
        RenderPanel& operator = ( const RenderPanel& ) = delete;
        RenderPanel& operator = ( RenderPanel&& ) = delete;


        /***
         * 3. Slots
         ***/
    public slots:
        void renderIfNeeded();
};

} // namespace como

#endif // RENDER_PANEL_HPP
