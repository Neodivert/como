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

#ifndef DRAWABLES_SELECTION_HPP
#define DRAWABLES_SELECTION_HPP

#include <client/models/3d/entity.hpp>
#include <client/models/3d/mesh.hpp>
#include <common/commands/user_commands/user_parameter_change_command.hpp> // For PivotPointMode type.
#include <map>
#include <functional>
#include <mutex>
#include <client/managers/selections/resources/resources_selection.hpp>

namespace como {

typedef std::map< ResourceID, DrawablePtr > DrawablesMap;


const unsigned int N_PIVOT_POINT_MODES = 3;

// Available pivot point modes (strings for GUI output).
const char pivotPointModeStrings[N_PIVOT_POINT_MODES][32] =
{
     "Median Point",
     "Individual Centroid",
     "World origin"
};

class DrawablesSelection : public virtual ResourcesSelection<Entity>
{
    private:
        // Selection border color.
        glm::vec4 borderColor_;

        // Selection's centroid.
        glm::vec4 centroid_;

        // Selection's pivot point mode.
        PivotPointMode pivotPointMode_;

        // Mutex for concurrent access to this selection.
        mutable std::recursive_mutex mutex_;

        const void* highlightedProperty_;

        bool displayEdges_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        DrawablesSelection( glm::vec4 borderColor = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
        DrawablesSelection( const DrawablesSelection& ) = delete;
        DrawablesSelection( DrawablesSelection&& ) = delete;

        ~DrawablesSelection() = default;


        /***
         * 2. Getters
         ***/
        glm::vec4 getCentroid() const ;
        PivotPointMode getPivotPointMode() const ;
        std::string getName() const;
        std::string getTypeName() const;

        bool existsDrawable( const ResourceID& id ) const;
        std::string getDrawableName( const ResourceID& id ) const;


        /***
         * 3. Setters
         ***/
        virtual void setPivotPointMode( PivotPointMode pivotPointMode );
        void displayEdges( bool displayEdges );


        /***
         * 4. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotate( GLfloat angle, glm::vec3 axis );
        virtual void scale( glm::vec3 scaleFactors );


    private:
        /***
         * 5. Updating
         ***/
        void updateSelectionCentroid();


    public:
        /***
         * 6. Ray picking
         ***/
        bool intersect( glm::vec3 r0, glm::vec3 r1, ResourceID& closestDrawable, float& minT ) const ;


        /***
         * 7. Drawing
         ***/
        virtual void draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const ;


        /***
         * 8. Operators
         ***/
        DrawablesSelection& operator=( const DrawablesSelection& ) = delete ;
        DrawablesSelection& operator=( DrawablesSelection&& ) = delete;


        /***
         * 9. Auxiliar methods
         ***/
        void highlighDrawableProperty( const void* property );
};

typedef shared_ptr< DrawablesSelection > DrawablesSelectionPtr;
typedef shared_ptr< const DrawablesSelection > DrawablesSelectionConstPtr;

} // namespace como

#endif // DRAWABLES_SELECTION_HPP
