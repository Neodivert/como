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

#ifndef VIEW_FRAME_H
#define VIEW_FRAME_H

#include <QFrame>
#include "viewport.hpp"
#include "../managers/scene/scene.hpp"

namespace como {

class ViewFrame : public QFrame
{
    private:
        // OpenGL rendering surface.
        Viewport* viewport;
        QComboBox* viewSelector;
        QGroupBox* projectionModeSwitch;

    public:
        /***
         * 1. Initialization
         ***/
        ViewFrame() = delete;
        ViewFrame( const ViewFrame& ) = delete;
        ViewFrame( ViewFrame&& ) = delete;
        ViewFrame( View view, shared_ptr< ComoApp > comoApp );
        QGroupBox* createProjectionSwitch();

        ~ViewFrame() = default;


        /***
         * 2. Operators
         ***/
        ViewFrame& operator = ( const ViewFrame& ) = delete;
        ViewFrame& operator = ( ViewFrame&& ) = delete;


        /***
         * 3. Updating and drawing
         ***/
    public slots:
        virtual void render();
};

} // namespace como

#endif // VIEW_FRAME_H
