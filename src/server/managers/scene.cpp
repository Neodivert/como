#include "scene.hpp"

namespace como {

/***
 * 1. Construction
 ***/

Scene::Scene( const std::string& sceneName, CommandsHistoricPtr commandsHistoric, const std::string& sceneFilePath ) :
    BasicScene( sceneName ),
    resourcesSyncLibrary_( commandsHistoric, getTempDirPath(), sceneFilePath )
{}


/***
 * 3. Command processing
 ***/

void Scene::processCommand( const Command& command )
{
    resourcesSyncLibrary_.processCommand( command );
}


} // namespace como
