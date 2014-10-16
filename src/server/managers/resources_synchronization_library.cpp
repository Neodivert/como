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

#include "resources_synchronization_library.hpp"
#include <common/commands/commands.hpp>
#include <server/sync_data/texture_sync_data.hpp>
#include <server/sync_data/entity_sync_data.hpp>
#include <server/sync_data/texture_wall_sync_data.hpp>
#include <server/sync_data/material_sync_data.hpp>
#include <common/commands/commands_file_parser.hpp>

namespace como {

/***
 * 1. Construction
 ***/

ResourcesSynchronizationLibrary::ResourcesSynchronizationLibrary( CommandsHistoricPtr commandsHistoric,
                                                                  UsersMap& users,
                                                                  const std::string &sceneDirPath,
                                                                  const std::string &tempDirPath,
                                                                  ResourceIDsGeneratorPtr resourceIDsGenerator,
                                                                  LogPtr log ) :
    AbstractResourcesOwnershipManager( log ),
    commandsHistoric_( commandsHistoric ),
    unpackingDirPath_( tempDirPath ),
    users_( users ),
    primitivesManager_( sceneDirPath,
                        tempDirPath,
                        commandsHistoric_,
                        log,
                        resourceIDsGenerator )
{}


/***
 * 3. Command execution
 ***/

void ResourcesSynchronizationLibrary::processCommand( const Command &command )
{
    lock();
    log()->debug( "Processing command (target: ",
                  commandTargetStrings[(int)( command.getTarget())],
                  ")\n" );
    switch( command.getTarget() ){
        case CommandTarget::RESOURCE:{
            const ResourceCommand& resourceCommand = dynamic_cast< const ResourceCommand& >( command );
            executeResourceCommand( resourceCommand );
        }break;
        case CommandTarget::RESOURCES_SELECTION:{
            const ResourcesSelectionCommand& resourcesSelectionCommand =
                    dynamic_cast< const ResourcesSelectionCommand& >( command );
            executeResourcesSelectionCommand( resourcesSelectionCommand );
        }break;
        case CommandTarget::TEXTURE:{
            const TextureCommand& textureCommand =
                    dynamic_cast< const TextureCommand& >( command );

            if( textureCommand.getType() == TextureCommandType::TEXTURE_CREATION ){
                resourcesSyncData_[ textureCommand.textureID() ] =
                        ResourceSyncDataPtr(
                            new TextureSyncData( &textureCommand,
                                                 textureCommand.textureID() ) );
            }else{
                resourcesSyncData_.at( textureCommand.textureID() )->processCommand( command );
            }
        }break;
        case CommandTarget::GEOMETRIC_PRIMITIVE:{
            const GeometricPrimitiveCommand& geometricPrimitiveCommand =
                    dynamic_cast< const GeometricPrimitiveCommand& >( command );

            if( ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::CUBE_CREATION ) ||
                ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::CONE_CREATION ) ||
                ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::CYLINDER_CREATION ) ||
                ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::SPHERE_CREATION ) ){
                log()->debug( "Geometric primitive created (", geometricPrimitiveCommand.getMeshID(), ")\n" );
                resourcesSyncData_[ geometricPrimitiveCommand.getMeshID() ] =
                        ResourceSyncDataPtr( new EntitySyncData( &geometricPrimitiveCommand,
                                                                 geometricPrimitiveCommand.getMeshID(),
                                                                 geometricPrimitiveCommand.centroid() ) );

                // TODO: Synchronize texture walls names.
                ResourceID textureWallID = geometricPrimitiveCommand.getFirstTextureWallID();
                for( unsigned int i = 0; i < geometricPrimitiveCommand.nTextureWalls(); i++ ){
                    resourcesSyncData_[textureWallID] =
                            ResourceSyncDataPtr(
                                new TextureWallSyncData( nullptr,
                                                         textureWallID ) );

                    resourcesSyncData_.at( geometricPrimitiveCommand.getMeshID() )->addChildResource( textureWallID );

                    textureWallID++;
                }

                // TODO: Synchronize material names.
                resourcesSyncData_[geometricPrimitiveCommand.getMaterialID()] =
                        ResourceSyncDataPtr(
                            new MaterialSyncData( nullptr,
                                                  geometricPrimitiveCommand.getMaterialID() ) );

                resourcesSyncData_.at( geometricPrimitiveCommand.getMeshID() )->addChildResource( geometricPrimitiveCommand.getMaterialID() );
            }else{
                resourcesSyncData_.at( geometricPrimitiveCommand.getMeshID() )->processCommand( command );
            }
        }break;
        case CommandTarget::PRIMITIVE:{
            const PrimitiveCommand& primitiveCommand =
                    dynamic_cast< const PrimitiveCommand& >( command );

            switch( primitiveCommand.getType() ){
                case PrimitiveCommandType::PRIMITIVE_CREATION:{
                    // PRIMITIVE_CREATION command received, cast its pointer.
                    const PrimitiveCreationCommand& primitiveCreationCommand =
                            dynamic_cast< const PrimitiveCreationCommand& >( command );

                    primitivesManager_.registerPrimitive( primitiveCreationCommand.getPrimitiveInfo(),
                                                          primitiveCreationCommand.getPrimitiveID() );

                    // We also register the primitive creation command here
                    // for saving / loading it along with the scene.
                    resourcesSyncData_[primitiveCreationCommand.getPrimitiveID()] =
                            ResourceSyncDataPtr(
                                new ResourceSyncData(
                                    &primitiveCreationCommand,
                                    primitiveCreationCommand.getPrimitiveID() ) );

                    // TODO: Complete, Save new primitive (Move it from temp to category directory).

                    // primitivesManager_.registerPrimitive() already inserts
                    // the creation command into the historic, so return for
                    // avoiding double insertion.
                    return;
                }break;
                case PrimitiveCommandType::PRIMITIVE_INSTANTIATION:{
                    const PrimitiveInstantiationCommand& primitiveCommand =
                            dynamic_cast< const PrimitiveInstantiationCommand& >( command );

                    // Add a node to the Drawable Owners map for the recently added
                    // drawable. Mark it with a 0 (no owner).
                    log()->debug( "Primitive instantiated (", primitiveCommand.getMeshID(), ")\n" );
                    resourcesSyncData_[ primitiveCommand.getMeshID() ] =
                        ResourceSyncDataPtr(
                                new EntitySyncData( &primitiveCommand,
                                                    primitiveCommand.getMeshID(),
                                                    primitiveCommand.centroid() ) );

                    // Synchronize new mesh's materials.
                    std::list<PlainMaterialData> primitiveMaterials =
                            primitivesManager_.primitivePlainMaterialsData( primitiveCommand.getPrimitiveID() );

                    ResourceID materialID = primitiveCommand.getMaterialID();

                    for( const PlainMaterialData& materialData : primitiveMaterials ){
                        resourcesSyncData_[ materialID ] =
                            ResourceSyncDataPtr(
                                    new MaterialSyncData( materialID,
                                                          materialData ) );

                        resourcesSyncData_.at( primitiveCommand.getMeshID() )->addChildResource( materialID );

                        materialID++;
                    }

                    log()->debug( "Mesh added! (", (int)( primitiveCommand.getMeshID().getCreatorID() ),
                                 ", ", (int)( primitiveCommand.getMeshID().getResourceIndex() ), "\n" );
                }break;
            }
        }break;
        case CommandTarget::CAMERA:{
            const CameraCommand& cameraCommand =
                    dynamic_cast< const CameraCommand& >( command );

            if( cameraCommand.getType() == CameraCommandType::CAMERA_CREATION ){
                // TODO: Retrieve real centroid from command.
                resourcesSyncData_[ cameraCommand.cameraID() ] =
                    ResourceSyncDataPtr( new EntitySyncData( &cameraCommand,
                                                             cameraCommand.cameraID(),
                                                             glm::vec3( 0.0f ) ) );
                undeletableResources_.insert( cameraCommand.cameraID() );
            }
        }break;
        case CommandTarget::SELECTION:{
            // We have a command that updates the user's selection, so apply
            // it to all resources currently owned by user.
            for( std::pair< const ResourceID, ResourceSyncDataPtr >& resourcePair : resourcesSyncData_ ){
                if( resourcePair.second->resourceOwner() == command.getUserID() ){
                    resourcePair.second->processCommand( command );
                }
            }
        }break;
        case CommandTarget::ENTITY:{
            const EntityCommand& entityCommand =
                    dynamic_cast< const EntityCommand& >( command );
            resourcesSyncData_.at( entityCommand.entityID() )->processCommand( entityCommand );
        }break;
        case CommandTarget::MATERIAL:{
            const MaterialCommand& materialCommand =
                    dynamic_cast< const MaterialCommand& >( command );
            resourcesSyncData_.at( materialCommand.getMaterialID() )->processCommand( materialCommand );
        }break;
        case CommandTarget::LIGHT:{
            const LightCommand& lightCommand = dynamic_cast< const LightCommand& >( command );

            if( lightCommand.getType() == LightCommandType::LIGHT_CREATION ){
                // Request the creation of the light to the lights manager.
                if( lights_.size() < MAX_LIGHTS ){
                    lights_.insert( lightCommand.getResourceID() );

                    resourcesSyncData_[ lightCommand.getResourceID() ] =
                            ResourceSyncDataPtr(
                                new EntitySyncData(
                                    &lightCommand,
                                    lightCommand.getResourceID(),
                                    glm::vec3( 0.0f ) ) ); // TODO: Retrieve centroid from command.

                    // If the user who originally made the request exists in
                    // the system, send him / her a response command.
                    if( users_.count( lightCommand.getUserID() ) ){
                        users_.at( lightCommand.getUserID() )->addResponseCommand(
                                    CommandConstPtr(
                                        new LightCreationResponseCommand( lightCommand.getResourceID(),
                                                                          true
                                                                          ) ) );
                    }
                }else{
                    // If the user who originally made the request exists in
                    // the system, send him / her a response command.
                    if( users_.count( lightCommand.getUserID() ) ){
                        users_.at( lightCommand.getUserID() )->addResponseCommand(
                                    CommandConstPtr(
                                        new LightCreationResponseCommand( lightCommand.getResourceID(),
                                                                          false
                                                                          ) ) );
                    }
                    // If the request was denied, return from this method so
                    // the creation command received from user isn't added to
                    // the commands historic.
                    return;
                }
            }
        }break;
        case CommandTarget::TEXTURE_WALL:{
            const TextureWallCommand& textureWallCommand =
                    dynamic_cast< const TextureWallCommand& >( command );
            resourcesSyncData_.at( textureWallCommand.textureWallID() )->processCommand( textureWallCommand );
        }
        default:
            // TODO: Complete
        break;
    }

    commandsHistoric_->addCommand( CommandConstPtr( command.clone() ) );
}


