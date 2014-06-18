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

#ifndef PRIMITIVE_CATEGORY_SELECTOR_HPP
#define PRIMITIVE_CATEGORY_SELECTOR_HPP

#include <vector>
#include <string>
#include <common/ids/resource_id.hpp>
#include <QComboBox>
#include <common/managers/primitives/abstract_primitives_manager.hpp> // CategoriesList type

namespace como {

class PrimitiveCategorySelector : public QComboBox
{
    private:
        // Vector with the IDs of all the categories given as arguments in
        // constructor. This acts as a map between the index given to the
        // category in this dropdown list (i) and its ID (v[i]).
        std::vector< ResourceID > categoriesIDs_;

    public:
        /***
         * 1. Construction
         ***/
        PrimitiveCategorySelector() = delete;
        PrimitiveCategorySelector( const CategoriesList categoriesList );
        PrimitiveCategorySelector( const PrimitiveCategorySelector& ) = delete;
        PrimitiveCategorySelector( PrimitiveCategorySelector&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~PrimitiveCategorySelector() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getCurrentCategoryID() const;


        /***
         * 4. Operators
         ***/
        PrimitiveCategorySelector& operator = ( const PrimitiveCategorySelector& ) = delete;
        PrimitiveCategorySelector& operator = ( PrimitiveCategorySelector&& ) = delete;
};

} // namespace como

#endif // PRIMITIVE_CATEGORY_SELECTOR_HPP
