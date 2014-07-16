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
#include <common/packables/packable_string.hpp>
#include <common/packables/packable_file.hpp>
#include <functional>

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

        /*! Primitive name. */
        PackableString name_;

        /*! Primitive specification files. */
        PackableFile primitiveFile_;


    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        PrimitiveCreationCommand() = delete;


        PrimitiveCreationCommand( const std::string& unpackingDirPath );

        PrimitiveCreationCommand( UserID userID, ResourceID primitiveID, PrimitiveInfo primitive, const std::string& unpackingDirPath );

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

        PrimitiveInfo getPrimitiveInfo() const;
    private:
        bool includesTexture() const;
    public:


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        PrimitiveCreationCommand& operator=( const PrimitiveCreationCommand& ) = delete;

        /*! \brief Move assignment operator */
        PrimitiveCreationCommand& operator=( PrimitiveCreationCommand&& ) = delete;


        /***
         * 5. Auxiliar methods
         ***/
    private:
        std::string getCurrentDateTimeStr() const;
};

} // namespace como

#endif // PRIMITIVE_CREATION_COMMAND_HPP
