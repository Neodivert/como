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

#include "packable_commands_list.hpp"
#include <iostream>

namespace como {

PackableCommandsList::PackableCommandsList( const std::string& unpackingDirPath ) :
    Packable(),
    unpackingDirPath_( unpackingDirPath )
{}


PackableCommandsList::PackableCommandsList( const PackableCommandsList& b ) :
    Packable( b ),
    unpackingDirPath_( b.unpackingDirPath_ )
{
    for( const auto& command : b.commands_ ){
        commands_.push_back( CommandConstPtr( command->clone() ) );
    }

    /*
     * We don't call CompositePackable::addPackable here because we can't
     * predict which types of commands we'll receive every time.
     */
}


/***
 * 3. Packing and unpacking
 ***/

void* PackableCommandsList::pack( void* buffer ) const
{
    CommandsList::const_iterator it;
    PackableUint8< std::uint8_t > nCommands;

    // Retrieve the number of commands and pack it.
    nCommands.setValue( commands_.size() );
    buffer = nCommands.pack( buffer );

    // Pack the commands.
    for( it = commands_.begin(); it != commands_.end(); it++ ){
        buffer = (*it)->pack( buffer );
    }

    return buffer;
}


const void* PackableCommandsList::unpack( const void* buffer )
{
    PackableUint8< std::uint8_t > nCommands;
    CommandPtr command;
    unsigned int i;

    // Unpack the number of commands.
    buffer = nCommands.unpack( buffer );

    // Preread each command's type and cast the command to the appropiate
    // derived type. Then unpack.
    for( i = 0; i < nCommands.getValue(); i++ ){
        switch( Command::getTarget( buffer ) ){
            // User commands
            case CommandTarget::USER:
                switch( UserCommand::getType( buffer ) ){
                    case UserCommandType::USER_CONNECTION:
                        command = CommandPtr( new UserConnectionCommand );
                    break;
                    case UserCommandType::USER_DISCONNECTION:
                        command =  CommandPtr( new UserDisconnectionCommand );
                    break;
                    case UserCommandType::PARAMETER_CHANGE:
                        command = CommandPtr( new UserParameterChangeCommand );
                    break;
                }
            break;

            // Selection commands
            case CommandTarget::SELECTION:
                switch( SelectionCommand::getType( buffer ) ){
                    case SelectionCommandType::SELECTION_TRANSFORMATION:
                        command = CommandPtr( new SelectionTransformationCommand );
                    break;
                }
            break;

            // Primitive commands.
            case CommandTarget::PRIMITIVE:
                switch( PrimitiveCommand::getType( buffer ) ){
                    case PrimitiveCommandType::PRIMITIVE_CREATION:
                        command = CommandPtr( new PrimitiveCreationCommand( unpackingDirPath_ ) );
                    break;
                    case PrimitiveCommandType::PRIMITIVE_INSTANTIATION:
                        command = CommandPtr( new PrimitiveInstantiationCommand );
                    break;
                }
            break;

            // Primitive category commands.
            case CommandTarget::PRIMITIVE_CATEGORY:
                switch( PrimitiveCategoryCommand::getType( buffer ) ){
                    case PrimitiveCategoryCommandType::PRIMITIVE_CATEGORY_CREATION:
                        command = CommandPtr( new PrimitiveCategoryCreationCommand );
                    break;
                }
            break;

            case CommandTarget::MATERIAL:
                switch( MaterialCommand::getType( buffer ) ){
                    case MaterialCommandType::MATERIAL_CREATION:
                        command = CommandPtr( new MaterialCreationCommand );
                    break;
                    case MaterialCommandType::MATERIAL_MODIFICATION:
                        switch( AbstractMaterialModificationCommand::getParameterName( buffer ) ){
                            case MaterialParameterName::COLOR:
                                command = CommandPtr( new MaterialColorChangeCommand );
                            break;
                            case MaterialParameterName::AMBIENT_REFLECTIVITY:
                                command = CommandPtr( new MaterialAmbientReflectivityChangeCommand );
                            break;
                            case MaterialParameterName::DIFFUSE_REFLECTIVITY:
                                command = CommandPtr( new MaterialDiffuseReflectivityChangeCommand );
                            break;
                            case MaterialParameterName::SPECULAR_REFLECTIVITY:
                                command = CommandPtr( new MaterialSpecularReflectivityChangeCommand );
                            break;
                            case MaterialParameterName::SPECULAR_EXPONENT:
                                command = CommandPtr( new MaterialSpecularExponentChangeCommand );
                            break;
                        }
                    break;
                }
            break;

            case CommandTarget::LIGHT:
                switch( LightCommand::getType( buffer ) ){
                    case LightCommandType::LIGHT_CREATION:
                        switch( LightCreationCommand::getLightType( buffer ) ){
                            case LightType::DIRECTIONAL_LIGHT:
                                command = CommandPtr( new DirectionalLightCreationCommand );
                            break;
                        }
                    break;
                    case LightCommandType::LIGHT_COLOR_CHANGE:
                        command = CommandPtr( new LightColorChangeCommand );
                    break;
                    case LightCommandType::LIGHT_AMBIENT_COEFFICIENT_CHANGE:
                        command = CommandPtr( new LightAmbientCoefficientChangeCommand );
                    break;
                    case LightCommandType::LIGHT_CREATION_RESPONSE:
                        command = CommandPtr( new LightCreationResponseCommand );
                    break;
                }
            break;

            case CommandTarget::RESOURCE:
                switch( ResourceCommand::getType( buffer ) ){
                    case ResourceCommandType::RESOURCE_LOCK:
                        command = CommandPtr( new ResourceCommand( ResourceCommandType::RESOURCE_LOCK ) );
                    break;
                    case ResourceCommandType::RESOURCE_SELECTION_RESPONSE:
                        command = CommandPtr( new ResourceSelectionResponse );
                    break;
                }
            break;

            case CommandTarget::RESOURCES_SELECTION:
                switch( ResourcesSelectionCommand::getType( buffer ) ){
                    case ResourcesSelectionCommandType::SELECTION_UNLOCK:
                        command = CommandPtr( new ResourcesSelectionCommand( ResourcesSelectionCommand::getType( buffer ) ) );
                    break;
                    case ResourcesSelectionCommandType::SELECTION_DELETION:
                        command = CommandPtr( new ResourcesSelectionCommand( ResourcesSelectionCommandType::SELECTION_DELETION ) );
                    break;
                }
            break;

            case CommandTarget::GEOMETRIC_PRIMITIVE:
                switch( GeometricPrimitiveCommand::getType( buffer ) ){
                    case GeometricPrimitiveCommandType::CUBE_CREATION:
                        command = CommandPtr( new CubeCreationCommand );
                    break;
                    case GeometricPrimitiveCommandType::CONE_CREATION:
                        command = CommandPtr( new ConeCreationCommand );
                    break;
                    case GeometricPrimitiveCommandType::CYLINDER_CREATION:
                        command = CommandPtr( new CylinderCreationCommand );
                    break;
                    case GeometricPrimitiveCommandType::SPHERE_CREATION:
                        command = CommandPtr( new SphereCreationCommand );
                    break;
                }
            break;

            case CommandTarget::TEXTURE:
                switch( TextureCommand::getType( buffer ) ){
                    case TextureCommandType::TEXTURE_CREATION:
                        command = CommandPtr( new TextureCreationCommand( unpackingDirPath_ ) );
                    break;
                }
            break;

            case CommandTarget::TEXTURE_WALL:
                switch( TextureWallCommand::getType( buffer ) ){
                    case TextureWallCommandType::TEXTURE_CHANGE:
                        command = CommandPtr( new TextureWallTextureChangeCommand );
                    break;
                    case TextureWallCommandType::TEXTURE_WALL_MODIFICATION:
                        command = CommandPtr( new TextureWallModificationCommand );
                    break;
                }
            break;

            default:
                int commandTarget = static_cast< int >( Command::getTarget( buffer ) );
                throw std::runtime_error( "Received an unrecognized command type" +
                                          std::to_string( commandTarget ) +
                                          ")" );
            break;
        }

        buffer = command->unpack( buffer );
        commands_.push_back( std::move( command ) );
    }

    return buffer;
}


const void* PackableCommandsList::unpack( const void* buffer ) const
{
    const PackableUint8< std::uint8_t > nCommands( commands_.size() );
    CommandsList::const_iterator it;

    // Unpack the number of commands.
    buffer = nCommands.unpack( buffer );

    // Unpack the commands.
    for( it = commands_.begin(); it != commands_.end(); it++ ){
        buffer = (*it)->unpack( buffer );
    }

    return buffer;
}


/***
 * 4. Getters
 ***/

PacketSize PackableCommandsList::getPacketSize() const
{
    CommandsList::const_iterator it;

    // The size of the list (Uint8) is packed / unpacked along with the list
    // itself. Sum its size (in bytes) to the total packet size.
    PacketSize packetSize = sizeof( std::uint8_t );

    // Sum the packet size of every command present in this list to the total
    // packet size.
    for( it = commands_.begin(); it != commands_.end(); it++ ){
        packetSize += (*it)->getPacketSize();
    }

    return packetSize;
}


const CommandsList* PackableCommandsList::getCommands() const
{
    return &commands_;
}


/***
 * 5. Commands management
 ***/

void PackableCommandsList::addCommand( CommandConstPtr command )
{
    commands_.push_back( std::move( command ) );
}


void PackableCommandsList::clear()
{
    commands_.clear();
}


} // namespace como
