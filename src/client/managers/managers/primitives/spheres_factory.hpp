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

#ifndef SPHERES_FACTORY_HPP
#define SPHERES_FACTORY_HPP

#include <client/managers/managers/primitives/specialized_system_primitives_factory.hpp>
#include <client/models/3d/meshes/system_mesh.hpp>
#include <common/commands/geometric_primitive_command/sphere_creation_command.hpp>

namespace como {

class SpheresFactory : public SpecializedSystemPrimitivesFactory
{
    public:
        /***
         * 1. Construction
         ***/
        SpheresFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager );
        SpheresFactory() = delete;
        SpheresFactory( const SpheresFactory& ) = delete;
        SpheresFactory( SpheresFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SpheresFactory() = default;


        /***
         * 3. Local spheres creation
         ***/
        ResourceID createSphere( float radius, std::uint16_t nDivisions );


        /***
         * 4. Remote command execution
         ***/
        void executeRemoteCommand( const SphereCreationCommand& command );


        /***
         * 5. Operators
         ***/
        SpheresFactory& operator = ( const SpheresFactory& ) = delete;
        SpheresFactory& operator = ( SpheresFactory&& ) = delete;


    protected:
        /***
         * 6. Primitive data generation
         ***/
        virtual void generateVerticesPositionsAndUV(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &uvCoordinates);
        virtual void generateWalls(SystemPrimitiveData &primitiveData);


    private:
        /***
         * 7. Remote spheres creation
         ***/
        glm::vec3 createSphere( const ResourceID& sphereID, const ResourceID& materialID, const ResourceID& firstTextureWallID, float radius, std::uint16_t nDivisions );


        /***
         * 8. Auxiliar methods
         ***/
        void generateHorizontalCircleVertices( std::vector< glm::vec3 >& vertices, float radius, unsigned int nDivisions, float y );
        void generateHorizontalCircleUV(std::vector<glm::vec2> &uvCoordinates, unsigned int nDivisions, float y);
        void generateTriangles( SystemPrimitiveData& primitiveData, unsigned int firstCircleIndex, unsigned int nDivisions );
        void generatePositionTriangles( std::vector< IndicesTriangle >& triangles, unsigned int firstCircleIndex, unsigned int nDivisions );
        void generateUVTriangles( std::vector< IndicesTriangle >& triangles, unsigned int firstCircleIndex, unsigned int nDivisions );
        unsigned int nExpectedVertices() const;
        unsigned int nExpectedTriangles() const;

    private:
        float sphereRadius_;
        std::uint16_t sphereNDivisions_;
};

} // namespace como

#endif // SPHERES_FACTORY_HPP
