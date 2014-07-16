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

#ifndef LIGHTS_LIST_HPP
#define LIGHTS_LIST_HPP

#include "lights_list_item.hpp"
#include <QListWidget>
#include <client/managers/drawables_manager/drawables_manager.hpp>
#include <map>

namespace como {

// TODO: Can't use "private QListWidget", what interests me because I don't
// want user to use QListWidget methods here.
class LightsList : public QListWidget
{
    Q_OBJECT

    private:
        LightsManagerPtr lightsManager_;

    public:
        /***
         * 1. Construction
         ***/
        LightsList() = delete;
        LightsList( LightsManagerPtr lightsManager );
        LightsList( const LightsList& ) = delete;
        LightsList( LightsList&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsList() = default;


        /***
         * 3. Operators
         ***/
        LightsList& operator = ( const LightsList& ) = delete;
        LightsList& operator = ( LightsList&& ) = delete;


        /***
         * 4. Slots
         ***/
    public slots:
        void addLight( LightID id, std::string name );
        void removeLight( PackableDrawableID id );


        /***
         * 5. Signals
         ***/
    signals:
        void lightSelected( LightID id );
};

} // namespace como

#endif // LIGHTS_LIST_HPP
