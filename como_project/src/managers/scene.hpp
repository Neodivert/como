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

#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "../models/3d/cube.hpp"
#include "../models/3d/camera.hpp"


namespace como {

typedef std::list< std::shared_ptr< Drawable > > DrawablesList;

typedef shared_ptr<Drawable> DrawablePtr;

enum class DrawableType
{
    CUBE = 0
};
const unsigned int N_DRAWABLE_TYPES = 1;

typedef std::array< DrawableType, N_DRAWABLE_TYPES > DrawableTypes;
extern DrawableTypes drawableTypes;

typedef std::array< QString, N_DRAWABLE_TYPES > DrawableTypeStrings;
extern DrawableTypeStrings drawableTypeStrings;

enum class PivotPointMode
{
    MEDIAN_POINT = 0,
    INDIVIDUAL_CENTROIDS,
    WORLD_ORIGIN
};
const unsigned int N_PIVOT_POINT_MODES = 3;

// Array with all the possible pivot point modes (for conversion between
// PivotPointMode and int).
typedef std::array< PivotPointMode, N_PIVOT_POINT_MODES > PivotPointModes;
extern PivotPointModes pivotPointModes;

// Array with a string for each app mode value (for output in GUI).
typedef std::array< QString, N_PIVOT_POINT_MODES > PivotPointModeStrings;
extern PivotPointModeStrings pivotPointModeStrings;


class Scene : public QObject
{
    Q_OBJECT

    private:
        // Scene's drawables not selected by user.
        DrawablesList nonSelectedDrawables;

        // Scene's drawables selected by user.
        DrawablesList selectedDrawables;

        GLuint guideRectsVAO;
        GLuint guideRectsVBO;

        GLuint uniformColorLocation;

        // VAO and VBO for displaying the world coordinates system axis for
        // each viewport.
        GLuint worldAxisVAO;
        GLuint worldAxisVBO;
    public:
        /***
         * 1. Initialization and destruction
         ***/
        Scene();
        ~Scene();

        void initWorldAxis();

        /***
         * 1. Drawables administration
         ***/
        void addDrawable( shared_ptr<Drawable> drawable );
        void addCube( Cube* cube );
        void addDrawable( DrawableType drawableType );


        /***
         * 2. Drawables selection.
         ***/
    public:
        void selectDrawable( const unsigned int index );
        void selectAll();
        void unselectAll();

        int selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection = false );


        glm::vec4 getSelectionCentroid() const ;


        /***
         * 3. Transformations
         ***/
        void translateSelection( const glm::vec3& direction );
        void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const PivotPointMode& pivotPointMode );
        void scaleSelection( const glm::vec3& scaleFactors );
        //void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot );

        void deleteSelection();

        /***
         * 4. Drawing
         ***/
        void draw( const int& drawGuideRect = -1 ) const ;
        void drawWorldAxis() const ;

        /***
         * 5. Signals
         ***/
    signals:
        void renderNeeded();
};

} // namespace como

#endif // SCENE_HPP
