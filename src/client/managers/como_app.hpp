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

#ifndef COMO_APP_HPP
#define COMO_APP_HPP

#include "scene.hpp"
#include <QOpenGLContext>
#include <string>
#include <QObject>
#include <map>
#include <array>
#include "../../common/utilities/log.hpp"

namespace como {

enum class AppMode {
    OBJECT = 0,
    EDITION
};
const unsigned int N_APP_MODES = 2;

// Array with all the possible app mode values (for conversion between
// AppMode and int).
typedef std::array< AppMode, N_APP_MODES > AppModes;
extern AppModes appModes;

// Array with a string for each app mode value (for output in GUI).
extern std::array< QString, N_APP_MODES > appModeStrings;


enum class EditionScope
{
    GLOBAL = 0,
    LOCAL
};

extern std::map<EditionScope, std::string> editionScopeStrings;

enum class TransformationType
{
    NONE = 0,
    TRANSLATION,
    ROTATION,
    SCALE
};


enum class TransformationMode
{
    FREE = 0,
    FIXED_X,
    FIXED_Y,
    FIXED_Z
};
const unsigned int N_TRANSFORMATION_MODES = 4;

// Array with all the possible transformation modes.
typedef std::array< TransformationMode, N_TRANSFORMATION_MODES > TransformationModes;
extern TransformationModes transformationModes;

// Array with a string for each transformation mode value (for output in GUI).
extern std::array< QString, N_TRANSFORMATION_MODES > transformationModeStrings;


class ComoApp : public QObject {
    Q_OBJECT

    protected:
        // Current app mode.
        AppMode appMode;

        // This sets if the current user transformation is free or fixed to a axis.
        TransformationMode transformationMode;

        // When in edition mode, this sets whether editions are applied to drawables
        // in local (object) or global (world) coordinates.
        EditionScope editionScope;

        // Current transformation type.
        TransformationType transformationType;

        // Current pivot point mode.
        PivotPointMode pivotPointMode;

        // Current OpenGL context.
        shared_ptr< QOpenGLContext > oglContext;

        // Curren scene.
        shared_ptr< Scene > scene;

        // Log
        LogPtr log_;


        /***
         * 1. Initialization and destruction
         ***/
    public:
        ComoApp( QWidget* parent, LogPtr log );
        ~ComoApp();
        /*
        static ComoApp* getInstance();
        static void destroy();
        */

        /***
         * 2. Getters
         ***/
        AppMode getAppMode() const ;
        EditionScope getEditionScope() const ;
        TransformationType getTransformationType() const ;
        TransformationMode getTransformationMode() const ;
        PivotPointMode getPivotPointMode() const;
        ScenePtr getScene() const ;
        shared_ptr< QOpenGLContext > getOpenGLContext() const ;
        LogPtr getLog() const ;


        /***
         * 3. Setters (slots)
         ***/
    public slots:
        void setAppMode( AppMode appMode );
        void setTransformationMode( TransformationMode transformationMode );

        void setEditionScope( EditionScope editionScope );
        void setTransformationType( TransformationType transformationType );

        void setPivotPointMode( PivotPointMode pivotPointMode );

    signals:
        void appModeIndexChanged( int index );
        void transformationModeIndexChanged( int index );
};

} // namespace como

#endif // COMO_APP_HPP
