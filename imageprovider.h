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

#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QDeclarativeImageProvider>

#include "config.h"

class ImageProvider : public QObject, public QDeclarativeImageProvider
{
public:
    ImageProvider(QObject* parent = 0);
    ~ImageProvider();
    QImage	requestImage ( const QString & id, QSize * size, const QSize & requestedSize );
    QPixmap	requestPixmap ( const QString & id, QSize * size, const QSize & requestedSize );
    void constructicons (QList<qint64>& uidlist);
    //should be called just before closing.
    void releaseMap();

private:
    QMap<qint64, QPixmap*>* m_iconlist;
};

#endif // IMAGEPROVIDER_H
