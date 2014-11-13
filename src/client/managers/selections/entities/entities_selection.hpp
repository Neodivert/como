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

#ifndef ENTITIES_SELECTION_HPP
#define ENTITIES_SELECTION_HPP

#include <client/managers/selections/lights/lights_selection.hpp>
#include <client/managers/selections/meshes/meshes_selection.hpp>
#include <client/models/3d/abstract_entities_set.hpp>
#include <client/managers/selections/resources/resources_selection.hpp>
#include <client/managers/selections/cameras/cameras_selection.hpp>

namespace como {

class EntitiesSelection : public AbstractEntitiesSet, public Observer, public Lockable
{
    public:
        /***
         * 1. Construction
         ***/
        // TODO: Pass *selections in a vector?
        EntitiesSelection( LightsSelection* lightsSelection,
                           MeshesSelection* meshesSelection,
                           CamerasSelection* camerasSelection,
                           PivotPointMode pivotPointMode = PivotPointMode::SELECTION_CENTROID );
        EntitiesSelection() = delete;
        EntitiesSelection( const EntitiesSelection& ) = delete;
        EntitiesSelection( EntitiesSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~EntitiesSelection() = default;


        /***
         * 3. Getters
         ***/
        virtual glm::vec3 centroid() const;
        virtual PivotPointMode pivotPointMode() const;
        virtual glm::vec4 borderColor() const;
        virtual unsigned int size() const;
        virtual bool containsEntity(const ResourceID &entityID) const;
        virtual std::string name() const;
        virtual std::string typeName() const;


        /***
         * 4. Setters
         ***/
        virtual void setPivotPointMode( PivotPointMode mode );
        virtual void setBorderColor( const glm::vec4& borderColor );
        virtual void setEntityModelMatrix( const ResourceID& entityID,
                                           const glm::mat4& modelMatrix );


        /***
         * 5. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotateAroundPivot(GLfloat angle, glm::vec3 axis, glm::vec3 pivot);
        virtual void rotateAroundIndividualCentroids(GLfloat angle, glm::vec3 axis);
        virtual void scaleAroundPivot(glm::vec3 scaleFactors, glm::vec3 pivot);
        virtual void scaleAroundIndividualCentroids(glm::vec3 scaleFactors);
        virtual void applyTransformationMatrix( const glm::mat4 &transformation );
        virtual void setModelMatrix(const glm::mat4 &modelMatrix);


        /***
         * 6. Intersections
         ***/
        virtual bool intersectsRay(glm::vec3 r0, glm::vec3 r1, ResourceID &closestEntity, float &minT) const;


        /***
         * 7. Updating (observer pattern)
         ***/
        virtual void update();


        /***
         * 8. Drawing
         ***/
        virtual void drawAll(OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) const;


        /***
         * 9. Operators
         ***/
        EntitiesSelection& operator = ( const EntitiesSelection& ) = delete;
        EntitiesSelection& operator = ( EntitiesSelection&& ) = delete;


    private:
        std::vector< AbstractEntitiesSet* > specializedEntitiesSelections_;

        LightsSelection* lightsSelection_;
        MeshesSelection* meshesSelection_;
        CamerasSelection* camerasSelection_;

        PivotPointMode pivotPointMode_;
        glm::vec3 centroid_;    
};


typedef std::shared_ptr< EntitiesSelection > EntitiesSelectionPtr;

} // namespace como

#endif // ENTITIES_SELECTION_HPP
