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

#include "primitive_category_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCategoryCreationCommand::PrimitiveCategoryCreationCommand() :
    PrimitiveCategoryCommand( NO_USER, 0, PrimitiveCategoryCommandType::PRIMITIVE_CATEGORY_CREATION ),
    categoryName_( "Unnamed category" )
{
    addPackable( &categoryName_ );
}


PrimitiveCategoryCreationCommand::PrimitiveCategoryCreationCommand( UserID userID, PrimitiveCategoryID categoryID, std::string categoryName ) :
    PrimitiveCategoryCommand( userID, categoryID, PrimitiveCategoryCommandType::PRIMITIVE_CATEGORY_CREATION ),
    categoryName_( categoryName.c_str() )
{
    addPackable( &categoryName_ );
}

} // namespace como
