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

#ifndef MESH_HPP
#define MESH_HPP

/***
 * Includes
 ***/
#include <client/managers/managers/materials/materials_manager.hpp>
#include "abstract_mesh.hpp"
#include <vector>
#include <array>
#include <fstream>
#include <stdexcept>
#include <client/models/3d/materials/material.hpp>
#include <common/packables/packable_color.hpp>
#include <common/primitives/primitive_data/primitive_data.hpp>


namespace como {

const GLuint COMPONENTS_PER_VERTEX_POSITION = 3;
const GLuint COMPONENTS_PER_VERTEX = 6;

enum class MeshType : std::uint8_t {
    MESH = 0,
    LIGHT,
    CAMERA
};


/***
 * File main class
 ***/

class Mesh : public AbstractMesh, public Entity
{
    friend class DirectionalLight; // TODO: Remove

    private:
        // Mesh type
        MeshType type_;

        // Mesh vertex data (vertices and vertex triangles).
        MeshVertexData vertexData_;

        // Location of the uniform shader variable used for coloring geometries.
        static GLint uniformColorLocation;

        // Location of the shader uniform variable for MVP matrix.
        static GLint mvpMatrixLocation_;

        // Location of the shader uniform variable for normal matrix.
        static GLint normalMatrixLocation_;

        // VAO : Vertex Attributes Array.
        GLuint vao;

        // VBO for transformed vertex data.
        GLuint vbo;

        // EBO : Elements-Buffer Array.
        GLuint ebo;
        GLsizei nEboElements_;

        bool includesTexture_;

        bool displayVertexNormals_;

        // Mesh's centroid.
        glm::vec4 originalCentroid;
        glm::vec4 transformedCentroid;

        unsigned int componensPerVertex_;

        // Mesh's material.
        //ConstMaterialsVector materials_;
        std::vector< ResourceID > materialIDs_;

        bool displayEdges_;

        MaterialsManager* materialsManager_;

        /***
         * 1. Construction.
         ***/
    protected:
        Mesh( const ResourceID& meshID, const std::string& meshName, const ResourceID& firstMaterialID, MeshType type, const char* file, MaterialsManager& materialsManager, bool displayVertexNormals = false );
    public:
        Mesh( const ResourceID& meshID, const ResourceID& firstMaterialID, const PrimitiveData& primitiveData, MaterialsManager& materialsManager, bool displayVertexNormals = false );
        Mesh( const Mesh& b ) = default;
        Mesh( Mesh&& ) = delete;


        /***
         * 2. Destruction.
         ***/
        virtual ~Mesh();


        /***
         * 3. Initialization.
         ***/
    protected:
        void init( const MeshOpenGLData& oglData );
        void genOpenGLBuffers();
        void initShaderLocations();
        void populateOpenGLBuffers( const MeshOpenGLData& oglData );

        void initMeshBuffers();

        void initVBO();
        virtual void initVAO();
        virtual unsigned int getBytesPerVertex() const;
        virtual unsigned int getComponentsPerVertex() const;

        void computeCentroid();


        /***
         * 5. Getters.
         ***/
    public:
        MeshType getType() const ;
        bool displaysVertexNormals() const;
        glm::vec3 getOriginalCentroid() const;
        virtual glm::vec3 centroid() const;
        bool includesTextures() const;
        virtual std::string typeName() const;


        /***
         * 6. Setters
         ***/
        void displayVertexNormals( bool display );
        virtual void displayEdges( bool display );


        /***
         * 7. Intersections.
         ***/
        virtual void intersects( glm::vec3 rayOrigin, glm::vec3 rayDirection, float& minT, unsigned int* triangle = nullptr ) const ;


        /***
         * 8. Shader communication
         ***/
        virtual void sendToShader( OpenGL& openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix ) const;


        /***
         * 8. Update and drawing.
         ***/
    protected:
        // Recompute transformed vertices based on original ones and transformation matrix.
        virtual void update();

        virtual void drawTriangles( unsigned int firstTriangleIndex, unsigned int nTriangles ) const;

    public:
        // Send mesh to OpenGL server for rendering it.

        virtual void drawEdges( OpenGLPtr openGL, const glm::mat4& view, const glm::mat4& projection, const glm::vec4* contourColor = nullptr ) const;
        virtual void drawVertexNormals( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec4 color ) const;
    public:

        /***
         * 9. Auxliar methods.
         ***/
        bool containsProperty( const void* property ) const;
        void sendMaterialToShader( const unsigned int index ) const;


        /***
         * 10. Operators
         ***/
        Mesh& operator=( const Mesh& ) = delete;
        Mesh& operator=( Mesh&& ) = delete;
};

typedef std::unique_ptr< Mesh > MeshPtr;

}

#endif // MESH_HPP
