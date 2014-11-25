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

#ifndef COMO_APP_HPP
#define COMO_APP_HPP

#include "scene/scene.hpp"
#include <common/utilities/log.hpp>
#include <memory> // std::shared_ptr

namespace como {

// Available transformation types.
enum class TransformationType
{
    NONE = 0,
    TRANSLATION,
    ROTATION,
    SCALE
};


// Available transformation modes.
enum class TransformationMode
{
    FREE = 0,
    FIXED_X,
    FIXED_Y,
    FIXED_Z
};


class ComoApp : public Lockable {
    public:
        /***
         * 1. Construction
         ***/
        ComoApp() = delete;
        ComoApp( const ComoApp& ) = delete;
        ComoApp( ComoApp&& ) = delete;
        ComoApp( ScenePtr scene, LogPtr log );


        /***
         * 2. Destruction
         ***/
        ~ComoApp() = default;


        /***
         * 3. Getters
         ***/
        TransformationType getTransformationType() const ;
        TransformationMode getTransformationMode() const ;
        LogPtr getLog() const ;
        ScenePtr getScene() const;


        /***
         * 4. Setters
         ***/
        void setTransformationMode( TransformationMode transformationMode );
        void setTransformationType( TransformationType transformationType );


        /***
         * 5. Operators
         ***/
        ComoApp& operator=( const ComoApp& ) = delete ;
        ComoApp& operator=( ComoApp&& ) = delete;


    private:
        ScenePtr scene_;

        // This sets if the current user transformation is free or fixed to a axis.
        TransformationMode transformationMode;

        // Current transformation type (translation, rotation, etc).
        TransformationType transformationType;

        LogPtr log_;
};


typedef std::shared_ptr< ComoApp > ComoAppPtr;

} // namespace como

#endif // COMO_APP_HPP
