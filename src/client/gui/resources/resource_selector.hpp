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

#ifndef RESOURCE_SELECTOR_HPP
#define RESOURCE_SELECTOR_HPP

#include <QComboBox>
#include <vector>
#include <string>
#include <common/ids/resource_id.hpp>
#include <common/managers/primitives/abstract_primitives_manager.hpp> // CategoriesList type

namespace como {

class ResourceSelector : public QComboBox
{
    Q_OBJECT

    private:
        // Vector with the IDs of all the resources given as arguments in
        // constructor. This acts as a map between the index given to the
        // resource in this dropdown list (i) and its ID (v[i]).
        std::vector< ResourceID > resourcesIDs_;

    public:
        /***
         * 1. Construction
         ***/
        ResourceSelector();
        ResourceSelector( const ResourcesList resourcesList );
        ResourceSelector( const ResourceSelector& ) = delete;
        ResourceSelector( ResourceSelector&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourceSelector() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getCurrentResourceID() const;


        /***
         * 4. Items management
         ***/
        void insertResource( ResourceID id, std::string name );


        /***
         * 5. Operators
         ***/
        ResourceSelector& operator = ( const ResourceSelector& ) = delete;
        ResourceSelector& operator = ( ResourceSelector&& ) = delete;


        /***
         * 6. Signals
         ***/
    signals:
        void resourceSelected( ResourceID resourceID );
};

} // namespace como

#endif // RESOURCE_SELECTOR_HPP
