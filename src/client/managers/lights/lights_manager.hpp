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

#ifndef LIGHTS_MANAGER_HPP
#define LIGHTS_MANAGER_HPP

#include <client/managers/drawables_manager/drawables_manager.hpp>
#include <client/models/3d/lights/lights.hpp>
#include <map>

namespace como {

typedef PackableDrawableID LightID;

class LightsManager : public QObject
{
    Q_OBJECT

    private:
        // Lights are drawables, so the drawables manager is also implied in
        // lights management.
        DrawablesManagerPtr drawablesManager_;

        std::map< LightID, LightPtr > lights_;

    public:
        /***
         * 1. Construction
         ***/
        LightsManager();
        LightsManager( DrawablesManagerPtr drawablesManager );
        LightsManager( const LightsManager& ) = delete;
        LightsManager( LightsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsManager() = default;


        /***
         * 3. Operators
         ***/
        LightsManager& operator = ( const LightsManager& ) = delete;
        LightsManager& operator = ( LightsManager&& ) = delete;
};

} // namespace como

#endif // LIGHTS_MANAGER_HPP
