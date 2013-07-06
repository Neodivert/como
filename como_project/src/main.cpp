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

#include "src/gui/main_window.hpp"
#include <QApplication>
#include <src/utilities/shader_loader.hpp>
//#include "3d_entities/scene.hpp"



int main(int argc, char *argv[])
{
    //glewInit();

    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.QMainWindow::show();
    
    return app.exec();

    /*
    QApplication a(argc, argv);

   QHBoxLayout* hl = new QHBoxLayout();

   QGraphicsScene* scene = new QGraphicsScene();
   QWidget* wdg = new QWidget();
   wdg->setGeometry(0,0,150,150);
   wdg->setWindowTitle("Title1");

   QGraphicsProxyWidget* proxy = scene->addWidget(wdg);
   proxy->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
   proxy->setPos(-150,0);

   //QGraphicsPixmapItem* pix = scene->addPixmap(QPixmap("e:/test.jpg"));
   //pix->setScale(0.1);
   //pix->setPos(50,0);

   {
   QGraphicsView* view = new QGraphicsView();
   view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
   view->setScene(scene);
   hl->addWidget(view);
   }

   {
   QGraphicsView* view = new QGraphicsView();
   view->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
   view->setScene(scene);
   QTransform t;
   t.scale(0.5,0.5);
   view->setTransform(t);
   hl->addWidget(view);
   }

   QWidget* w = new QWidget();
   w->setLayout(hl);
   w->setGeometry(QRect(50, 50, 600, 400));
   w->show();
   return a.exec();
    */
}
