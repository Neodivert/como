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

#ifndef SELECTION_RESPONSE_COMMAND_HPP
#define SELECTION_RESPONSE_COMMAND_HPP

#include "selection_command.hpp"

namespace como {

const std::uint8_t MAX_SELECTION_CONFIRMATIONS = 32;

class SelectionResponseCommand : public SelectionCommand
{
    private:
        PackableUint8< std::uint8_t > nSelections_;

        // Bits flag: the i-th bit indicates whether the i-th selection sent
        // by the client is confirmed (1) or denied (0).
        PackableUint32< std::uint32_t > selectionConfirmed_;


    public:
        /***
         * 1. Construction
         ***/
        SelectionResponseCommand();
        SelectionResponseCommand( const SelectionResponseCommand& b );
        SelectionResponseCommand( SelectionResponseCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SelectionResponseCommand() = default;


        /***
         * 3. Getters
         ***/
        std::uint8_t getNSelections() const ;
        std::uint32_t getSelectionConfirmed() const ;


        /***
         * 4. Setters
         ***/
        void addSelectionConfirmation( bool confirmed );
        void clear();


        /***
         * 5. Operators
         ***/
        SelectionResponseCommand& operator=( const SelectionResponseCommand& ) = delete;
        SelectionResponseCommand& operator=( SelectionResponseCommand&& ) = delete;
};

typedef std::shared_ptr< SelectionResponseCommand > SelectionResponseCommandPtr;
typedef std::shared_ptr< const SelectionResponseCommand > SelectionResponseCommandConstPtr;

} // namespace como

#endif // SELECTION_RESPONSE_COMMAND_HPP
