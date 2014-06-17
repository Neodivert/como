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

#ifndef PRIMITIVE_CATEGORY_CREATION_COMMAND_HPP
#define PRIMITIVE_CATEGORY_CREATION_COMMAND_HPP

#include "primitive_category_command.hpp"

namespace como {

class PrimitiveCategoryCreationCommand : public PrimitiveCategoryCommand
{
    private:
        PackableString<64> categoryName_;

    public:
        /***
         * 1. Construction
         ***/
        PrimitiveCategoryCreationCommand(); // TODO: Remove this constructor in all base commands (but CommandsList needs it...).
        PrimitiveCategoryCreationCommand( UserID userID, PrimitiveCategoryID categoryID, std::string categoryName );
        PrimitiveCategoryCreationCommand( const PrimitiveCategoryCreationCommand& ) = delete;
        PrimitiveCategoryCreationCommand( PrimitiveCategoryCreationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~PrimitiveCategoryCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        std::string getCategoryName() const;


        /***
         * 4. Operators
         ***/
        PrimitiveCategoryCreationCommand& operator = ( const PrimitiveCategoryCreationCommand& ) = delete;
        PrimitiveCategoryCreationCommand& operator = ( PrimitiveCategoryCreationCommand&& ) = delete;
};

DEFINE_SHARED_POINTERS( PrimitiveCategoryCreationCommand,
                        PrimitiveCategoryCreationCommandPtr,
                        PrimitiveCategoryCreationCommandConstPtr )

} // namespace como

#endif // PRIMITIVE_CATEGORY_CREATION_COMMAND_HPP
