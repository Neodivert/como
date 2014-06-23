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

#include "abstract_primitives_manager.hpp"
#include <boost/filesystem.hpp>
#include <common/utilities/paths.hpp>

namespace como {

/***
 * 1. Construction
 ***/

AbstractPrimitivesManager::AbstractPrimitivesManager( std::string sceneDirPath, LogPtr log ) :
    log_( log )
{
    boost::system::error_code errorCode;

    // Build the path to the scene's primitives directory.
    scenePrimitivesDir_ = sceneDirPath + "/primitives";

    // Build the scene's primitives directory.
    boost::filesystem::create_directories( scenePrimitivesDir_, errorCode );
    if( errorCode ){
        throw std::runtime_error( errorCode.message() );
    }

    log_->debug( "Scene primitives dir [", scenePrimitivesDir_, "] created\n" );
}


/***
 * 2. Destruction
 ***/

AbstractPrimitivesManager::~AbstractPrimitivesManager()
{
    log_->debug( "Removing scene primitives dir [", scenePrimitivesDir_, "]\n" );
    boost::filesystem::remove_all( scenePrimitivesDir_ );
}


/***
 * 3. Getters
 ***/

std::string AbstractPrimitivesManager::getPrimitiveRelativePath( ResourceID id, PrimitiveComponent component ) const
{
    PrimitiveInfo primitive = primitiveInfo_.at( id );
    std::string categoryRelativePath = getCategoryRelativePath( primitive.category );

    switch( component ){
        case PrimitiveComponent::MESH:
            return categoryRelativePath + "/" + primitive.meshFileName;
        break;
        case PrimitiveComponent::MATERIAL:
            return categoryRelativePath + "/" + primitive.materialFileName;
        break;
        default:
            throw std::runtime_error( "AbstractPrimitivesManager::getPrimitiveRelativePath - what?" );
        break;
    }
}

std::string AbstractPrimitivesManager::getPrimitiveAbsolutePath( ResourceID id, PrimitiveComponent component ) const
{
    return scenePrimitivesDir_ + '/' + getPrimitiveRelativePath( id, component );
}


std::string AbstractPrimitivesManager::getCategoryRelativePath( ResourceID id ) const
{
    return categoryNames_.at( id );
}


std::string AbstractPrimitivesManager::getCategoryAbsoluteePath( ResourceID id ) const
{
    return scenePrimitivesDir_ + '/' + getCategoryRelativePath( id );
}


ResourcesList AbstractPrimitivesManager::getCategoriesList() const
{
    ResourcesList categoriesList;

    for( auto category : categoryNames_ ){
        categoriesList.push_back( std::pair< ResourceID, std::string >( category.first, category.second ) );
    }

    return categoriesList;
}


/***
 * 4. Categories management
 ***/

void AbstractPrimitivesManager::registerCategory( ResourceID id, std::string name )
{
    if( categoryNameInUse( name ) ){
        throw std::runtime_error( std::string( "Category name [" ) +
                                  name +
                                  "] already in use" );
    }

    categoryNames_[ id ] = name;
}


void AbstractPrimitivesManager::createCategory( ResourceID id, std::string name )
{
    boost::system::error_code errorCode;

    AbstractPrimitivesManager::registerCategory( id, name );

    boost::filesystem::create_directory( scenePrimitivesDir_ + '/' + name, errorCode );
    if( errorCode ){
        throw std::runtime_error( errorCode.message() );
    }
}


/***
 * 5. Primitives management
 ***/

void AbstractPrimitivesManager::registerPrimitive( ResourceID id, ResourceID category, std::string meshFileName, std::string materialFileName )
{
    std::string name = meshFileName;

    log_->debug( "Primitive registered - id (", id,
                 "), name (", name,
                 ") - category(", category, ")",
                 ") - meshFileName(", meshFileName, ")",
                 ") . materialFileName(", materialFileName, ")\n" );

    primitiveInfo_[id] = PrimitiveInfo( { name, category, meshFileName, materialFileName } );
}


/***
 * 6. Auxiliar methods
 ***/

bool AbstractPrimitivesManager::categoryNameInUse( std::string categoryName ) const
{
    for( auto categoryPair : categoryNames_ ){
        if( categoryPair.second == categoryName ){
            return true;
        }
    }
    return false;
}


} // namespace como