/***
 * 4. File management
 ***/

void ResourcesSynchronizationLibrary::saveToFile( std::ofstream& file ) const
{
    lock();
    CommandsFileParser fileParser( unpackingDirPath_ );

    // First pass: write creation commands to file.
    for( const auto& resourceSyncDataPair : resourcesSyncData_ ){
        if( resourceSyncDataPair.second->getCreationCommand() != nullptr ){
            fileParser.writeCommand( *( resourceSyncDataPair.second->getCreationCommand() ),
                                     file );
        }
    }

    // Second pass: write update commands to file.
    for( const auto& resourceSyncDataPair : resourcesSyncData_ ){
        CommandsList updateCommands = resourceSyncDataPair.second->generateUpdateCommands();
        for( const auto& command : updateCommands ){
            fileParser.writeCommand( *command, file );
        }
    }
}


void ResourcesSynchronizationLibrary::readFromFile( std::ifstream& file )
{
    lock();
    CommandsFileParser fileParser( unpackingDirPath_ );
    CommandPtr command;

    while( ( command = fileParser.readNextCommand( file ) ) != nullptr ){
        processCommand( *command );
    }
}


/***
 * 3. Resources registation
 ***

void ResourcesSynchronizationLibrary::registerResource(const ResourceID& resourceID, UserID ownerID, bool deletable )
{
    lock();

    resourcesOwnershipMap_[ resourceID ] = ownerID;

    if( !deletable ){
        undeletableResources_.insert( resourceID );
    }

    notifyElementInsertion( resourceID );
}
*/

