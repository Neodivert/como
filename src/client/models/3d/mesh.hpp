/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
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
#include <common/commands/drawable_commands/mesh_creation_command.hpp> // For MeshType type.
#include <client/models/3d/materials/material.hpp>
#include <common/packables/array/packable_color.hpp>

namespace como {

const GLuint COMPONENTS_PER_VERTEX_POSITION = 3;
const GLuint COMPONENTS_PER_VERTEX = 6;


/***
 * File main class
 ***/

class Mesh : public Drawable
{
    private:
        // Mesh type
        MeshType type_;

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

        // Original vertex attribues.
        std::vector< glm::vec3 > originalVertices;
        std::vector< glm::vec3 > originalNormals;

        // Mesh's triangles.
        std::vector< std::array< GLuint, 3 > > triangles;

        // Mesh's centroid.
        glm::vec4 originalCentroid;
        glm::vec4 transformedCentroid;

        // Mesh's material.
        Material material_;

        /***
         * 1. Construction.
         ***/
    protected:
        Mesh( MeshType type, const char* file, PackableColor meshColor );
    public:
        Mesh( const char* file, PackableColor meshColor );
        Mesh( const Mesh& b );
        Mesh( Mesh&& ) = delete;


        virtual DrawablePtr clone();


        /***
         * 2. Destruction.
         ***/
        ~Mesh();


        /***
         * 3. Initialization.
         ***/
    private:
        void initMesh( const char* file );

        void initMeshBuffers();

        void initVertexData();
        void computeCentroid();
        void computeVertexNormals();


        /***
         * 4. File loading.
         ***/
    private:
        void LoadFromOBJ( const char* objFile );
    public:


        /***
         * 5. Getters.
         ***/
        MeshType getType() const ;
        glm::vec4 getCentroid() const ;
        void getVertexData( unsigned int& n, GLfloat* vertices );


        /***
         * 6. Setters.
         ***/

        void setMeshColor( PackableColor meshColor );
        void setMaterialDiffuseReflexivity( const PackableColor& diffuseReflexivity );


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
        virtual void draw( const glm::mat4& viewProjMatrix, const GLfloat* contourColor = nullptr ) const;


        /***
         * 9. Auxliar methods.
         ***/
        static void sendMVPMatrixToShader( const glm::mat4& mvpMatrix );


        /***
         * 10. Operators
         ***/
        Mesh& operator=( const Mesh& ) = delete ;
        Mesh& operator=( Mesh&& ) = delete;
};

}

#endif // MESH_HPP
