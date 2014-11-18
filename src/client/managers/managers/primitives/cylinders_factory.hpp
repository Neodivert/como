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

#ifndef CYLINDERS_FACTORY_HPP
#define CYLINDERS_FACTORY_HPP

#include <client/managers/managers/primitives/specialized_system_primitives_factory.hpp>
#include <client/models/3d/meshes/system_mesh.hpp>
#include <common/commands/geometric_primitive_command/cylinder_creation_command.hpp>

namespace como {

class CylindersFactory : public SpecializedSystemPrimitivesFactory
{
    public:
        /***
         * 1. Construction
         ***/
        CylindersFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager );
        CylindersFactory() = delete;
        CylindersFactory( const CylindersFactory& ) = delete;
        CylindersFactory( CylindersFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~CylindersFactory() = default;


        /***
         * 3. Local cylinders creation
         ***/
        ResourceID createCylinder( float height, float radius, std::uint16_t nRadialVertices );


        /***
         * 4. Remote command execution
         ***/
        void executeRemoteCommand( const CylinderCreationCommand& command );


        /***
         * 5. Operators
         ***/
        CylindersFactory& operator = ( const CylindersFactory& ) = delete;
        CylindersFactory& operator = ( CylindersFactory&& ) = delete;


    protected:
        /***
         * 6. Primitive data generation
         ***/
        virtual void generateVerticesPositionsAndUV(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &uvCoordinates);
        virtual void generateWalls(SystemPrimitiveData &primitiveData);


    private:
        /***
         * 7. Remote cylinders creation
         ***/
        glm::vec3 createCylinder( const ResourceID& cylinderID, const ResourceID& materialID, const ResourceID& firstTextureWallID, float height, float radius, std::uint16_t nRadialVertices );


    private:
        float cylinderHeight_;
        float cylinderRadius_;
        std::uint16_t cylinderNRadialVertices_;
};

} // namespace como

#endif // CYLINDERS_FACTORY_HPP
