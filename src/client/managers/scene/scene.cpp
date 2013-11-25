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

#include "scene.hpp"

namespace como {

DrawableTypes drawableTypes =
{{
    DrawableType::CUBE
}};

DrawableTypeStrings drawableTypeStrings =
{{
    QString::fromUtf8( "Cube" )
}};

PivotPointModes pivotPointModes =
{{
    PivotPointMode::MEDIAN_POINT,
    PivotPointMode::INDIVIDUAL_CENTROIDS,
    PivotPointMode::WORLD_ORIGIN
}};

// Array with a string for each pivot point mode value (for output in GUI).
PivotPointModeStrings pivotPointModeStrings =
{{
     QString::fromUtf8( "Median Point" ),
     QString::fromUtf8( "Individual Centroid" ),
     QString::fromUtf8( "World origin" )
}};

/***
 * 1. Initialization and destruction
 ***/

Scene::Scene( LogPtr log ) :
    log_( log ),
    server_( std::bind( &Scene::executeRemoteCommand, this, std::placeholders::_1 ), log_ )
{
    initLinesBuffer();

    // Set the default contour color for those drawable that are
    // not selected by any user.
    defaultContourColor[0] = 0.0f;
    defaultContourColor[1] = 0.0f;
    defaultContourColor[2] = 0.0f;
    defaultContourColor[3] = 0.0f;

    // Set the background color.
    setBackgroundColor( 0.9f, 0.9f, 0.9f, 0.9f );

    // Insert the local user as the first one in the users list.
    // addUser( 0.0f, 0.0f, 1.0f );

    // TODO: delete
    //addUser( 1.0f, 0.0f, 0.0f );
    //addUser( 0.0f, 1.0f, 0.0f );

    // TODO: Change : Get the real local user ID.
    localUserID_ = 1;

    checkOpenGL( "Scene - constructor\n" );
}


Scene::~Scene()
{
    log_->debug( "Destroying scene\n" );

    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &linesVBO );
    glDeleteVertexArrays( 1, &linesVAO );
}


void Scene::initLinesBuffer()
{
    GLint currentShaderProgram;
    GLint vPosition;

    const GLfloat x0 = -0.5f;
    const GLfloat y0 = -0.5f;
    const GLfloat z0 = 0.5f;

    const GLfloat x1 = 0.5f;
    const GLfloat y1 = 0.5f;
    const GLfloat z1 = -0.5f;

    GLfloat linesData[] =
    {
        // World X axis
        x0, y0, z0,
        x1, y0, z0,

        // World Y axis
        x0, y0, z0,
        x0, y1, z0,

        // World Z axis
        x0, y0, z0,
        x0, y0, z1,

        // X guide axis
        -100.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 0.0f,

        // Y guide axis
        0.0f, -100.0f, 0.0f,
        0.0f, 100.0f, 0.0f,

        // Z guide axis
        0.0f, 0.0f, -100.0f,
        0.0f, 0.0f, 100.0f,

        // Auxiliar line for rotations and scales
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    const int LINES_BUFFER_SIZE = 42 * sizeof( GLfloat );

    linesBufferOffsets[WORLD_AXIS] = 0;
    linesBufferOffsets[GUIDE_AXIS] = 6;
    linesBufferOffsets[TRANSFORM_GUIDE_LINE] = 12;

    // Get the position of the vertex shader variable "vPosition"
    // for the current shader program.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    vPosition = glGetAttribLocation( currentShaderProgram, "vPosition" );
    if( vPosition == GL_INVALID_OPERATION ){
        log_->error( "Error getting layout of \"position\"\n" );
    }else{
        log_->debug( "vPosition: (", vPosition, ")\n" );
    }
    // Get location of uniform shader variable "color".
    uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );
    log_->debug( "Scene: uniform color location initialized to (", uniformColorLocation, ")\n" );

    // Set a VBO for the world axis rects.
    glGenBuffers( 1, &linesVBO );
    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );
    glBufferData( GL_ARRAY_BUFFER, LINES_BUFFER_SIZE, linesData, GL_DYNAMIC_DRAW );

    // Set a VAO for the world axis rects.
    glGenVertexArrays( 1, &linesVAO );
    glBindVertexArray( linesVAO );

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );
}


/***
 * 2. Users administration
 ***/

void Scene::removeUser( UserID userID )
{
    if( users_.erase( userID ) ){
        emit userDisconnected( userID );
    }
}


