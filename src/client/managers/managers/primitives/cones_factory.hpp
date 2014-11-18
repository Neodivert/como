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

#ifndef CONES_FACTORY_HPP
#define CONES_FACTORY_HPP

#include <client/managers/managers/primitives/specialized_system_primitives_factory.hpp>
#include <client/models/3d/meshes/system_mesh.hpp>
#include <common/commands/geometric_primitive_command/cone_creation_command.hpp>

namespace como {

class ConesFactory : public SpecializedSystemPrimitivesFactory
{
    public:
        /***
         * 1. Construction
         ***/
        ConesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager );
        ConesFactory() = delete;
        ConesFactory( const ConesFactory& ) = delete;
        ConesFactory( ConesFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ConesFactory() = default;


        /***
         * 3. Local cones creation
         ***/
        ResourceID createCone( float height, float radius, std::uint16_t nBaseVertices );


        /***
         * 4. Remote command execution
         ***/
        void executeRemoteCommand( const ConeCreationCommand& command );


        /***
         * 5. Operators
         ***/
        ConesFactory& operator = ( const ConesFactory& ) = delete;
        ConesFactory& operator = ( ConesFactory&& ) = delete;


    protected:
        /***
         * 6. Primitive data generation
         ***/
        /*
        virtual void generateVertexData( MeshVertexData &vertexData );
        virtual void generateUVData( MeshTextureData &uvData );
        virtual void generateTrianglesGroups( std::vector<NamedTrianglesGroup> &trianglesGroups );
        */
        virtual void generateVerticesPositionsAndUV(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &uvCoordinates);
        virtual void generateWalls(SystemPrimitiveData &primitiveData);


    private:
        /***
         * 7. Remote cones creation
         ***/
        glm::vec3 createCone( const ResourceID& coneID, const ResourceID& materialID, const ResourceID& firstTextureWallID, float height, float radius, std::uint16_t nBaseVertices );


    private:
        float coneHeight_;
        float coneRadius_;
        std::uint16_t coneNBaseVertices_;
};

} // namespace como

#endif // CONES_FACTORY_HPP
