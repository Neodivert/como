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
#include "abstract_mesh.hpp"
#include <vector>
#include <array>
#include <fstream>
#include <stdexcept>
#include <client/models/3d/materials/material.hpp>
#include <common/packables/array/packable_color.hpp>
#include <common/mesh_info/mesh_info.hpp>

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
    protected:
        GLuint vao;

        // VBO for transformed vertex data.
        GLuint vbo;
    private:

        // EBO : Elements-Buffer Array.
        GLuint ebo;
        GLsizei nEboElements_;

        // Mesh's centroid.
        glm::vec4 originalCentroid;
        glm::vec4 transformedCentroid;

        bool includesTexture_;
        unsigned int componensPerVertex_;

        std::vector< PolygonGroupData > polygonsGroups_;

        // Mesh's material.
        std::vector< MaterialConstPtr > materials_;

        bool displayVertexNormals_;

        bool displayEdges_;


        /***
         * 1. Construction.
         ***/
    protected:
        Mesh( MeshType type, const char* file, MaterialConstPtr material, bool displayVertexNormals = false );
    public:
        Mesh( MeshVertexData vertexData, const MeshOpenGLData& oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials, bool displayVertexNormals = false );
        Mesh( const Mesh& b ) = default;
        Mesh( Mesh&& ) = delete;


        /***
         * 2. Destruction.
         ***/
        ~Mesh();


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
        virtual glm::vec3 centroid() const;


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
         * 8. Update and drawing.
         ***/
    protected:
        // Recompute transformed vertices based on original ones and transformation matrix.
        virtual void update();

    public:
        // Send mesh to OpenGL server for rendering it.
        virtual void draw( OpenGLPtr openGL, const glm::mat4& view, const glm::mat4& projection, const glm::vec4* contourColor = nullptr ) const;
    private:
        virtual void drawVertexNormals( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec4 color ) const;
    public:

        /***
         * 9. Auxliar methods.
         ***/
        bool containsProperty( const void* property ) const;


        /***
         * 10. Operators
         ***/
        Mesh& operator=( const Mesh& ) = delete;
        Mesh& operator=( Mesh&& ) = delete;
};

typedef std::shared_ptr< Mesh > MeshPtr;

}

#endif // MESH_HPP
