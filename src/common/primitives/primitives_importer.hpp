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

#ifndef PRIMITIVES_IMPORTER_HPP
#define PRIMITIVES_IMPORTER_HPP

#include <string>

namespace como {

class PrimitivesImporter {
    public:
        /***
         * 1. Construction
         ***/
        PrimitivesImporter() = default;
        PrimitivesImporter( PrimitivesImporter& ) = delete;
        PrimitivesImporter( PrimitivesImporter&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~PrimitivesImporter() = default;


        /***
         * 3. Primitives import
         ***/

        /*!
         * \brief Rename the file <filePath> and the files referenced by it as
         * <name>.original_extension and copy them all to <destDirectory>.
         */
        virtual void importPrimitive( std::string name, std::string filePath, std::string destDirectory ) = 0;


        /***
         * 4. Operators
         ***/
        PrimitivesImporter& operator = ( const PrimitivesImporter& ) = delete;
        PrimitivesImporter& operator = ( PrimitivesImporter&& ) = delete;
};

}

#endif // PRIMITIVES_IMPORTER_HPP