/***
 * 3. Getters and setters
 ***/

void Scene::setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a )
{
    glClearColor( r, g, b, a );
}


void Scene::setTransformGuideLine( glm::vec3 origin, glm::vec3 destiny )
{
    GLfloat* guideRectsBuffer = nullptr;
    unsigned int i = 0;

    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );
    guideRectsBuffer = (GLfloat *)glMapBufferRange( GL_ARRAY_BUFFER, linesBufferOffsets[TRANSFORM_GUIDE_LINE]*3*sizeof( GLfloat ), 6*sizeof( GLfloat ), GL_MAP_WRITE_BIT );

    for( ; i<3; i++ ){
        guideRectsBuffer[i] = origin[i];
    }
    for( ; i<6; i++ ){
        guideRectsBuffer[i] = destiny[i-3];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
}


glm::vec3 Scene::getPivotPoint( const PivotPointMode& pivotPointMode )
{
    switch( pivotPointMode ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
        case PivotPointMode::MEDIAN_POINT:
            return glm::vec3( selectionCentroid );
        break;
        default:
            return glm::vec3( 0.0f, 0.0f, 0.0f );
        break;
    }
}


/***
 * 4. Drawables administration
 ***/

void Scene::addDrawable( DrawablePtr drawable )
{
    //unselectAll();

    log_->debug( "Scene - Pushing back non selected drawable 1\n" );
    checkOpenGL( "Scene::addDrawable" );
    nonSelectedDrawables.push_back( drawable );

    emit renderNeeded();
}


void Scene::addDrawable( DrawableType drawableType )
{
    DrawablePtr drawable;
    Cube* cube;

    switch( drawableType ){
        case DrawableType::CUBE:
            cube = new Cube;
            drawable = DrawablePtr( cube );
        break;
    }

    addDrawable( drawable );
}


/***
 * 5. Drawables selection
 ***/

void Scene::selectDrawable( const unsigned int& index )
{
    selectDrawable( index, localUserID_ );
}


void Scene::selectDrawable( const unsigned int& index, const unsigned int& userId )
{
    // TODO: In future versions, set a global unique id for each drawables,
    // whether they are selected or not.
    DrawablesList::iterator it = nonSelectedDrawables.begin();
    std::advance( it, index );

    log_->debug( "Selecting drawable(userId: ", userId, ")\n" );
    DrawablesList& userSelection = users_.at( userId ).selection;
    userSelection.splice( userSelection.end(), nonSelectedDrawables, it );

    updateSelectionCentroid( userId );

    emit renderNeeded();
}

/*
void Scene::selectAll()
{
    selectedDrawables.splice( selectedDrawables.end(), nonSelectedDrawables );

    updateSelectionCentroid();

    emit renderNeeded();
}
*/

void Scene::unselectAll()
{
    unselectAll( localUserID_ );
}


void Scene::unselectAll( const unsigned int& userId )
{
    DrawablesList& userSelection = users_.at( userId ).selection;

    nonSelectedDrawables.splice( nonSelectedDrawables.end(), userSelection );

    updateSelectionCentroid( userId );

    emit renderNeeded();
}


int Scene::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection )
{
    return selectDrawableByRayPicking( r0, r1, addToSelection, localUserID_ );
}


int Scene::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, const unsigned int& userId )
{
    log_->debug( "Scene::selectDrawableByRayPicking - UserID: (", userId, ")\n" );

    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    float t = MAX_T;
    unsigned int currentObject = 0;
    unsigned int closestObject = -1;

    DrawablesList& userSelection = users_.at( userId ).selection;

    r1 = glm::normalize( r1 );

    log_->debug( "Scene::selectDrawableByRayPicking\n",
                 "\tr0 : (", r0.x, ", ", r0.y, ", ", r0.z, ")\n",
                 "\tr1 : (", r1.x, ", ", r1.y, ", ", r1.z, ")\n" );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        nonSelectedDrawables.splice( nonSelectedDrawables.end(), userSelection );
    }

    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    DrawablesList::iterator it;
    for( it = nonSelectedDrawables.begin(); it != nonSelectedDrawables.end(); it++, currentObject++ ){
        (*it)->intersects( r0, r1, t );
        if( ( t >= 0.0f ) && (t < minT ) ){
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minT = t;
        }
    }

    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    for( it = userSelection.begin(); it != userSelection.end(); it++, currentObject++ ){
        (*it)->intersects( r0, r1, t );
        if( ( t > 0.0f ) && (t < minT ) ){
            // New closest object, get its index and distance.
            closestObject = currentObject;
            minT = t;
            log_->debug( "RETURN 0\n" );
            emit renderNeeded();
            return 0;
        }
    }

    // If there were intersections, select the closest one.
    if( minT < MAX_T ){
        log_->debug( "FINAL CLOSEST OBJECT: ", closestObject, "\n",
                     "\t min t: ", minT, ")\n",
                     "\t min distance: ", glm::distance( glm::vec3( 0.0f, 0.0f, 0.0f ), r1 * t ), "\n" );
        selectDrawable( closestObject );
    }else{
        log_->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
        unselectAll( userId );
    }

    emit renderNeeded();

    return closestObject;
}


