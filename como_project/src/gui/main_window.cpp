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

#include "main_window.hpp"
#include "ui_main_window.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Load OpenGL context.
    oglContext = shared_ptr<OpenGLContext>( new OpenGLContext );

    // Load empty scene.
    scene = shared_ptr<Scene>( new Scene );
    scene->addCube( new Cube );
    scene->selectAll();


    RenderPanel* renderPanel = new RenderPanel( oglContext, scene );


    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( renderPanel );
    setCentralWidget(new QWidget);
    centralWidget()->setLayout( layout );


    //renderPanel->setWindowFlags(Qt::Widget);

    //oglContext.LoadShaders();

    //setCentralWidget( renderPanel );

    /*
    layout->addWidget( renderPanel );

    setCentralWidget(new QWidget);
    centralWidget()->setLayout( layout );
    */
    /*
    renderPanel = new RenderPanel;
    ui->statusBar->addWidget( renderPanel );
*/

    /*
    scene = new QGraphicsScene;

    //populateScene();

    h1Splitter = new QSplitter;
    h2Splitter = new QSplitter;

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);
    vSplitter->addWidget(h2Splitter);

    ViewFrame *viewFrame = new ViewFrame("Top left view");
    viewFrame->getView()->setScene(scene);
    h1Splitter->addWidget( viewFrame );

    viewFrame = new ViewFrame("Top right view");
    viewFrame->getView()->setScene(scene);
    h1Splitter->addWidget(viewFrame);

    viewFrame = new ViewFrame("Bottom left view");
    viewFrame->getView()->setScene(scene);
    h2Splitter->addWidget( viewFrame );

    viewFrame = new ViewFrame("Bottom right view");
    viewFrame->getView()->setScene(scene);
    h2Splitter->addWidget( viewFrame );

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(vSplitter);
    setLayout(layout);
    */
    //setWindowTitle(tr("Chip Demo"));
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete scene;
}
