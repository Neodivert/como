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

#ifndef DRAWABLES_SELECTION_HPP
#define DRAWABLES_SELECTION_HPP

#include <client/models/3d/drawable.hpp>
#include <client/models/3d/mesh.hpp>
#include <common/utilities/ids.hpp>
#include <common/commands/user_commands/parameter_change_command.hpp> // For PivotPointMode type.
#include <map>
#include <functional>
#include <mutex>
#include <QObject>
#include <client/models/utilities/changeable/changeable.hpp>

namespace como {

typedef std::map< PackableDrawableID, DrawablePtr > DrawablesMap;

typedef std::function<void (void)> NotificationCallback;

const unsigned int N_PIVOT_POINT_MODES = 3;

// Available pivot point modes (strings for GUI output).
const char pivotPointModeStrings[N_PIVOT_POINT_MODES][32] =
{
     "Median Point",
     "Individual Centroid",
     "World origin"
};


class DrawablesSelection : public Changeable
{
    private:
        // Drawables in the selection.
        DrawablesMap drawables_;

        // Selection border color.
        glm::vec4 borderColor_;

        // Selection's centroid.
        glm::vec4 centroid_;

        // Selection's pivot point mode.
        PivotPointMode pivotPointMode_;

        // Mutex for concurrent access to this selection.
        mutable std::recursive_mutex mutex_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        DrawablesSelection( glm::vec4 borderColor );
        DrawablesSelection( const DrawablesSelection& );
        DrawablesSelection( DrawablesSelection&& );

        ~DrawablesSelection() = default;


        /***
         * 2. Getters
         ***/
        glm::vec4 getCentroid() const ;
        PivotPointMode getPivotPointMode() const ;
        std::string getName() const;
        std::string getTypeName() const;
        unsigned int getSize() const;


        /*!
         * \brief Returns true if the selection contains one or more drawables
         * of the type given as an argument.
         */
        bool contains( DrawableType drawableType ) const ;


        /*!
         * \brief Returns true if the selection contains one or more meshes
         * of the type given as an argument.
         */
        bool contains( MeshType meshType ) const ;

        bool existsDrawable( const PackableDrawableID& id ) const;


        /***
         * 3. Setters
         ***/
        virtual void setPivotPointMode( PivotPointMode pivotPointMode );


        /***
         * 4. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotate( GLfloat angle, glm::vec3 axis );
        virtual void scale( glm::vec3 scaleFactors );


        /***
         * 5. Updating
         ***/
    protected:

        /*!
         * \brief This method is automatically called when setChanged is
         * called so inherited objects can perfom actions whenever they
         * change.
         */
        virtual void onChange();

    private:
        void updateSelectionCentroid();



        /***
         * 6. Drawables management
         ***/
    public:
        void addDrawable( PackableDrawableID drawableID, DrawablePtr drawable );
        bool moveDrawable( PackableDrawableID drawableID, DrawablesSelection& destinySelection );
        void moveAll( DrawablesSelection& destinySelection );
    private:
        void clear();
    public:
        void erase();


        /***
         * 7. Ray picking
         ***/
        bool intersect( glm::vec3 r0, glm::vec3 r1, PackableDrawableID& closestDrawable, float& minT ) const ;


        /***
         * 8. Drawing
         ***/
        void draw( const glm::mat4& viewProjMatrix ) const ;


        /***
         * 9. Operators
         ***/
        DrawablesSelection& operator=( const DrawablesSelection& ) = delete ;
        DrawablesSelection& operator=( DrawablesSelection&& ) = delete;
};

typedef shared_ptr< DrawablesSelection > DrawablesSelectionPtr;
typedef shared_ptr< const DrawablesSelection > DrawablesSelectionConstPtr;

} // namespace como

#endif // DRAWABLES_SELECTION_HPP