/***
 * 5. Owners management
 ***/

void ResourcesSynchronizationLibrary::removeUser( UserID userID )
{
    lock();

    unlockResourcesSelection( userID );
}


/***
 * 6. Resources ownership management
 ***/

void ResourcesSynchronizationLibrary::lockResource( const ResourceID& resourceID, UserID userID )
{
    lock();
    log()->debug( "User (", userID, ") tries to lock resource (", resourceID, "): " );
    if( resourcesSyncData_.at( resourceID )->resourceOwner() == NO_USER ){
        resourcesSyncData_.at( resourceID )->setResourceOwner( userID );
        //notifyElementUpdate( resourceID );
        users_.at( userID )->addResponseCommand( CommandConstPtr( new ResourceSelectionResponse( resourceID, true ) ) );
        log()->debug( "Yes!\n" );
    }else{
        users_.at( userID )->addResponseCommand( CommandConstPtr( new ResourceSelectionResponse( resourceID, false ) ) );
        log()->debug( "No, resource already locked! :'-(\n" );
    }
}


void ResourcesSynchronizationLibrary::unlockResourcesSelection( UserID userID )
{
    lock();
    log()->debug( "(User: ", userID, ") Unlocking Selection\n" );
    for( auto& resourceSyncData : resourcesSyncData_ ){
        if( resourceSyncData.second->resourceOwner() == userID ){
            resourceSyncData.second->setResourceOwner( NO_USER );
        }
    }
}

