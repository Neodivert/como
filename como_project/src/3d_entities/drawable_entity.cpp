
#include "drawable_entity.hpp"

DrawableEntity::DrawableEntity()
{
    transformationMatrix = glm::mat4( 1.0f );

    for( GLuint i=0; i<4; i++ ){
        for( GLuint j=0; j<4; j++ ){
            cout << transformationMatrix[i][j] << " ";
        }
        cout << endl;
    }
}
