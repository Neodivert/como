/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef SELECTION_RESPONSE_HPP
#define SELECTION_RESPONSE_HPP

#include "scene_command.hpp"

namespace como {

const std::uint8_t MAX_SELECTION_CONFIRMATIONS = 32;

class SelectionResponse : public SceneCommand
{
    private:
        std::uint8_t nSelections_;

        // Bits flag: the i-th bit indicates whether the i-th selection sent
        // by the client is confirmed (1) or denied (0).
        std::uint32_t selectionConfirmed_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        SelectionResponse();
        SelectionResponse( const SelectionResponse& b );

        virtual ~SelectionResponse(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        std::uint8_t getNSelections() const ;
        std::uint32_t getSelectionConfirmed() const ;


        /***
         * 4. Setters
         ***/
        void addSelectionConfirmation( bool confirmed );
        void clear();
};

typedef std::shared_ptr< SelectionResponse > SelectionResponsePtr;
typedef std::shared_ptr< const SelectionResponse > SelectionResponseConstPtr;

} // namespace como

#endif // SELECTION_RESPONSE_HPP
