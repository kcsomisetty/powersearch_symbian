//Powersearch is search app for symbian smartphones
//Copyright (C) 2010 Krishna Somisetty

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Powersearch is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QtGui/QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QDesktopWidget>
#include <QtCore>
#include <QDeclarativeEngine>

#include "model.h"
#include "imageprovider.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect clientRect = desktopWidget->screenGeometry();
    QDeclarativeView* view = new QDeclarativeView();

#if QT_VERSION > 0x040701
    view->setAttribute( Qt::WA_LockPortraitOrientation, true);
#endif

    ImageProvider* ip = new ImageProvider();
    view->engine()->addImageProvider(QLatin1String("model"), ip);

    Model* m = new Model(ip);
    ip->setParent(m);
    m->setParent(view);

    view->rootContext()->setContextProperty("appModel", m);
    view->rootContext()->setContextProperty("cntModel", m->cntModel());
    view->setSource(QUrl("qrc:/ui.qml"));
    view->setSceneRect(clientRect);
    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->showFullScreen();
    QObject::connect((QObject*)view->engine(), SIGNAL(quit()), m, SLOT(quitNow()));
    QObject::connect(m, SIGNAL(canQuit()), &app, SLOT(quit()));
    view->show();

    int i = app.exec();
    delete view;

    return i;
}
