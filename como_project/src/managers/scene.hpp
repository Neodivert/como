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

enum LinesBufferOffset {
    WORLD_AXIS = 0,
    GUIDE_AXIS,
    TRANSFORM_GUIDE_LINE,

    N_LINES_BUFFER_OFFSETS
};

class Scene : public QObject
{
    Q_OBJECT

    private:
        // Scene's drawables not selected by user.
        DrawablesList nonSelectedDrawables;

        // Scene's drawables selected by user.
        DrawablesList selectedDrawables;

        // Lines VAO, VBO and offsets.
        GLuint linesVAO;
        GLuint linesVBO;
        GLuint linesBufferOffsets[N_LINES_BUFFER_OFFSETS];

        GLuint uniformColorLocation;
        glm::vec4 selectionCentroid;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Scene();
        ~Scene();

        void initLinesBuffer();

        /***
         * 2. Getters and setters
         ***/
        void setTransformGuideLine( glm::vec3 origin, glm::vec3 destiny );
        glm::vec3 getPivotPoint( const PivotPointMode& pivotPointMode );


        /***
         * 3. Drawables administration
         ***/
        void addDrawable( shared_ptr<Drawable> drawable );
        void addCube( Cube* cube );
        void addDrawable( DrawableType drawableType );


        /***
         * 4. Drawables selection.
         ***/
    public:
        void selectDrawable( const unsigned int index );
        void selectAll();
        void unselectAll();

        int selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection = false );

        glm::vec4 getSelectionCentroid() const ;


        /***
         * 5. Transformations
         ***/
        void translateSelection( const glm::vec3& direction );
        void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const PivotPointMode& pivotPointMode );
        void scaleSelection( const glm::vec3& scaleFactors, const PivotPointMode& pivotPointMode );
        //void rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot );

        void deleteSelection();


        /***
         * 6. Updating
         ***/
        void updateSelectionCentroid();


        /***
         * 7. Drawing
         ***/
        void draw( const int& drawGuideRect = -1 ) const ;
        void drawWorldAxis() const ;
        void drawTransformGuideLine() const ;


        /***
         * 8. Signals
         ***/
    signals:
        void renderNeeded();
};

} // namespace como

#endif // SCENE_HPP
