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

#ifndef MATERIALS_LIST_HPP
#define MATERIALS_LIST_HPP

#include <QComboBox>
#include <client/managers/managers/materials/materials_manager.hpp>
#include <map>

namespace como {

// TODO: Inherit from / replace by ResourcesDropdownList class.
// TODO: Can't use "private QListWidget", what interests me because I don't
// want user to use QListWidget methods here.
class MaterialsListWidget : public QComboBox
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        MaterialsListWidget() = delete;
        MaterialsListWidget( MaterialsManagerPtr materialsManager );
        MaterialsListWidget( const MaterialsListWidget& ) = delete;
        MaterialsListWidget( MaterialsListWidget&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialsListWidget() = default;


        /***
         * 3. Operators
         ***/
        MaterialsListWidget& operator = ( const MaterialsListWidget& ) = delete;
        MaterialsListWidget& operator = ( MaterialsListWidget&& ) = delete;


        /***
         * 4. Populating
         ***/
        void populate();


        /***
         * 5. Signals
         ***/
    signals:
        void materialSelected( MaterialHandlerPtr materialHandler );


    private:
        MaterialsManagerPtr materialsManager_;

        // FIXME: This is wrong because when removing materials from
        // QComboBox, indices of the rest of materiales are updated (I think).
        std::map< int, ResourceID > indexToID_;
};

} // namespace como

#endif // MATERIALS_LIST_HPP
