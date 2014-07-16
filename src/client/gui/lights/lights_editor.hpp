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

#ifndef LIGHTS_EDITOR_HPP
#define LIGHTS_EDITOR_HPP

#include <QFrame>
//#include <client/managers/lights/lights_manager.hpp>
#include "lights_list.hpp"

namespace como {

// FIXME: This class and MaterialsEditor can share some base code.
class LightsEditor : public QFrame
{
    Q_OBJECT

    private:
        LightsManagerPtr lightsManager_;
        LightsList* lightsList_;


    public:
        /***
         * 1. Construction
         ***/
        LightsEditor( LightsManagerPtr lightsManager );
        LightsEditor( const LightsEditor& ) = delete;
        LightsEditor( LightsEditor&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsEditor() = default;


        /***
         * 3. Events
         ***/
        virtual void enterEvent( QEvent * event );
        virtual void leaveEvent( QEvent * event );


        /***
         * 4. Operators
         ***/
        LightsEditor& operator = ( const LightsEditor& ) = delete;
        LightsEditor& operator = ( LightsEditor&& ) = delete;
};

} // namespace como

#endif // LIGHTS_EDITOR_HPP
