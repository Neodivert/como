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

#include "obj_primitives_importer.hpp"
#include <fstream>
#include <stdexcept>
#include <common/exceptions/file_not_open_exception.hpp>
#include <common/primitives/primitive_file.hpp>
#include <common/primitives/primitive_data/imported_primitive_data.hpp>
#include <map>
#include <array>

namespace como {

/***
 * 3. Primitives import
 ***/

PrimitiveInfo OBJPrimitivesImporter::importPrimitive( std::string srcFilePath, std::string dstDirectory, std::string nameSuffix )
{
    ImportedPrimitiveData primitiveData;
    PrimitiveInfo primitiveInfo;

    processMeshFile( srcFilePath, primitiveInfo, primitiveData );

    primitiveInfo.name += nameSuffix;
    primitiveInfo.filePath =
            dstDirectory + '/' +
            primitiveInfo.name +
            ".prim";

    // TODO: Remove this?
    primitiveData.exportToFile( primitiveInfo.filePath );

    return primitiveInfo;
}


void OBJPrimitivesImporter::processMeshFile( std::string filePath, PrimitiveInfo& primitiveInfo, ImportedPrimitiveData& primitiveData )
{
    std::ifstream file;
    std::string fileLine;

    // Set '.' as the float separator (for parsing floats from a text
    // line).
    setlocale( LC_NUMERIC, "C" );

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    while( !file.eof() ){
        readLine( file, fileLine );

        processMeshFileLine( filePath, fileLine, primitiveInfo, primitiveData );
    }

    file.close();

    if( ( primitiveData.normalData.normals.size() == 0 ) ||
            ( primitiveData.normalData.normalTriangles.size() == 0 )  ){
        computeNormalData( primitiveData.vertexData, primitiveData.normalData );
    }

    generateOGLData( primitiveData );
}

void OBJPrimitivesImporter::processMeshFileLine( std::string filePath, std::string line, PrimitiveInfo& primitiveInfo, ImportedPrimitiveData& primitiveData )
{
    std::string lineHeader;
    std::string lineBody;

    if( !line.size() ){
        return;
    }

    splitFileLine( line, lineHeader, lineBody );

    if( lineHeader == "o" ){
        primitiveInfo.name = lineBody;
    }else if( lineHeader == "g" ){
        // TODO: Create a new triangles group?
    }else if( lineHeader == "v" ){
        glm::vec3 vertex;

        // Extract the vertex from the line and add it to the Mesh.
        sscanf( lineBody.c_str(), "%f %f %f", &vertex[0], &vertex[1], &vertex[2] );
        primitiveData.vertexData.vertices.push_back( vertex );
    }else if( lineHeader == "vt" ){
        glm::vec2 textureCoordinates;

        // Extract the UV coordinates from the line and add it to the Mesh.
        sscanf( lineBody.c_str(), "%f %f", &textureCoordinates[0], &textureCoordinates[1] );

        // Invert Y component. (TODO: Move this computation to another place?)
        textureCoordinates.y = 1.0f - textureCoordinates.y;
        primitiveData.uvData.uvVertices.push_back( textureCoordinates );
    }else if( lineHeader == "vn" ){
        glm::vec3 normal;

        // Extract the normal from the line and add it to the Mesh.
        sscanf( lineBody.c_str(), "%f %f %f", &normal[0], &normal[1], &normal[2] );
        primitiveData.normalData.normals.push_back( glm::normalize( normal ) );

    }else if( lineHeader == "f" ){
        if( primitiveData.trianglesGroups_.size() == 0 ){
            // A face has been specified without defining its associated
            // material, so create a default material and a default
            // triangles group and join them.
            primitiveData.materialsInfo_.push_back( MaterialInfo() );

            TrianglesGroupWithMaterial newTrianglesGroup;
            newTrianglesGroup.materialIndex = 0;

            primitiveData.trianglesGroups_.push_back( newTrianglesGroup );
        }

        switch( getFaceType( lineBody ) ){
            case FaceType::TRIANGLE:
                processTriangleFaceStr( lineBody, primitiveData );
            break;
            case FaceType::QUAD:
                processQuadFaceStr( lineBody, primitiveData );
            break;
            case FaceType::OTHER:
                throw std::runtime_error( "OBJ primitive importer can't process faces others than triangles and quads" );
            break;
        }
    }else if( lineHeader == "mtllib" ){
        std::string fileDirectory = filePath.substr( 0, filePath.rfind( '/' ) );
        std::string materialFilePath = fileDirectory + '/' + lineBody;

        processMaterialFile( materialFilePath, primitiveData.oglData.includesTextures, primitiveData.materialsInfo_ );
    }else if( lineHeader == "usemtl" ){
        TrianglesGroupWithMaterial newTrianglesGroup;
        if( primitiveData.trianglesGroups_.size() ){
            newTrianglesGroup.firstTriangleIndex =
                    primitiveData.trianglesGroups_.back().firstTriangleIndex +
                    primitiveData.trianglesGroups_.back().nTriangles;
        }
        newTrianglesGroup.materialIndex = primitiveData.getMaterialIndex( lineBody );

        primitiveData.trianglesGroups_.push_back( newTrianglesGroup );
    }
}


void OBJPrimitivesImporter::computeNormalData( const MeshVertexData& meshVertexData, MeshNormalData& normalData )
{
    normalData.initFromMeshVertexData( meshVertexData );
}


void OBJPrimitivesImporter::processMaterialFile( std::string filePath, bool &includesTextures, MaterialsInfoVector &materials )
{
    std::ifstream file;
    std::string fileLine;
    includesTextures = false;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    while( !file.eof() ){
        readLine( file, fileLine );

        processMaterialFileLine( filePath, fileLine, materials, includesTextures );
    }

    file.close();
}


void OBJPrimitivesImporter::processMaterialFileLine( std::string filePath, std::string fileLine, MaterialsInfoVector& materials, bool &includesTextures )
{
    std::string lineHeader;
    std::string lineBody;

    splitFileLine( fileLine, lineHeader, lineBody );

    if( !fileLine.size() ){
        return;
    }

    // Retrieve material parameters from file.
    if( lineHeader == "newmtl" ){
        MaterialInfo newMaterial;
        newMaterial.name = lineBody;
        materials.push_back( newMaterial );
    }else if( lineHeader == "Ka" ){
        materials.back().ambientReflectivity = PrimitiveFile::readVec3( lineBody );
    }else if( lineHeader == "Kd" ){
        materials.back().diffuseReflectivity = PrimitiveFile::readVec3( lineBody );
    }else if( lineHeader == "Ks" ){
        materials.back().specularReflectivity = PrimitiveFile::readVec3( lineBody );
    }else if( lineHeader == "Ns" ){
        materials.back().specularExponent = std::atof( lineBody.c_str() );
    }else if( lineHeader == "map_Kd" ){
        includesTextures = true;
        std::string textureFilePath = filePath.substr( 0, filePath.rfind( '/' ) ) + '/' + lineBody;
        processTextureFile( textureFilePath, materials.back().textureInfo );
    }
}


void OBJPrimitivesImporter::processTextureFile( std::string filePath, std::unique_ptr<TextureInfo>& textureInfo )
{
    std::ifstream file;
    unsigned int imageFileSize = 0;

    if( !supportedImageFile( filePath ) ){
        throw std::runtime_error( "Texture image extension not supported (" +
                                  filePath +
                                  ")" );
    }

    file.open( filePath, std::ios_base::binary );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    textureInfo = std::unique_ptr< TextureInfo >( new TextureInfo );

    // Retrieve the size of the texture image file.
    file.seekg( 0, file.end );
    imageFileSize = file.tellg();
    file.seekg( 0, file.beg );

    textureInfo->imageFileData.resize( imageFileSize );
    file.read( &( textureInfo->imageFileData[0] ), imageFileSize );

    file.close();
}


/***
 * 5. Auxiliar methods
 ***/

void OBJPrimitivesImporter::readLine( std::ifstream &file, std::string &fileLine )
{
    std::getline( file, fileLine );

    if( fileLine.size() && ( fileLine[ fileLine.size() - 1 ] == '\r' ) ){
        fileLine = fileLine.substr( 0, fileLine.size() - 1 );
    }
}


FaceType OBJPrimitivesImporter::getFaceType( const std::string &faceBody )
{
    int nSpaces = std::count( faceBody.begin(), faceBody.end(), ' ' );

    if( nSpaces == 2 ){
        return FaceType::TRIANGLE;
    }else if( nSpaces == 3 ){
        return FaceType::QUAD;
    }else{
        return FaceType::OTHER;
    }
}


FaceComponents OBJPrimitivesImporter::getFaceComponents( const std::string &faceDefinition )
{
    std::string firstTriangleVertexStr = faceDefinition.substr( 0, faceDefinition.find( ' ' ) );
    size_t firstSlashPosition = std::string::npos;
    size_t secondSlashPosition = std::string::npos;

    firstSlashPosition = faceDefinition.find( '/' );

    if( firstSlashPosition == std::string::npos ){
        // No slashes found. Face defines only vertex positions.
        // Vertex: %u
        return FaceComponents::ONLY_VERTICES;
    }else{
        secondSlashPosition = firstTriangleVertexStr.find( '/', firstSlashPosition + 1 );

        if( secondSlashPosition == std::string::npos ){
            // Only one slash found per face vertex. Face defines vertex
            // position and UV coordinates.
            // Vertex: %u/%u
            return FaceComponents::VERTICES_AND_UVS;
        }else{
            if( secondSlashPosition != (firstSlashPosition + 1) ){
                // Vertex: %u/%u/%u
                return FaceComponents::VERTICES_NORMALS_AND_UVS;
            }else{
                // Vertex: %u//%u
                return FaceComponents::VERTICES_AND_NORMALS;
            }
        }
    }
}


void OBJPrimitivesImporter::processTriangleFaceStr( const std::string &lineBody, ImportedPrimitiveData& primitiveData )
{
    std::array< GLuint, 3 > vertexTriangle = { 0 };
    std::array< GLuint, 3 > uvTriangle = { 0 };
    std::array< GLuint, 3 > normalTriangle = { 0 };
    int componentsRead;
    int i;

    switch( getFaceComponents( lineBody ) ){
        case FaceComponents::ONLY_VERTICES:
            componentsRead = sscanf( lineBody.c_str(), "%u %u %u", &vertexTriangle[0], &vertexTriangle[1], &vertexTriangle[2] );

            if( componentsRead != 3 ){
                std::string errorMessage =
                        "Error reading mesh triangle - line definition [" +
                        lineBody +
                        "] - components read [" +
                        std::to_string( componentsRead ) + "]";
                throw std::runtime_error( errorMessage );
            }

            for( i=0; i<3; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexTriangle[i] -= 1;
            }
            primitiveData.vertexData.vertexTriangles.push_back( vertexTriangle );
        break;
        case FaceComponents::VERTICES_AND_UVS:
            // Extract the UV coordinates from the line and add it to the Mesh.
            componentsRead =
                    sscanf( lineBody.c_str(), "%u/%u %u/%u %u/%u",
                    &vertexTriangle[0], &uvTriangle[0],
                    &vertexTriangle[1], &uvTriangle[1],
                    &vertexTriangle[2], &uvTriangle[2]);

            if( componentsRead != 6 ){
                throw std::runtime_error( "ERROR reading OBJ face line (v/vt)" );
            }

            for( i=0; i<3; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexTriangle[i] -= 1;
                uvTriangle[i] -= 1;
            }

            primitiveData.vertexData.vertexTriangles.push_back( vertexTriangle );
            primitiveData.uvData.uvTriangles.push_back( uvTriangle );

        break;
        case FaceComponents::VERTICES_NORMALS_AND_UVS:
            componentsRead =
                    sscanf( lineBody.c_str(), "%u/%u/%u %u/%u/%u %u/%u/%u",
                    &vertexTriangle[0], &uvTriangle[0], &normalTriangle[0],
                    &vertexTriangle[1], &uvTriangle[1], &normalTriangle[1],
                    &vertexTriangle[2], &uvTriangle[2], &normalTriangle[2] );

            if( componentsRead != 9 ){
                throw std::runtime_error( "ERROR reading OBJ face line (v/vt/vn)" );
            }

            for( i=0; i<3; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexTriangle[i] -= 1;
                normalTriangle[i] -= 1;
                uvTriangle[i] -= 1;
            }

            primitiveData.vertexData.vertexTriangles.push_back( vertexTriangle );
            primitiveData.normalData.normalTriangles.push_back( normalTriangle );
            primitiveData.uvData.uvTriangles.push_back( uvTriangle );
        break;
        case FaceComponents::VERTICES_AND_NORMALS:
            componentsRead =
                    sscanf( lineBody.c_str(), "%u//%u %u//%u %u//%u",
                    &vertexTriangle[0], &normalTriangle[0],
                    &vertexTriangle[1], &normalTriangle[1],
                    &vertexTriangle[2], &normalTriangle[2] );

            if( componentsRead != 6 ){
                throw std::runtime_error( "ERROR reading OBJ face line (v//vn)" );
            }

            for( i=0; i<3; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexTriangle[i] -= 1;
                normalTriangle[i] -= 1;
            }
            primitiveData.vertexData.vertexTriangles.push_back( vertexTriangle );
            primitiveData.normalData.normalTriangles.push_back( normalTriangle );
        break;
    }

    primitiveData.trianglesGroups_.back().nTriangles++;
}


void OBJPrimitivesImporter::processQuadFaceStr( const std::string &lineBody, ImportedPrimitiveData& primitiveData )
{
    std::array< GLuint, 4 > vertexQuad = { 0 };
    std::array< GLuint, 4 > uvQuad = { 0 };
    std::array< GLuint, 4 > normalQuad = { 0 };
    int componentsRead;
    int i;

    switch( getFaceComponents( lineBody ) ){
        case FaceComponents::ONLY_VERTICES:
            componentsRead = sscanf( lineBody.c_str(),
                                     "%u %u %u %u",
                                     &vertexQuad[0],
                                     &vertexQuad[1],
                                     &vertexQuad[2],
                                     &vertexQuad[3] );

            if( componentsRead != 4 ){
                throw std::runtime_error( "Error reading mesh quad (Is it a textured mesh?)" );
            }

            for( i=0; i<4; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexQuad[i] -= 1;
            }
            insertQuad( primitiveData.vertexData.vertexTriangles, vertexQuad );
        break;
        case FaceComponents::VERTICES_AND_UVS:
            // Extract the UV coordinates from the line and add it to the Mesh.
            componentsRead =
                    sscanf( lineBody.c_str(), "%u/%u %u/%u %u/%u %u/%u",
                    &vertexQuad[0], &uvQuad[0],
                    &vertexQuad[1], &uvQuad[1],
                    &vertexQuad[2], &uvQuad[2],
                    &vertexQuad[3], &uvQuad[3] );

            if( componentsRead != 8 ){
                throw std::runtime_error( "ERROR reading OBJ face quad (v/vt)" );
            }

            for( i=0; i<4; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexQuad[i] -= 1;
                uvQuad[i] -= 1;
            }
            insertQuad( primitiveData.vertexData.vertexTriangles, vertexQuad );
            insertQuad( primitiveData.uvData.uvTriangles, uvQuad );
        break;
        case FaceComponents::VERTICES_NORMALS_AND_UVS:
            componentsRead =
                    sscanf( lineBody.c_str(), "%u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u",
                    &vertexQuad[0], &uvQuad[0], &normalQuad[0],
                    &vertexQuad[1], &uvQuad[1], &normalQuad[1],
                    &vertexQuad[2], &uvQuad[2], &normalQuad[2],
                    &vertexQuad[3], &uvQuad[3], &normalQuad[3] );

            if( componentsRead != 12 ){
                throw std::runtime_error( "ERROR reading OBJ face quad (v/vt/vn)" );
            }

            for( i=0; i<4; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexQuad[i] -= 1;
                uvQuad[i] -= 1;
                normalQuad[i] -= 1;
            }
            insertQuad( primitiveData.vertexData.vertexTriangles, vertexQuad );
            insertQuad( primitiveData.normalData.normalTriangles, normalQuad );
            insertQuad( primitiveData.uvData.uvTriangles, uvQuad );
        break;
        case FaceComponents::VERTICES_AND_NORMALS:
            componentsRead =
                    sscanf( lineBody.c_str(), "%u//%u %u//%u %u//%u %u//%u",
                    &vertexQuad[0], &normalQuad[0],
                    &vertexQuad[1], &normalQuad[1],
                    &vertexQuad[2], &normalQuad[2],
                    &vertexQuad[3], &normalQuad[3] );

            if( componentsRead != 8 ){
                throw std::runtime_error( "ERROR reading OBJ face quad (v//vn)" );
            }

            for( i=0; i<4; i++ ){
                // Decrement every vertex index because they are 1-based in the .obj file.
                vertexQuad[i] -= 1;
                normalQuad[i] -= 1;
            }
            insertQuad( primitiveData.vertexData.vertexTriangles, vertexQuad );
            insertQuad( primitiveData.normalData.normalTriangles, normalQuad );
        break;
    }

    primitiveData.trianglesGroups_.back().nTriangles += 2;
}


void OBJPrimitivesImporter::triangulateQuad(const FaceQuad &quad, FaceTriangle &triangle1, FaceTriangle &triangle2 )
{
    // First triangle
    triangle1[0] = quad[0];
    triangle1[1] = quad[1];
    triangle1[2] = quad[2];

    // Second triangle
    triangle2[0] = quad[0];
    triangle2[1] = quad[2];
    triangle2[2] = quad[3];
}


void OBJPrimitivesImporter::insertQuad(std::vector<FaceTriangle> &triangles, FaceQuad &quad)
{
    FaceTriangle triangle1;
    FaceTriangle triangle2;

    triangulateQuad( quad, triangle1, triangle2 );

    triangles.push_back( triangle1 );
    triangles.push_back( triangle2 );
}

void OBJPrimitivesImporter::generateOGLData(ImportedPrimitiveData& primitiveData)
{
    CompoundVerticesMap compoundVerticesMap;
    unsigned int currentTriangleIndex = 0;
    unsigned int currentTriangleElement = 0;

    CompoundVertex compoundVertex;
    CompoundVerticesMap::const_iterator finalVerticesIt;
    VertexIndice compoundVertexIndex;

    std::vector< TrianglesGroupWithMaterial >::iterator currentTriangleGroupIt = primitiveData.trianglesGroups_.begin();
    std::vector< TrianglesGroupWithMaterial >::iterator nextTriangleGroupIt;

    for( currentTriangleIndex = 0; currentTriangleIndex < primitiveData.vertexData.vertexTriangles.size(); currentTriangleIndex++ ){
        for( currentTriangleElement = 0; currentTriangleElement < 3; currentTriangleElement++ ){
            compoundVertex[0] = primitiveData.vertexData.vertexTriangles[currentTriangleIndex][currentTriangleElement];
            compoundVertex[1] = primitiveData.normalData.normalTriangles[currentTriangleIndex][currentTriangleElement];
            compoundVertex[2] = primitiveData.uvData.uvTriangles.size() ? primitiveData.uvData.uvTriangles[currentTriangleIndex][currentTriangleElement] : 0;

            finalVerticesIt = compoundVerticesMap.find( compoundVertex );

            if( finalVerticesIt != compoundVerticesMap.end() ){
                compoundVertexIndex = finalVerticesIt->second;
            }else{
                compoundVertexIndex = compoundVerticesMap.size();

                primitiveData.oglData.vboData.push_back( primitiveData.vertexData.vertices[ compoundVertex[0] ][0] );
                primitiveData.oglData.vboData.push_back( primitiveData.vertexData.vertices[ compoundVertex[0] ][1] );
                primitiveData.oglData.vboData.push_back( primitiveData.vertexData.vertices[ compoundVertex[0] ][2] );

                // Insert vertex normal.
                primitiveData.oglData.vboData.push_back( primitiveData.normalData.normals[ compoundVertex[1] ][0] );
                primitiveData.oglData.vboData.push_back( primitiveData.normalData.normals[ compoundVertex[1] ][1] );
                primitiveData.oglData.vboData.push_back( primitiveData.normalData.normals[ compoundVertex[1] ][2] );

                // Insert UV coordinates (if exist).
                if( primitiveData.oglData.includesTextures ){
                    primitiveData.oglData.vboData.push_back( primitiveData.uvData.uvVertices[ compoundVertex[2] ][0] );
                    primitiveData.oglData.vboData.push_back( primitiveData.uvData.uvVertices[ compoundVertex[2] ][1] );
                }

                compoundVerticesMap.insert( std::pair< CompoundVertex, VertexIndice >( compoundVertex, compoundVertexIndex ) );
            }

            if( currentTriangleGroupIt->firstTriangleIndex == currentTriangleIndex ){
                currentTriangleGroupIt->firstTriangleIndex = compoundVertexIndex;
                currentTriangleGroupIt->nTriangles = 1;
            }else{
                nextTriangleGroupIt = currentTriangleGroupIt + 1;
                if( ( nextTriangleGroupIt != primitiveData.trianglesGroups_.end() ) &&
                       ( nextTriangleGroupIt->firstTriangleIndex == currentTriangleIndex ) ){
                    nextTriangleGroupIt->firstTriangleIndex = compoundVertexIndex;
                    nextTriangleGroupIt->nTriangles = 1;
                    currentTriangleGroupIt = nextTriangleGroupIt;
                }else{
                    currentTriangleGroupIt->nTriangles++;
                }
            }

            primitiveData.oglData.eboData.push_back( compoundVertexIndex );
        }
    }
}


void OBJPrimitivesImporter::splitFileLine( const std::string &line, std::string &lineHeader, std::string &lineBody )
{
    unsigned int i = 0;

    while( ( i < line.size() ) && (line[i] != ' ') && (line[i] != '\t') ){
        i++;
    }

    if( i < line.size() ){
        lineHeader = line.substr( 0, i );

        while( ( i < line.size() ) &&
               ( (line[i] == ' ') || (line[i] == '\t') ) ){
            i++;
        }

        if( i < line.size() ){
            lineBody = line.substr( i, line.size() - i );
        }else{
            lineBody = "";
        }
    }else{
        lineHeader = "";
        lineBody = "";
    }
}


bool OBJPrimitivesImporter::supportedImageFile( const std::string& filePath )
{
    // List of supported image formats
    // (http://jcatki.no-ip.org:8080/SDL_image/SDL_image.html)
    // ".xcf" is not listed here although SDL_image supports it, because of the
    // following advice on SDL_image website: "This format is always changing,
    // and since there's no library supplied by the GIMP project to load XCF,
    // the loader may frequently fail to load much of any image from an XCF
    // file. It's better to load this in GIMP and convert to a better supported
    // image format."
    std::array< std::string, 15 > supportedExtensions =
    {
        ".tga",
        ".bmp",
        ".pnm",
        ".pgm",
        ".ppm",
        ".xpm",
        ".pcx",
        ".gif",
        ".jpg",
        ".jpeg",
        ".tif",
        ".tiff",
        ".lbm",
        ".iff",
        ".png"
    };

    std::string fileExtension = boost::filesystem::extension( filePath );

    for( const auto& supportedExtension : supportedExtensions ){
        if( fileExtension == supportedExtension ){
            return true;
        }
    }
    return false;
}



} // namespace como
