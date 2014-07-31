#include "viewport_projection_menu.hpp"

namespace como {

/***
 * 1. Construction
 ***/

ViewportProjectionMenu::ViewportProjectionMenu( Viewport* viewport ) :
    QMenu( "View" )
{
    unsigned int i = 0;
    QAction* currentAction = nullptr;

    for( i = 0; i < N_PROJECTIONS; i++ ){
        currentAction = new QAction( QString( projectionModeStrings[i] ), this );
        addAction( currentAction );

        QObject::connect( currentAction, &QAction::triggered, [=](){
            setProjection( static_cast< Projection >( i ) );
            viewport->setProjection( static_cast< Projection >( i ) );
        });
    }

    setProjection( viewport->getProjection() );
    setVisible( true );
}


/***
 * 3. Setters
 ***/

void ViewportProjectionMenu::setProjection( Projection projection )
{
    setTitle( QString( "Proj: " ) + actions().at( static_cast< int >( projection ) )->text() );
}

} // namespace como
