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

#ifndef PACKABLE_FILE_HPP
#define PACKABLE_FILE_HPP

#include "composite_packable.hpp"
#include "packable_string.hpp"
#include "packable_uint32.hpp"
#include <fstream>
#include <boost/filesystem.hpp>

namespace como {

/*!
 * \class PackableFile
 *
 * \brief Wrapping class which contains a file whose data can be packed into
 * or unpacked from a given buffer.
 */
class PackableFile : public CompositePackable {
    private:   
        static int fileCounter;

        /*! \brief Path of the directory where this file will be unpacked */
        std::string unpackingDirPath_;

        /*! \brief Path to the file to be packed / unpacked */
        std::string filePath_;

        /*! \brief Size of the file to be packed / unpacked */
        PackableUint32< std::uint32_t > fileSize_;

        /*! \brief File to be packed / unpacked */
        mutable std::fstream file_;


    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        PackableFile() = delete;

        /*! \brief  */
        PackableFile( const std::string& unpackingDirPath );

        /*!
         * \brief Constructor. Wraps a file (or created, if requested),
         * making it packable.
         */
        PackableFile( const std::string& unpackingDirPath, const std::string& filePath, bool createFile = false );

        /*! brief Copy constructor. */
        PackableFile( const PackableFile& b );

        /*! \brief Move constructor. */
        PackableFile( PackableFile&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor. */
        virtual ~PackableFile(){}


        /***
         * 3. Packing and unpacking
         ***/

        /*!
         * \brief Packs the file held by this instance into the given buffer.
         * \param buffer a pointer to the buffer where we'll pack the file
         * into.
         * \return a poiner to the next position in the given buffer.
         */
        virtual void* pack( void* buffer ) const;


        /*!
         * \brief Unpacks the file held by this instance from the given buffer.
         * \param buffer a pointer to the buffer the data will be unpacked
         * from.
         * \return a pointer to the next free position in the given buffer.
         */
        virtual const void* unpack( const void* buffer );


        /*!
         * \brief Testing unpacking - Analogous to unpack(), but this one
         * throws an exception if the unpacked data doesn't match the object's
         * data.
         * \param buffer a pointer to the buffer the data are unpacked from.
         * \return a pointer to the next position in the given buffer.
         */
        virtual const void* unpack( const void* buffer ) const ;


        /***
         * 4. Getters
         ***/

        /*!
         * \brief returns the size (in bytes) that the data held by this
         * PackableFile would ocuppy once packed.
         * \return an integer with the size (in bytes) that this object
         * would ocuppy once packed.
         */
        virtual PacketSize getPacketSize() const;

        /*!
         * \brief Returns the size (in bytes) that the file held by this
         * PackableFile would ocuppy once packed.
         */
        std::uint32_t getFileSize() const;


        std::string getFileName() const;

        /*!
         * \brief Returns a pointer to the path of the primitive's
         * specification file.
         */
        std::string getFilePath() const ;


        /***
         * 6. Auxiliar methods
         ***/
    protected:
        /*! \brief Update the fileSize attribute. Used when initializing. */
        void updateFileSize();

        std::string generateUnpackedFilePath() const;


    public:
        /***
         * 7. Operators
         ***/

        /*! \brief Copy assignment operator */
        PackableFile& operator = (const PackableFile& b);

        /*! \brief Move assignment operator */
        PackableFile& operator = ( PackableFile&& ) = delete;
};

} // namespace como

#endif // PACKABLE_FILE_HPP
