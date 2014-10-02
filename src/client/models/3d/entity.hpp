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

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <common/resources/resource.hpp>
#include <client/models/3d/drawable.hpp>
#include <client/models/3d/transformable.hpp>
#include <client/models/dependencies.hpp>
#include <client/models/utilities/open_gl.hpp>

namespace como {


/***
 * Enum types
 ***/
enum COORDINATES
{
    X = 0,
    Y,
    Z
};

enum class DrawableType : std::uint8_t
{
    MESH = 0
};


/***
 * Main file class
 ***/

class Entity : public Resource, public Drawable, public Transformable
{
    private:
        /*! Type of drawable */
        const DrawableType type_;


    protected:
        // TODO: Try to make these attributes private?.
        std::array< glm::vec4, 3 > originalOrientation;
        std::array< glm::vec4, 3 > transformedOrientation;

        glm::mat4 modelMatrix_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        // Constructor. Initialize transformation matrix to identity matrix.
        Entity( const ResourceID& id, const std::string& name, DrawableType type );
        Entity( const Entity& ) = delete;
        Entity( Entity&& ) = delete;

        virtual ~Entity(){}


        /***
         * 2. Getters
         ***/
        glm::mat4 getModelMatrix() const;
        DrawableType getType() const;
        virtual bool containsProperty( const void* property ) const = 0;


        /***
         * 3. Transformations
         ***/
        virtual void intersects( glm::vec3 r0, glm::vec3 r1, float& t, unsigned int* triangle = nullptr  ) const = 0;

        virtual void applyTransformationMatrix( const glm::mat4& newTransformation );


        /***
         * 4. Operators
         ***/
        Entity& operator=( const Entity& ) = delete ;
        Entity& operator=( Entity&& ) = delete;


        /***
         * 5. Updating and drawing
         ***/
    protected:
        virtual void update();

    public:
        virtual void draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec4* contourColor = nullptr ) const = 0;
};



} // namespace como

#endif // ENTITY_HPP
