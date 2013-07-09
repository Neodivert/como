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

enum AppMode {
    MODE_CREATION = 0,
    MODE_EDITION
};

enum EditionScope
{
    GLOBAL = 0,
    LOCAL
};

enum EditionSubMode
{
    TRANSLATION = 0,
    ROTATION,
    SCALE
};

class ComoApp {
    protected:
        AppMode appMode;
        EditionScope editionScope;
        EditionSubMode editionSubMode;

        static ComoApp* singlentonInstance;

        /***
         * 1. Initialization and destruction
         ***/
    private:
        ComoApp();

    public:
        ComoApp* getInstance();
        void destroy();

        /***
         * 2. Setters and getters
         ***/
        void setAppMode( AppMode appMode );
        AppMode getAppMode() const ;

        void setEditionScope( EditionScope editionScope );
        EditionScope getEditionScope() const ;

        void setEditionSubMode( EditionSubMode editionSubMode );
        EditionSubMode getEditionSubMode() const ;
};

#endif // COMO_APP_HPP
