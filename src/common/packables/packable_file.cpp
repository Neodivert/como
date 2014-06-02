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

#include "packable_file.hpp"

namespace como {


/***
 * 1. Construction
 ***/

PackableFile::PackableFile()
{
    // "Register" the filePath and the fileSize as part of this
    // CompositePackable (for automatic packing / unpacking when calling
    // CompositePackable packing / unpacking methods).
    // The file content's are packed / unpacked explicitly in this class.
    addPackable( &filePath_ );
    addPackable( &fileSize_ );
}


PackableFile::PackableFile( const PackableString<NAME_SIZE>& filePath, bool createFile ) :
    CompositePackable(),
    filePath_( filePath ),
    fileSize_( 0 )
{
    // Check that we can create or open the file, as requested.
    if( createFile ){
        file_.open( filePath_.getValue(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc );

        if( !file_.is_open() ){
            throw std::runtime_error( std::string( "Couldn't create file [" ) + filePath_.getValue() + "]" );
        }

        file_.close();
    }else{
        file_.open( filePath_.getValue(), std::ios_base::in | std::ios_base::binary );

        if( !file_.is_open() ){
            throw std::runtime_error( std::string( "Couldn't open file [" ) + filePath_.getValue() + "]" );
        }

        file_.close();

        // Compute file size.
        updateFileSize();
    }


    // "Register" the filePath and the fileSize as part of this
    // CompositePackable (for automatic packing / unpacking when calling
    // CompositePackable packing / unpacking methods).
    // The file content's are packed / unpacked explicitly in this class.
    addPackable( &filePath_ );
    addPackable( &fileSize_ );
}


PackableFile::PackableFile( const PackableFile& b ) :
    CompositePackable( b ),
    filePath_( b.filePath_ ),
    fileSize_( b.fileSize_ )
{
    // Check that we can open the file.
    file_.open( filePath_.getValue(), std::ios_base::in | std::ios_base::binary );

    if( !file_.is_open() ){
        throw std::runtime_error( std::string( "Couldn't open file [" ) + filePath_.getValue() + "]" );
    }

    file_.close();

    // "Register" the filePath and the fileSize as part of this
    // CompositePackable (for automatic packing / unpacking when calling
    // CompositePackable packing / unpacking methods).
    // The file content's are packed / unpacked explicitly in this class.
    addPackable( &filePath_ );
    addPackable( &fileSize_ );
}


/***
 * 3. Packing and unpacking
 ***/

void* PackableFile::PackableFile::pack( void* buffer ) const
{
    char* castedBuffer = nullptr;
    char errorMessage[128];

    // Pack the file path and size, among other packables held by this
    // CompositePackable (parent class), into the given buffer.
    buffer = CompositePackable::pack( buffer );

    // Pack the file contents into the given buffer.
    castedBuffer = static_cast< char* >( buffer );

    file_.open( filePath_.getValue(), std::ios_base::in | std::ios_base::binary );
    file_.read( castedBuffer, fileSize_.getValue() );

    if( !file_ ){
        sprintf( errorMessage, "ERROR unpacking file. Only %ld bytes could be read", file_.gcount() );
        throw std::runtime_error( errorMessage );
    }

    file_.close();

    // Increment buffer pointer to the first position after packed file contents.
    castedBuffer += fileSize_.getValue();

    // Return a pointer to the next free position in the buffer.
    return static_cast< void* >( castedBuffer );
}


const void* PackableFile::unpack( const void* buffer )
{
    const char* castedBuffer = nullptr;
    char errorMessage[128];
    char consoleCommand[128];
    std::string filePathBaseName;

    // Unpack the file path and size, among other packables held by this
    // CompositePackable (parent class), from the given buffer.
    buffer = CompositePackable::unpack( buffer );

    // Maybe the file being unpacked will be placed in a directory which
    // doesn't exist. Create it.
    filePathBaseName = filePath_.getValue();
    filePathBaseName = filePathBaseName.substr( 0, filePathBaseName.rfind( '/' ) );

    sprintf( consoleCommand, "mkdir -p \"%s\"", filePathBaseName.c_str() );
    system( consoleCommand );

    // Unpack the file contents from the given buffer and write them to file.
    castedBuffer = static_cast< const char* >( buffer );

    file_.open( filePath_.getValue(), std::ios_base::out | std::ios_base::binary );

    if( !file_.is_open() ){
        sprintf( errorMessage, "ERROR creating file [%s] for unpacking", filePath_.getValue() );
        throw std::runtime_error( errorMessage );
    }

    file_.write( castedBuffer, fileSize_.getValue() );

    if( !file_ ){
        sprintf( errorMessage, "ERROR unpacking file [%s]", filePath_.getValue() );
        throw std::runtime_error( errorMessage );
    }

    file_.close();

    // Increment buffer pointer to the first position after packed file contents.
    castedBuffer += fileSize_.getValue();

    // Return a pointer to the next free position in the buffer.
    return static_cast< const void*>( castedBuffer );
}


const void* PackableFile::unpack( const void* buffer ) const
{
    const char* castedBuffer = nullptr;
    char* fileContents = nullptr;
    char errorMessage[128];

    // Unpack the file path and size, among other packables held by this
    // CompositePackable (parent class), from the given buffer.
    buffer = CompositePackable::unpack( buffer );

    // Read the file contents for checking if they match the buffer contents.
    fileContents = new char[fileSize_.getValue()];

    file_.open( filePath_.getValue(), std::ios_base::in | std::ios_base::binary );
    file_.read( fileContents, fileSize_.getValue() );

    if( !file_ ){
        sprintf( errorMessage, "ERROR when unpacking file (const)" );
        throw std::runtime_error( errorMessage );
    }

    file_.close();

    // Unpack data from the given buffer and check if it matches the file contents.
    castedBuffer = static_cast< const char* >( buffer );

    if( strncmp( fileContents, castedBuffer, fileSize_.getValue() ) ){
        throw std::runtime_error( "ERROR unpacked data doesn't match file contents (const)" );
    }

    // Free allocated resources.
    delete [] fileContents;

    // Return a pointer to the next free position in the buffer.
    castedBuffer += fileSize_.getValue();
    return static_cast< const void* >( castedBuffer );
}


/***
 * 4. Getters
 ***/

PacketSize PackableFile::getPacketSize() const
{
    return CompositePackable::getPacketSize() + getFileSize();
}


std::uint32_t PackableFile::getFileSize() const
{
    return fileSize_.getValue();
}


const PackableString< NAME_SIZE >* PackableFile::getFilePath() const
{
    return &filePath_;
}


/***
 * 6. Auxiliar methods
 ***/

void PackableFile::updateFileSize()
{
    // Open the file and set the cursor at the end.
    file_.open( filePath_.getValue(), std::ios_base::in | std::ios_base::binary | std::ios_base::ate );

    if( !file_.is_open() ){
        throw std::runtime_error( std::string( "Couldn't open file [" ) + filePath_.getValue() + "]" );
    }


    // Update the file size and close the file.
    fileSize_.setValue( static_cast< std::uint32_t >( file_.tellg() ) );
    file_.close();
}

} // namespace como