glm::vec4 Scene::getSelectionCentroid() const
{
    return selectionCentroid;
}


/***
 * 6. Transformations
 ***/

void Scene::translateSelection( const glm::vec3& direction )
{
    translateSelection( direction, localUserID_ );
}


void Scene::translateSelection( const glm::vec3& direction, const unsigned int& userId )
{
    DrawablesList& userSelection = users_.at( userId ).selection;
    DrawablesList::iterator it = userSelection.begin();

    for( ; it != userSelection.end(); it++ )
    {
        (*it)->translate( direction );
    }

    updateSelectionCentroid( userId );

    emit renderNeeded();
}


void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const PivotPointMode& pivotPointMode )
{
    rotateSelection( angle, axis, pivotPointMode );
}


void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const PivotPointMode& pivotPointMode, const unsigned int& userId )
{
    DrawablesList& userSelection = users_.at( userId ).selection;
    DrawablesList::iterator it = userSelection.begin();

    switch( pivotPointMode ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( ; it != userSelection.end(); it++ ){
                (*it)->rotate( angle, axis, glm::vec3( (*it)->getCentroid() ) );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( ; it != userSelection.end(); it++ ){
                (*it)->rotate( angle, axis, glm::vec3( selectionCentroid ) );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( ; it != userSelection.end(); it++ ){
                (*it)->rotate( angle, axis );
            }
        break;
    }

    updateSelectionCentroid( userId );

    emit renderNeeded();
}


void Scene::scaleSelection( const glm::vec3& scaleFactors, const PivotPointMode& pivotPointMode )
{
    scaleSelection( scaleFactors, pivotPointMode );
}


void Scene::scaleSelection( const glm::vec3& scaleFactors, const PivotPointMode& pivotPointMode, const unsigned int& userId )
{
    DrawablesList& userSelection = users_.at( userId ).selection;
    DrawablesList::iterator it = userSelection.begin();

    switch( pivotPointMode ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
            for( ; it != userSelection.end(); it++ ){
                (*it)->scale( scaleFactors, glm::vec3( (*it)->getCentroid() ) );
            }
        break;
        case PivotPointMode::MEDIAN_POINT:
            for( ; it != userSelection.end(); it++ ){
                (*it)->scale( scaleFactors, glm::vec3( selectionCentroid ) );
            }
        break;
        case PivotPointMode::WORLD_ORIGIN:
            for( ; it != userSelection.end(); it++ ){
                (*it)->scale( scaleFactors );
            }
        break;
    }

    updateSelectionCentroid( userId );

    emit renderNeeded();
}


/*
void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot )
{
    DrawablesList::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->rotate( angle, axis, pivot );
    }
    emit renderNeeded();
}
*/

void Scene::deleteSelection()
{
    deleteSelection( localUserID_ );
}


void Scene::deleteSelection( const unsigned int& userId )
{
    DrawablesList& userSelection = users_.at( userId ).selection;

    // Delete selected drawables.
    userSelection.clear();

    emit renderNeeded();
}


void Scene::executeRemoteCommand( const SceneCommand* command )
{
    const UserConnected* userConnected = nullptr;

    log_->debug( "Scene::executeRemoteCommand(", command, ")\n" );

    switch( command->getType() ){
        case SceneCommandType::USER_CONNECTED:
            log_->debug( "\tCasting to USER_CONNECTED command\n" );
            userConnected = dynamic_cast< const UserConnected* >( command );
            log_->debug( "Adding user to scene [", userConnected->getName(), "] ...\n" );
            addUser( std::shared_ptr< const UserConnected>( new UserConnected( *userConnected ) ) );
            log_->debug( "Adding user to scene [", userConnected->getName(), "] ...OK\n" );
        break;
        default:
            log_->debug( "Removing user from scene [", users_.at( command->getUserID() ).name, "]\n" );
            removeUser( command->getUserID() );
        break;
    }
}


/***
 * 7. Updating
 ***/

void Scene::updateSelectionCentroid( const unsigned int& userId )
{
    DrawablesList& userSelection = users_.at( userId ).selection;
    DrawablesList::const_iterator it = userSelection.begin();
    //GLfloat* selectionCentroidBuffer = nullptr;
    selectionCentroid = glm::vec4( 0.0f );

    // Update scene selection centroid.
    if( userSelection.size() ){
        for( ; it != userSelection.end(); it++ ){
            selectionCentroid += (*it)->getCentroid();
        }
        selectionCentroid /= userSelection.size();
        selectionCentroid.w = 1.0f;
    }

    /*
    // Map the pivot point VBO to client memory and update the selection centroid
    // coordinates (for drawing).
    glBindBuffer( GL_ARRAY_BUFFER, selectionCentroidVBO );
    selectionCentroidBuffer = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    for( unsigned int i=0; i<3; i++ ){
        selectionCentroidBuffer[i] = selectionCentroid[i];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
    */
}


/***
 * 8. Drawing
 ***/

void Scene::draw( const int& drawGuideRect ) const
{
    GLfloat WHITE_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    DrawablesList::const_iterator it = nonSelectedDrawables.begin();
    UsersMap::const_iterator usersIterator = users_.begin();

    DrawablesList userSelection;

    checkOpenGL( "Scene::draw 1" );
    for( ; it != nonSelectedDrawables.end(); it++ )
    {
        (*it)->draw( defaultContourColor );
    }
    log_->debug( "Drawing (", nonSelectedDrawables.size(), ") non selected drawables ...OK\n" );
    checkOpenGL( "Scene::draw 2" );
    for( ; usersIterator != users_.end(); usersIterator++  ){
        userSelection = (usersIterator->second).selection;

        for( it = userSelection.begin(); it != userSelection.end(); it++ ){
            (*it)->draw( (usersIterator->second).color );
        }
    }

    // Draw a guide rect if asked.
    if( drawGuideRect != -1 ){
        // Change painting color to white.
        glUniform4fv( uniformColorLocation, 1, WHITE_COLOR );

        // Bind guide rects' VAO and VBO as the active ones.
        glBindVertexArray( linesVAO );
        glBindBuffer( GL_ARRAY_BUFFER, linesVBO );

        // Draw the guide rect.
        glDrawArrays( GL_LINES, linesBufferOffsets[GUIDE_AXIS] + (drawGuideRect << 1), 2 );
    }
}


void Scene::drawWorldAxis() const
{
    GLfloat worldAxisColors[3][4] =
    {
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f }
    };

    // Bind world axis rects' VAO and VBO as the active ones.
    glBindVertexArray( linesVAO );
    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );

    // Draw each world axis with its corresponding color.
    for( unsigned int i=0; i<3; i++ ){
        glUniform4fv( uniformColorLocation, 1, worldAxisColors[i] );
        glDrawArrays( GL_LINES, linesBufferOffsets[WORLD_AXIS] + (i << 1), 2 );
    }
}


void Scene::drawTransformGuideLine() const
{
    const GLfloat lineColor[4] =
    {
        0.0f, 1.0f, 0.0f, 1.0f
    };

    // Bind selection centroid VAO and VBO as the active ones.
    glBindVertexArray( linesVAO );
    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );

    // Set selection centroid color.
    glUniform4fv( uniformColorLocation, 1, lineColor );

    // Draw selection centroid point.
    // TODO: The range of point sizes are implementation-dependent. Also I have to
    // check wheter point size mode is enabled or not.
    glDisable( GL_DEPTH_TEST );
    glPointSize( 3.0f );
    glDrawArrays( GL_POINTS, linesBufferOffsets[TRANSFORM_GUIDE_LINE], 1 );
    glPointSize( 1.0f );
    glDrawArrays( GL_LINES, linesBufferOffsets[TRANSFORM_GUIDE_LINE], 2 );
    glEnable( GL_DEPTH_TEST );
}


} // namespace como
