/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2018 Deepin, Inc.
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Rekols    <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UTILS_H
#define UTILS_H

#include <QKeyEvent>
#include <QObject>
#include <QPainter>
#include <QString>
#include <QIcon>

#include "application.h"

class Utils : public QObject
{
    Q_OBJECT

public:
    static QString getQrcPath(const QString &imageName);
    static QString getQrcPath(const QString &imageName, const QString &);
    static QString getQssPath(const QString &qssName);
    static QSize getRenderSize(int fontSize, const QString &string);
    static void setFontSize(QPainter &painter, int textSize);
    static void applyQss(QWidget *widget, const QString &qssName);
    static QString getKeyshortcut(QKeyEvent *keyEvent);

    static bool fileExists(const QString &path);
    static bool fileIsWritable(const QString &path);
    static bool fileIsHome(const QString &path);
    static void passInputEvent(int wid);
    static QPixmap dropShadow(const QPixmap &source, qreal radius, const QColor &color, const QPoint &offset);
    static QImage dropShadow(const QPixmap &px, qreal radius, const QColor &color);
    static QByteArray detectEncode(const QByteArray &data, const QString &fileName = QString());
    static QByteArray getEncode(const QByteArray &data);
    static qreal easeInOut(qreal x);
    static qreal easeInQuad(qreal x);
    static qreal easeInQuint(qreal x);
    static qreal easeOutQuad(qreal x);
    static qreal easeOutQuint(qreal x);
    static QVariantMap getThemeMapFromPath(const QString &filepath);
    static bool isMimeTypeSupport(const QString &filepath);

    static QPixmap renderSVG(const QString &filePath, const QSize &size);

    static QString getConfigPath();
    static QString getSuffixList();

    static QString getInputDataSize(const qint64 &);

    static bool runApp(const QString &args);
    static QImage roundImage(const QPixmap &img_in, int radius);
    static QPixmap roundQPixmap(const QPixmap &img_in, int radius);

    static void copyText(const QString &sText);
};

#endif // UTILS_H
