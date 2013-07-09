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

#include "3d_entities/scene.hpp"
#include <QOpenGLContext>
#include <string>
#include <vector>

enum AppMode {
    MODE_CREATION = 0,
    MODE_EDITION,

    N_APP_MODES
};

const char appModeStrings [N_APP_MODES][50] = {
    "Creation",
    "Edition"
};

enum EditionScope
{
    GLOBAL = 0,
    LOCAL,

    N_EDITION_SCOPES
};

const char editionScopeStrings [N_EDITION_SCOPES][50] = {
    "Global",
    "Local"
};

enum EditionSubMode
{
    TRANSLATION = 0,
    ROTATION,
    SCALE
};

class ComoApp {
    protected:
        // Current app mode.
        AppMode appMode;

        // When in edition mode, this sets whether editions are applied to drawables
        // in local (object) or global (world) coordinates.
        EditionScope editionScope;

        // Current edition submode.
        EditionSubMode editionSubMode;

        // Singlenton class instance.
        static ComoApp* singlentonInstance;

        // Current OpenGL context.
        shared_ptr< QOpenGLContext > oglContext;

        // Curren scene.
        shared_ptr< Scene > scene;

        /***
         * 1. Initialization and destruction
         ***/
    private:
        ComoApp();

    public:
        static ComoApp* getInstance();
        static void destroy();

        /***
         * 2. Setters and getters
         ***/
        void setAppMode( AppMode appMode );
        AppMode getAppMode() const ;

        void setEditionScope( EditionScope editionScope );
        EditionScope getEditionScope() const ;

        void setEditionSubMode( EditionSubMode editionSubMode );
        EditionSubMode getEditionSubMode() const ;

        shared_ptr< Scene > getScene() const ;

        shared_ptr< QOpenGLContext > getOpenGLContext() const ;
};

#endif // COMO_APP_HPP
