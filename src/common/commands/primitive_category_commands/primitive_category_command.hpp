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

#ifndef PRIMITIVE_CATEGORY_COMMAND_HPP
#define PRIMITIVE_CATEGORY_COMMAND_HPP

#include <common/commands/type_command.hpp>
#include <common/packables/packable_integer.hpp>
#include <common/packables/ids/packable_resource_id.hpp>

namespace como {

enum class PrimitiveCategoryCommandType : std::uint8_t {
    PRIMITIVE_CATEGORY_CREATION = 0
};

typedef PackableUint8< PrimitiveCategoryCommandType > PackablePrimitiveCategoryCommandType;

class PrimitiveCategoryCommand : public TypeCommand< PrimitiveCategoryCommandType >
{
    private:
        PackableResourceID categoryID_;

    public:
        /***
         * 1. Construction
         ***/
        PrimitiveCategoryCommand() = delete;
        PrimitiveCategoryCommand( UserID userID, ResourceID categoryID, PrimitiveCategoryCommandType commandType );
        PrimitiveCategoryCommand( const PrimitiveCategoryCommand& ) = delete;
        PrimitiveCategoryCommand( PrimitiveCategoryCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~PrimitiveCategoryCommand() = default;


        /***
         * 3. Getters
         ***/
        ResourceID getCategoryID() const;


        /***
         * 4. Operators
         ***/
        PrimitiveCategoryCommand& operator = ( const PrimitiveCategoryCommand& ) = delete;
        PrimitiveCategoryCommand& operator = ( PrimitiveCategoryCommand&& ) = delete;
};

DEFINE_SHARED_POINTERS( PrimitiveCategoryCommand,
                        PrimitiveCategoryCommandPtr,
                        PrimitiveCategoryCommandConstPtr )

} // namespace como

#endif // PRIMITIVE_CATEGORY_COMMAND_HPP
