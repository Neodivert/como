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
#include "materials_list_item.hpp"

namespace como {

// TODO: Can't use "private QListWidget", what interests me because I don't
// want user to use QListWidget methods here.
class MaterialsList : public QComboBox
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        MaterialsList() = delete;
        MaterialsList( MaterialsManagerPtr materialsManager );
        MaterialsList( const MaterialsList& ) = delete;
        MaterialsList( MaterialsList&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MaterialsList() = default;


        /***
         * 3. Operators
         ***/
        MaterialsList& operator = ( const MaterialsList& ) = delete;
        MaterialsList& operator = ( MaterialsList&& ) = delete;


        /***
         * 4. Popup
         ***/
        virtual void showPopup();

    private:
        MaterialsManagerPtr materialsManager_;
};

} // namespace como

#endif // MATERIALS_LIST_HPP
