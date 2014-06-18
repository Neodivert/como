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
#include <common/ids/resource_id.hpp>
#include <common/ids/primitive_id.hpp>
#include <common/managers/primitives/abstract_primitives_manager.hpp>
#include <list>

namespace como {

struct PrimitiveInfo
{
    std::string name;
    ResourceID category;
};

typedef std::list< std::pair< ResourceID, std::string > > CategoriesList;

class AbstractPrimitivesManager
{
    protected:
        std::string scenePrimitivesDir_;

        std::map< ResourceID, std::string > categoryNames_;
        std::map< PrimitiveID, PrimitiveInfo > primitiveInfo_;

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
         * 3. Getters
         ***/
        std::string getPrimitiveRelativePath( PrimitiveID id ) const;
        std::string getPrimitiveAbsolutePath( PrimitiveID id ) const;
        CategoriesList getCategoriesList() const;


        /***
         * 4. Categories management
         ***/
    protected:
        void registerCategory( ResourceID id, std::string name );
        void createCategory( ResourceID id, std::string name );


        /***
         * 5. Primitives management
         ***/
        void registerPrimitive( PrimitiveID id, std::string name, ResourceID category );


        /***
         * 6. Auxiliar methods
         ***/
        bool categoryNameInUse( std::string categoryName ) const;


        /***
         * 7. Operators
         ***/
        AbstractPrimitivesManager& operator = ( const AbstractPrimitivesManager& ) = delete;
        AbstractPrimitivesManager& operator = ( AbstractPrimitivesManager&& ) = delete;
};

} // namespace como

#endif // ABSTRACT_PRIMITIVES_MANAGER_HPP
