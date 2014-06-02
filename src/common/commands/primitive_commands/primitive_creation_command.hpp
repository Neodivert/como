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

namespace como {

/*!
 * \class PrimitiveCreationCommand
 *
 * \brief Command for creating a new primitive from a given file.
 */
class PrimitiveCreationCommand : public PrimitiveCommand
{
    private:
        /*! Primitive specification file. */
        PackableFile file_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        PrimitiveCreationCommand();

        /*!
         * \brief Constructor. Creates a PrimitiveCreationCommand from the
         * given specification file.
         * \param filePath path to the primitive's specification file.
         * \param userID ID of the user who created / shared the primitive.
         * \param drawableID Unique ID given to the new primitive.
         */
        PrimitiveCreationCommand( PackableString< NAME_SIZE > filePath, UserID userID, PrimitiveID primitiveID );

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

        /*! \brief Returns a pointer to the primitive's specification file. */
        const PackableFile* getFile() const ;


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
