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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SCENE_HPP
#define SCENE_HPP

#include <common/scene/basic_scene.hpp>
#include <server/commands_historic.hpp>
#include <server/managers/resources_synchronization_library.hpp>
#include <common/ids/resource_ids_generator.hpp>

namespace como {

class Scene;
typedef std::unique_ptr< Scene > ScenePtr;

class Scene : public BasicScene
{
    public:
        /***
         * 1. Construction
         ***/
        Scene( const std::string& sceneName, CommandsHistoricPtr commandsHistoric, UsersMap& users, ResourceIDsGeneratorPtr resourceIDsGenerator, LogPtr log, const std::string& sceneFilePath = "" );
        Scene() = delete;
        Scene( const Scene& ) = delete;
        Scene( Scene&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~Scene();


        /***
         * 3. Command processing
         ***/
        void processCommand( const Command& command );


        /***
         * 4. Scene saving / loading
         ***/
        std::string saveToFile();
        void loadFromFile( const std::string& filePath );


        /***
         * 5. Getters
         ***/
        UserID generateUserID();


        /***
         * 6. Users management
         ***/
        void removeUser( UserID userID );


        /***
         * 7. Auxiliar I/O methods
         ***/
        static bool askForUserResponse( const std::string& question );
        static std::string askForUserString( const std::string& question );
        static void askForUserKeyPress( const std::string& message );


        /***
         * 8. Operators
         ***/
        Scene& operator = ( const Scene& ) = delete;
        Scene& operator = ( Scene&& ) = delete;


    private:
        /***
         * 9. Initialization
         ***/
        void initEmptyScene();


        /***
         * 10. Auxiliar methods
         ***/
        std::string generateSaveFilePath( const std::string& fileName );
        void saveToFile( const std::string& filePath );


        /***
         * Attributes
         ***/
        ResourceIDsGeneratorPtr resourceIDsGenerator_;

        ResourcesSynchronizationLibrary resourcesSyncLibrary_;

        UserID nextUserID_;
};

} // namespace como

#endif // SCENE_HPP
