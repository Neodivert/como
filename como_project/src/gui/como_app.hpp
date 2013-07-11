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

#include "../3d_entities/scene.hpp"
#include <QOpenGLContext>
#include <string>
#include <vector>
#include <QObject>
#include <map>

enum class AppMode {
    CREATION = 0,
    EDITION
};

extern std::map<QString, AppMode> appModeStrings;

enum class EditionScope
{
    GLOBAL = 0,
    LOCAL
};

extern std::map<EditionScope, std::string> editionScopeStrings;

enum class EditionSubMode
{
    TRANSLATION = 0,
    ROTATION,
    SCALE
};

class ComoApp : public QObject {
    Q_OBJECT

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
    public:
        ComoApp();
        /*
        static ComoApp* getInstance();
        static void destroy();
        */

        /***
         * 2. Getters
         ***/
        AppMode getAppMode() const ;
        EditionScope getEditionScope() const ;
        EditionSubMode getEditionSubMode() const ;

        shared_ptr< Scene > getScene() const ;

        shared_ptr< QOpenGLContext > getOpenGLContext() const ;


        /***
         * 3. Setters
         ***/
    public slots:
        void setAppMode( AppMode appMode );
        void setEditionScope( EditionScope editionScope );
        void setEditionSubMode( EditionSubMode editionSubMode );
};

#endif // COMO_APP_HPP
