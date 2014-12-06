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

#include <client/models/3d/lights/lights.hpp>
#include <common/commands/light_commands/light_commands.hpp>
#include <map>
#include <common/utilities/observer_pattern/observer.hpp>
#include <client/managers/managers/entities/specialized_entities_manager.hpp>
#include <client/managers/selections/lights/local_lights_selection.hpp>

namespace como {

typedef std::map< ResourceID, LightSharedPtr > LightsMap;

class LightsManager : public SpecializedEntitiesManager< Light, LightsSelection, LocalLightsSelection >
{
    public:
        /***
         * 1. Construction
         ***/
        LightsManager() = delete;
        LightsManager( ServerInterfacePtr server, LogPtr log, OpenGL* openGL );
        LightsManager( const LightsManager& ) = delete;
        LightsManager( LightsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LightsManager() = default;


        /***
         * 4. Lights management
         ***/
        void requestDirectionalLightCreation();

    private:
        void addDirectionalLight( const ResourceID& lightID, const Color& lightColor );


        /***
         * 5. Remote command execution
         ***/
    public:
        void executeRemoteCommand( const LightCommand& command );


        /***
         * 6. Shader communication
         ***/
        void sendLightsToShader( OpenGL& openGL, const glm::mat4& viewMatrix ) const;


        /***
         * 7. Operators
         ***/
        LightsManager& operator = ( const LightsManager& ) = delete;
        LightsManager& operator = ( LightsManager&& ) = delete;


        /***
         * 8. Updating
         ***/
    private:
        virtual void update();


        OpenGL* openGL_;
};

typedef std::shared_ptr< LightsManager > LightsManagerPtr;

} // namespace como

#endif // LIGHTS_MANAGER_HPP
