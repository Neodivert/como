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

#ifndef LIGHTS_LIST_ITEM_HPP
#define LIGHTS_LIST_ITEM_HPP

#include <QListWidgetItem>
#include <client/managers/lights/lights_manager.hpp>

namespace como {

class LightsListItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

    private:
        ResourceID lightID_;

    public:
        /***
         * 1. Construction
         ***/
        LightsListItem() = delete;
        LightsListItem( const ResourceID& id, const std::string& name );
        LightsListItem( const LightsListItem& ) = delete;
        LightsListItem( LightsListItem&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsListItem() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getResourceID() const;


        /***
         * 4. Operators
         ***/
        LightsListItem& operator = ( const LightsListItem& ) = delete;
        LightsListItem& operator = ( LightsListItem&& ) = delete;
};

} // namespace como

#endif // LIGHTS_LIST_ITEM_HPP
