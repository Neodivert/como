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
#include <common/primitives/primitive_info.hpp>
#include <boost/filesystem.hpp>

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
        virtual ~PrimitivesImporter() = default;


        /***
         * 3. Primitives import
         ***/

        /*!
         * \brief Import a primitive from file <filePath> and save it on
         * <dstDirectory> by adding a <nameSuffix> suffix to its final name.
         */
        virtual PrimitiveInfo importPrimitive( std::string srcFilePath, std::string dstDirectory, std::string nameSuffix) = 0;


        /***
         * 4. Operators
         ***/
        PrimitivesImporter& operator = ( const PrimitivesImporter& ) = delete;
        PrimitivesImporter& operator = ( PrimitivesImporter&& ) = delete;
};

}

#endif // PRIMITIVES_IMPORTER_HPP
