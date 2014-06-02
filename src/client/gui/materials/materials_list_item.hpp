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

#ifndef MATERIALS_LIST_ITEM_HPP
#define MATERIALS_LIST_ITEM_HPP

#include <QListWidgetItem>
#include <common/ids/material_id.hpp>

namespace como {

class MaterialsListItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

    private:
        MaterialID materialID_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialsListItem() = delete;
        MaterialsListItem( const MaterialID& id, const std::string& name );
        MaterialsListItem( const MaterialsListItem& ) = delete;
        MaterialsListItem( MaterialsListItem&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialsListItem() = default;


        /***
         * 3. Getters
         ***/
        MaterialID getMaterialID() const;


        /***
         * 4. Operators
         ***/
        MaterialsListItem& operator = ( const MaterialsListItem& ) = delete;
        MaterialsListItem& operator = ( MaterialsListItem&& ) = delete;
};

} // namespace como

#endif // MATERIALS_LIST_ITEM_HPP
