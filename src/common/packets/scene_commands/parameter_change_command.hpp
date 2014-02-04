/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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

#ifndef PARAMETER_CHANGE_COMMAND_HPP
#define PARAMETER_CHANGE_COMMAND_HPP

#include "scene_command.hpp"

namespace como {

// Available pivot point modes.
enum class PivotPointMode
{
    MEDIAN_POINT = 0,
    INDIVIDUAL_CENTROIDS,
    WORLD_ORIGIN
};

enum class ParameterType
{
    PIVOT_POINT_MODE
};


class ParameterChangeCommand : public SceneCommand
{
    private:
        ParameterType parameterType_;

        union ParameterValue {
            PivotPointMode pivotPointMode_;
        } value_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        ParameterChangeCommand();
        ParameterChangeCommand( UserID userID );
        ParameterChangeCommand( UserID userID, PivotPointMode pivotPointMode );
        ParameterChangeCommand( const ParameterChangeCommand& b );
        ParameterChangeCommand( ParameterChangeCommand&& ) = delete;

        ~ParameterChangeCommand() = default;


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        ParameterType getParameterType() const ;
        PivotPointMode getPivotPointMode() const ;


        /***
         * 4. Setters
         ***/
        void setPivotPointMode( PivotPointMode pivotPointMode );


        /***
         * 5. Operators
         ***/
        ParameterChangeCommand& operator=( const ParameterChangeCommand& ) = delete;
        ParameterChangeCommand& operator=( ParameterChangeCommand&& ) = delete;
};

} // namespace como

#endif // PARAMETER_CHANGE_COMMAND_HPP
