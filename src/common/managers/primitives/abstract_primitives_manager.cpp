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
#include <iomanip>
#include <ctime>

namespace como {

/***
 * 1. Construction
 ***/

AbstractPrimitivesManager::AbstractPrimitivesManager( std::string sceneDirPath, std::string sceneTempDirPath, LogPtr log ) :
    tempDirPath_( sceneTempDirPath ),
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


PrimitiveInfo AbstractPrimitivesManager::getPrimitiveInfo( ResourceID id ) const
{
    LOCK
    PrimitiveInfo primitiveInfo = primitiveInfo_.at( id );


    return primitiveInfo;
}


/***
 * 3. Getters
 ***/

std::string AbstractPrimitivesManager::getPrimitiveFilePath( ResourceID id ) const
{
    LOCK
    std::string filePath = primitiveInfo_.at( id ).filePath;


    return filePath;
}

std::string AbstractPrimitivesManager::getPrimitiveRelativePath(ResourceID id) const
{
    LOCK
    std::string relPath = primitiveInfo_.at( id ).filePath;
    relPath = relPath.substr( scenePrimitivesDir_.size() );


    return relPath;
}

std::string AbstractPrimitivesManager::getCategoryRelativePath( ResourceID id ) const
{
    std::string categoryRelativePath = categoryNames_.at( id );


    return categoryRelativePath;
}


std::string AbstractPrimitivesManager::getCategoryAbsoluteePath( ResourceID id ) const
{
    std::string categoryAbsolutePath =
            scenePrimitivesDir_ + '/' + getCategoryRelativePath( id );


    return categoryAbsolutePath;
}


ResourcesList AbstractPrimitivesManager::getCategoriesList() const
{
    ResourcesList categoriesList;

    LOCK
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

    log_->debug( "Category created [", scenePrimitivesDir_ + '/' + name, "]\n" );


}


/***
 * 5. Primitives management
 ***/

void AbstractPrimitivesManager::registerPrimitive( ResourceID id, PrimitiveInfo primitive )
{
    log_->debug( "Primitive registered - id (", id,
                 "), name (", primitive.name,
                 ") - category(", primitive.category, ")",
                 ") - file path(", primitive.filePath, ")\n" );

    primitiveInfo_[id] = primitive;
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


/***
 * 8. Auxiliar methods
***/

std::string AbstractPrimitivesManager::getCurrentDateTimeStr()
{
    // Source: http://stackoverflow.com/a/16358264
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime( buffer, 80, "%d-%m-%Y_%I-%M-%S", timeinfo );

    return buffer;
}


} // namespace como
