#include "cubes_factory.hpp"

namespace como {


/***
 * 1. Construction
 ***/

CubesFactory::CubesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager ) :
    SpecializedSystemPrimitivesFactory( server, meshesManager, materialsManager, textureWallsManager )
{}


/***
 * 2. Destruction
 ***/

ResourceID CubesFactory::createCube( float width, float height, float depth )
{
    ResourceID cubeID = reserveResourceIDs( 1 );
    ResourceID materialID = reserveResourceIDs( 1 );

    createCube( cubeID, materialID, width, height, depth );

    sendCommandToServer(
                CommandConstPtr(
                    new CubeCreationCommand( cubeID,
                                             materialID,
                                             width,
                                             height,
                                             depth ) ) );

    return cubeID;
}


/***
 * 4. Command execution
 ***/

void CubesFactory::executeRemoteCommand( const CubeCreationCommand &command )
{
    createCube( command.getMeshID(),
                command.getMaterialID(),
                command.getCubeWidth(),
                command.getCubeHeight(),
                command.getCubeDepth() );
}


/***
 * 6. Remote cubes creation
 ***/

void CubesFactory::createCube( const ResourceID& cubeID, const ResourceID& materialID, float width, float height, float depth )
{
    std::unique_ptr< Mesh > cube;

    width_ = width;
    height_ = height;
    depth_ = depth;

    materialsManager_->createMaterial( MaterialInfo(), materialID, cubeID );

    cube = std::unique_ptr< Mesh >( new SystemMesh( cubeID, generatePrimitiveData(), materialsManager_->getMaterial( materialID ) ) );

    return meshesManager_->addMesh( std::move( cube ), cubeID );
}


/***
 * 7. SystemPrimitiveData generation
 ***/

void CubesFactory::generateVertexData( MeshVertexData &vertexData )
{
    // Front face (vertices)
    vertexData.vertices.push_back( glm::vec3( width_ / 2.0f, height_ / 2.0f, depth_ / 2.0f ) );
    vertexData.vertices.push_back( glm::vec3( -width_ / 2.0f, height_ / 2.0f, depth_ / 2.0f ) );
    vertexData.vertices.push_back( glm::vec3( -width_ / 2.0f, -height_ / 2.0f, depth_ / 2.0f ) );
    vertexData.vertices.push_back( glm::vec3( width_ / 2.0f, -height_ / 2.0f, depth_ / 2.0f ) );

    // Back face (vertices)
    vertexData.vertices.push_back( glm::vec3( width_ / 2.0f, height_ / 2.0f, -depth_ / 2.0f ) );
    vertexData.vertices.push_back( glm::vec3( -width_ / 2.0f, height_ / 2.0f, -depth_ / 2.0f ) );
    vertexData.vertices.push_back( glm::vec3( -width_ / 2.0f, -height_ / 2.0f, -depth_ / 2.0f ) );
    vertexData.vertices.push_back( glm::vec3( width_ / 2.0f, -height_ / 2.0f, -depth_ / 2.0f ) );

    // Front face (triangles)
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 0, 2, 3 } );
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 0, 1, 2 } );

    // Right face (triangles)
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 3, 7 } );
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 0, 3 } );

    // Left face (triangles)
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 1, 6, 2 } );
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 1, 5, 6 } );

    // Back face (triangles)
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 5, 7, 6 } );
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 5, 4, 7 } );

    // Top face (triangles)
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 1, 0 } );
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 4, 5, 1 } );

    // Bottom face (triangles)
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 3, 6, 7 } );
    vertexData.vertexTriangles.push_back( IndicesTriangle{ 3, 2, 6 } );
}


void CubesFactory::generateUVData( MeshTextureData &uvData )
{
    const unsigned int N_CUBE_FACES = 6;
    unsigned int cubeFaceIndex;

    // UV - Top right
    uvData.uvVertices.push_back( glm::vec2( 1.0f, 1.0f ) );

    // UV - Top left
    uvData.uvVertices.push_back( glm::vec2( 0.0f, 1.0f ) );

    // UV - Bottom left
    uvData.uvVertices.push_back( glm::vec2( 0.0f, 0.0f ) );

    // UV - Botom right
    uvData.uvVertices.push_back( glm::vec2( 1.0f, 0.0f ) );

    // UV triangles
    for( cubeFaceIndex = 0; cubeFaceIndex < N_CUBE_FACES; cubeFaceIndex++ ){
        uvData.uvTriangles.push_back( IndicesTriangle{ 0, 2, 3 } );
        uvData.uvTriangles.push_back( IndicesTriangle{ 0, 1, 2 } );
    }
}


void CubesFactory::generateTrianglesGroups( std::vector<NamedTrianglesGroup> &trianglesGroups )
{
    unsigned int cubeFaceIndex;
    const char textureWallsNames[][32] =
    {
        "Front face",
        "Right face",
        "Left face",
        "Back face",
        "Top face",
        "Bottom face"
    };

    trianglesGroups.clear();
    for( cubeFaceIndex = 0; cubeFaceIndex < 6; cubeFaceIndex++ ){
        trianglesGroups.push_back(
                    NamedTrianglesGroup(
                        textureWallsNames[cubeFaceIndex],
                        cubeFaceIndex * 2,
                        2 ) );
    }
}

} // namespace como
