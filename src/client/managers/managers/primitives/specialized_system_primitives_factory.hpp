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

#ifndef SPECIALIZED_SYSTEM_PRIMITIVES_FACTORY_HPP
#define SPECIALIZED_SYSTEM_PRIMITIVES_FACTORY_HPP

#include <client/managers/managers/meshes/meshes_manager.hpp>
#include <common/primitives/primitive_data/system_primitive_data.hpp>
#include <client/managers/managers/textures/texture_walls_manager.hpp>

namespace como {

// TODO: Combine methods generateVertexData() and generateUVData() into
// one method.
class SpecializedSystemPrimitivesFactory : public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        SpecializedSystemPrimitivesFactory( ServerInterfacePtr server, const std::string& primitiveName, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager );
        SpecializedSystemPrimitivesFactory() = delete;
        SpecializedSystemPrimitivesFactory( const SpecializedSystemPrimitivesFactory& ) = delete;
        SpecializedSystemPrimitivesFactory( SpecializedSystemPrimitivesFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SpecializedSystemPrimitivesFactory() = default;


        /***
         * 3. Operators
         ***/
        SpecializedSystemPrimitivesFactory& operator = ( const SpecializedSystemPrimitivesFactory& ) = delete;
        SpecializedSystemPrimitivesFactory& operator = ( SpecializedSystemPrimitivesFactory&& ) = delete;


    protected:
        /***
         * 4. SystemPrimitiveData generation
         ***/
        virtual SystemPrimitiveData generatePrimitiveData();
        virtual void generateVerticesPositionsAndUV( std::vector< glm::vec3 >& positions,
                                                     std::vector< glm::vec2 >& uvCoordinates ){ (void)( positions ); (void)( uvCoordinates ); }
        virtual void generateWalls( SystemPrimitiveData& primitiveData ){ (void)( primitiveData ); }
        virtual void generateOGLData( SystemPrimitiveData& primitiveData );


        /***
         * 5. Auxiliar methods
         ***/
        unsigned int generateHorizontalVerticesCircle(std::vector<glm::vec3> &vertices, float radius, unsigned int nDivisions, float height);
        unsigned int generateHorizontalUVCircle(std::vector<glm::vec2> &vertices, unsigned int nDivisions);
        void generateTrianglesCircle(std::vector<IndicesTriangle> &triangles, unsigned int nDivisions, unsigned int centerVertexIndex, unsigned int firstRadialVertexIndex, bool increaseIndices);


        const std::string primitiveName_;
        MeshesManagerPtr meshesManager_;
        TextureWallsManager* textureWallsManager_;
        MaterialsManagerPtr materialsManager_;

};

} // namespace como

#endif // SPECIALIZED_SYSTEM_PRIMITIVES_FACTORY_HPP
