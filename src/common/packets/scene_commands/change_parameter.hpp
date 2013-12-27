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

#ifndef CHANGE_PARAMETER_HPP
#define CHANGE_PARAMETER_HPP

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


class ChangeParameter : public SceneCommand
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
        ChangeParameter();
        ChangeParameter( UserID userID );
        ChangeParameter( UserID userID, PivotPointMode pivotPointMode );
        ChangeParameter( const ChangeParameter& b );

        virtual ~ChangeParameter(){}


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
};

} // namespace como

#endif // CHANGE_PARAMETER_HPP
