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

#ifndef IMPORTED_PRIMITIVE_DATA_HPP
#define IMPORTED_PRIMITIVE_DATA_HPP

#include <common/primitives/primitive_data/primitive_data.hpp>
#include <common/primitives/primitive_data/triangles_group_with_material.hpp>
#include <list>


namespace como {

struct ImportedPrimitiveData : public PrimitiveData
{
    public:
        std::vector< TrianglesGroupWithMaterial > trianglesGroups_;


        /***
         * 1. File importing / exporting
         ***/
        virtual void importFromFile( const std::string &filePath );
        virtual void exportToFile( const std::string &filePath ) const;


    private:
        /***
         * 2. File reading
         ***/
        void readTrianglesGroups( std::ifstream& file );


        /***
         * 3. File writting
         ***/
        void writeTrianglesGroups( std::ofstream& file ) const;
};

} // namespace como

#endif // IMPORTED_PRIMITIVE_DATA_HPP
