/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef DRAWABLES_SELECTION_HPP
#define DRAWABLES_SELECTION_HPP

#include "drawable.hpp"
#include "../../../common/utilities/ids.hpp"
#include <map>
#include <functional>

namespace como {

typedef std::map< DrawableID, DrawablePtr > DrawablesMap;

typedef std::function<void (void)> NotificationCallback;


// Available pivot point modes.
enum class PivotPointMode
{
    MEDIAN_POINT = 0,
    INDIVIDUAL_CENTROIDS,
    WORLD_ORIGIN
};
const unsigned int N_PIVOT_POINT_MODES = 3;

// Available pivot point modes (strings for GUI output).
const char pivotPointModeStrings[N_PIVOT_POINT_MODES][32] =
{
     "Median Point",
     "Individual Centroid",
     "World origin"
};


class DrawablesSelection
{
    private:
        // Drawables in the selection.
        DrawablesMap drawables_;

        // Selection's centroid.
        glm::vec4 centroid_;

        // Notification callback (this function is called when the selection
        // is updated).
        NotificationCallback notificationCallback_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        DrawablesSelection( NotificationCallback notificationCallback );


        /***
         * 2. Getters
         ***/
        glm::vec4 getCentroid() const ;


        /***
         * 3. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotate( GLfloat angle, glm::vec3 axis, PivotPointMode pivotPointMode );
        virtual void scale( glm::vec3 scaleFactors, PivotPointMode pivotPointMode );


        /***
         * 4. Centroid updating
         ***/
    private:
        void updateSelectionCentroid();


        /***
         * 5. Drawables management
         ***/
    public:
        void addDrawable( DrawableID drawableID, DrawablePtr drawable );
        bool moveDrawable( DrawableID drawableID, DrawablesSelection& destinySelection );
        void moveAll( DrawablesSelection& destinySelection );
        void clear();


        /***
         * 6. Ray picking
         ***/
        bool intersect( glm::vec3 r0, glm::vec3 r1, DrawableID& closestDrawable, float& minT ) const ;


        /***
         * 7. Drawing
         ***/
        void draw( const GLfloat* contourColor = nullptr ) const ;
};

} // namespace como

#endif // DRAWABLES_SELECTION_HPP