void ResourcesSynchronizationLibrary::deleteResourcesSelection( UserID userID )
{
    lock();
    log()->debug( "(User: ", userID, ") Deleting Selection\n" );
    std::map< ResourceID, ResourceSyncDataPtr >::iterator currentElement;

    currentElement = resourcesSyncData_.begin();
    while( currentElement != resourcesSyncData_.end() ){
        if( lights_.count( currentElement->first ) ){
            lights_.erase( currentElement->first );
        }

        if( currentElement->second->resourceOwner() == userID ){
            deleteResource( currentElement->first );

            // deleteResource() removes resources recursively, so this
            // iterator could end invalidated. Start again from the
            // begining.
            // TODO: Optimize this!.
            currentElement = resourcesSyncData_.begin();
        }else{
            currentElement++;
        }
    }
}


void ResourcesSynchronizationLibrary::deleteResource( const ResourceID &resourceID )
{
    ResourceSyncData& resourceSyncData =
            *( resourcesSyncData_.at( resourceID ) );

    if( !undeletableResources_.count( resourceID ) ){
        // Delete first the children of the resource.
        for( const ResourceID& childResourceID : resourceSyncData.childResourceIDs() ){
            deleteResource( childResourceID );
        }

        //notifyElementDeletion( currentElement->first );
        resourcesSyncData_.erase( resourceID );
    }else{
        //notifyElementUpdate( currentElement->first );
        resourceSyncData.setResourceOwner( NO_USER );
    }
}


void ResourcesSynchronizationLibrary::processLockResponse( const ResourceID& resourceID, bool lockResponse )
{
    lock();
    // TODO: Make this trick unnecessary.
    (void)( resourceID );
    (void)( lockResponse );
    throw std::runtime_error( "ResourcesOwnershipManager::processLockResponse called!" );
}


} // namespace como
