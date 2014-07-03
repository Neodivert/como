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
#include "drawable.hpp"
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

class Mesh : public Drawable
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

        // Mesh's material.
        MaterialConstPtr material_;

        /***
         * 1. Construction.
         ***/
    protected:
        Mesh( MeshType type, const char* file, MaterialConstPtr material );
    public:
        Mesh( MeshVertexData vertexData, const MeshOpenGLData& oglData, MaterialConstPtr material );
        Mesh( const Mesh& b ) = default;
        Mesh( Mesh&& ) = delete;


        virtual DrawablePtr clone();


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
        unsigned int getOwnBytesPerVertex() const;
        unsigned int getOwnComponentsPerVertex() const;
        virtual unsigned int getBytesPerVertex() const;
        virtual unsigned int getComponentsPerVertex() const;

        void computeCentroid();


        /***
         * 5. Getters.
         ***/
    public:
        MeshType getType() const ;
        glm::vec4 getCentroid() const ;


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
        virtual void draw( OpenGLPtr openGL, const glm::mat4& viewProjMatrix, const GLfloat* contourColor = nullptr ) const;


        /***
         * 9. Auxliar methods.
         ***/
        static void sendMVPMatrixToShader( const glm::mat4& mvpMatrix );


        /***
         * 10. Operators
         ***/
        Mesh& operator=( const Mesh& ) = delete;
        Mesh& operator=( Mesh&& ) = delete;
};

}

#endif // MESH_HPP
