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

#ifndef ABSTRACT_PRIMITIVES_MANAGER_HPP
#define ABSTRACT_PRIMITIVES_MANAGER_HPP

#include <string>
#include <map>
#include <common/utilities/log.hpp>
#include <common/ids/primitive_category_id.hpp>
#include <common/managers/primitives/abstract_primitives_manager.hpp>

namespace como {

class AbstractPrimitivesManager
{
    protected:
        std::string scenePrimitivesDir_;

        std::map< PrimitiveCategoryID, std::string > categoryNames_;

        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        AbstractPrimitivesManager() = delete;
        AbstractPrimitivesManager( std::string sceneName, LogPtr log );
        AbstractPrimitivesManager( const AbstractPrimitivesManager& ) = delete;
        AbstractPrimitivesManager( AbstractPrimitivesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~AbstractPrimitivesManager() = default;


        /***
         * 3. Categories management
         ***/
    protected:
        void registerCategory( PrimitiveCategoryID id, std::string name );
        void createCategory( PrimitiveCategoryID id, std::string name );


        /***
         * 4. Auxiliar methods
         ***/
        bool categoryNameInUse( std::string categoryName ) const;


        /***
         * 5. Operators
         ***/
        AbstractPrimitivesManager& operator = ( const AbstractPrimitivesManager& ) = delete;
        AbstractPrimitivesManager& operator = ( AbstractPrimitivesManager&& ) = delete;
};

} // namespace como

#endif // ABSTRACT_PRIMITIVES_MANAGER_HPP
