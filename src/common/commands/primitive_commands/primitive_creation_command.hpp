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

#ifndef PRIMITIVE_CREATION_COMMAND_HPP
#define PRIMITIVE_CREATION_COMMAND_HPP

#include "primitive_command.hpp"
#include <common/ids/resource_id.hpp>
#include <common/primitives/primitive_info.hpp>

namespace como {

/*!
 * \class PrimitiveCreationCommand
 *
 * \brief Command for creating a new primitive from a given file.
 */
class PrimitiveCreationCommand : public PrimitiveCommand
{
    private:
        /*! Primitive category ID. */
        PackableResourceID category_;

        /*! Primitive specification files. */
        PackableFile meshFile_;
        PackableFile materialFile_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        PrimitiveCreationCommand();

        PrimitiveCreationCommand( UserID userID, ResourceID primitiveID, ResourceID categoryID, PackableString< NAME_SIZE > meshFilePath, PackableString< NAME_SIZE > materialFilePath );

        /*! \brief Copy assignment operator */
        PrimitiveCreationCommand( const PrimitiveCreationCommand& );

        /*! \brief Move assignment operator */
        PrimitiveCreationCommand( PrimitiveCreationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor. */
        ~PrimitiveCreationCommand() = default;


        /***
         * 3. Getters
         ***/

        std::string getPrimitiveName() const;
        std::string getMeshFileName() const;
        std::string getMaterialFileName() const;

        ResourceID getCategoryID() const;

        /*! \brief Returns a pointer to the primitive's specification file. */
        const PackableFile* getMeshFile() const;

        const PackableFile* getMaterialFile() const;

        PrimitiveInfo getPrimitiveInfo() const;


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        PrimitiveCreationCommand& operator=( const PrimitiveCreationCommand& ) = delete;

        /*! \brief Move assignment operator */
        PrimitiveCreationCommand& operator=( PrimitiveCreationCommand&& ) = delete;
};

} // namespace como

#endif // PRIMITIVE_CREATION_COMMAND_HPP
