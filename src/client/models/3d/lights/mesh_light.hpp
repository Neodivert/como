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

#ifndef MESH_LIGHT_HPP
#define MESH_LIGHT_HPP

#include <client/models/3d/lights/abstract_light.hpp>
#include <client/models/3d/mesh.hpp>
#include <common/commands/light_commands/light_creation_command.hpp>

namespace como {

/*!
 * \class MeshLight
 *
 * \brief Base class for all type of Lights.
 */
class MeshLight : public AbstractLight, public Mesh
{
    private:
        /*! MeshLight type */
        const LightType type_;

        /*!
         * Location of the GLSL shader uniform variable keeping this light's
         * color.
         */
        GLint colorLocation_;

    public:
        /***
         * 1. Construction
         ***/

        /*!
         * \brief Constructs a light from the given data.
         * \param type light type
         * \param primitivePath path to the file containing the specification
         * for the light's associated mesh.
         * \param lightIndex index of this light's data into the array of
         * lights in GLSL shader.
         * TODO: Complete
         */
        MeshLight( LightType type, const char* meshPath, GLuint lightIndex, MaterialConstPtr material, PackableColor lightColor );

        /*! \brief Copy constructor */
        // TODO: Define a right copy constructor when inplementing multiple lights
        // (colorLocation_ attribute would point to different places from light to
        // light).
        MeshLight( const MeshLight& ) = default;

        /*! \brief Move constructor */
        MeshLight( MeshLight&& ) = delete;

        virtual DrawablePtr clone() = 0;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~MeshLight() = default;


        /***
         * 3. Getters
         ***/

        /*! \brief Get MeshLight's light color */
        virtual PackableColor getLightColor() const;

        /*! \brief Get the light type */
        LightType getType() const;


        /***
         * 4. Setters
         ***/

        /*! \brief Set MeshLight's light color */
        virtual void setLightColor( const PackableColor& color );


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        MeshLight& operator=( const MeshLight& ) = delete ;

        /*! \brief Move assignment operator */
        MeshLight& operator=( MeshLight&& ) = delete;
};

typedef std::shared_ptr< MeshLight > MeshLightPtr;
typedef std::shared_ptr< const MeshLight > MeshLightConstPtr;

} // namespace como

#endif // MESH_LIGHT_HPP
