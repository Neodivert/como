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

#ifndef COMO_APP_HPP
#define COMO_APP_HPP

#include "scene/public_scene.hpp"
#include <QOpenGLContext>
#include <string>
#include <QObject>
#include <map>
#include <array>
#include "../../common/utilities/log.hpp"

namespace como {

// Available app modes.
enum class AppMode {
    OBJECT = 0,
    EDITION
};
const unsigned int N_APP_MODES = 2;

// Available app modes (strings for GUI output).
const char appModeStrings[N_APP_MODES][32] =
{
    "Object mode",
    "Edition mode"
};

// Available edition scopes.
enum class EditionScope
{
    GLOBAL = 0,
    LOCAL
};
const unsigned int N_EDITION_SCOPES = 2;

// Available edition scopes (strings for GUI output).
const char editionScopeStrings[N_EDITION_SCOPES][16] =
{
    "Local",
    "Global"
};


// Available transformation types.
enum class TransformationType
{
    NONE = 0,
    TRANSLATION,
    ROTATION,
    SCALE
};
const unsigned int N_TRANSFORMATION_TYPES = 4;

// Available transformation types (strings for GUI output).
const char transformationTypeStrings[N_TRANSFORMATION_TYPES][16] =
{
    "None",
    "Translation",
    "Rotation",
    "Scale"
};

// Available transformation modes.
enum class TransformationMode
{
    FREE = 0,
    FIXED_X,
    FIXED_Y,
    FIXED_Z
};
const unsigned int N_TRANSFORMATION_MODES = 4;

// Available transformation modes (strings for GUI output).
const char transformationModeStrings[N_TRANSFORMATION_MODES][16] =
{
    "Free",
    "Fixed to X axis",
    "Fixed to Y axis",
    "Fixed to Z axis"
};

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

        // Log
        LogPtr log_;

        // Curren scene.
        shared_ptr< Scene > scene;

        /***
         * 1. Initialization and destruction
         ***/
    public:
        ComoApp();
        ComoApp( const ComoApp& ) = delete;
        ComoApp( ComoApp&& ) = delete;

        ~ComoApp() = default;

        /***
         * 2. Getters
         ***/
        AppMode getAppMode() const ;
        EditionScope getEditionScope() const ;
        TransformationType getTransformationType() const ;
        TransformationMode getTransformationMode() const ;
        ScenePtr getScene() const ;
        LogPtr getLog() const ;


        /***
         * 3. Operators
         ***/
        ComoApp& operator=( const ComoApp& ) = delete ;
        ComoApp& operator=( ComoApp&& ) = delete;


        /***
         * 4. Setters (slots)
         ***/
    public slots:
        void setAppMode( AppMode appMode );
        void setTransformationMode( TransformationMode transformationMode );

        void setEditionScope( EditionScope editionScope );
        void setTransformationType( TransformationType transformationType );

    signals:
        void appModeIndexChanged( int index );
        void transformationModeIndexChanged( int index );
};

} // namespace como

#endif // COMO_APP_HPP
