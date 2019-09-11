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

#include "settings.h"
//#include "fontitemdelegate.h"

#include <DThemeManager>
#include <DSettings>
#include <DSettingsGroup>
#include <DSettingsWidgetFactory>
#include <DSettingsOption>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QApplication>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

Settings::Settings(QWidget *parent)
    : QObject(parent)
{
    m_configPath = QString("%1/%2/%3/config.conf")
        .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
        .arg(qApp->organizationName())
        .arg(qApp->applicationName());

    m_backend = new QSettingBackend(m_configPath);

    settings = DSettings::fromJsonFile(":/resources/settings.json");
    settings->setBackend(m_backend);

    auto wordWrap = settings->option("base.font.wordwrap");
    connect(wordWrap, &Dtk::Core::DSettingsOption::valueChanged, this, [=] (QVariant value) {
        emit adjustWordWrap(value.toBool());
    });

    auto fontSize = settings->option("base.font.size");
    connect(fontSize, &Dtk::Core::DSettingsOption::valueChanged, this, [=] (QVariant value) {
        emit adjustFontSize(value.toInt());
    });

    auto theme = settings->option("advance.editor.theme");
    connect(theme, &Dtk::Core::DSettingsOption::valueChanged, this, [=] (QVariant value) {
        emit themeChanged(value.toString());
    });

    auto tabSpaceNumber = settings->option("advance.editor.tabspacenumber");
    connect(tabSpaceNumber, &Dtk::Core::DSettingsOption::valueChanged, this, [=](QVariant value) {
        emit adjustTabSpaceNumber(value.toInt());
    });

    auto fontFamliy = settings->option("base.font.family");
    connect(fontFamliy, &Dtk::Core::DSettingsOption::valueChanged, this, [=] (QVariant value) {
        adjustFont(value.toString());
    });

    auto keymap = settings->option("shortcuts.keymap.keymap");
    QMap<QString, QVariant> keymapMap;
    keymapMap.insert("keys", QStringList() << "standard" << "emacs" << "customize");
    keymapMap.insert("values", QStringList() << tr("Standard") << "Emacs" << tr("Customize"));
    keymap->setData("items", keymapMap);

    connect(keymap, &Dtk::Core::DSettingsOption::valueChanged, this, [=] (QVariant value) {
        // Update all key's display value with user select keymap.
        updateAllKeysWithKeymap(value.toString());
    });

    auto MainWindowState = settings->option("advance.MainWindow.MainWindowstate");
    QMap<QString, QVariant> MainWindowStateMap;
    MainWindowStateMap.insert("keys", QStringList() << "MainWindow_normal" << "MainWindow_maximum" << "fullscreen");
    MainWindowStateMap.insert("values", QStringList() << tr("Normal") << tr("Maximum") << tr("Fullscreen"));
    MainWindowState->setData("items", MainWindowStateMap);

    connect(settings, &Dtk::Core::DSettings::valueChanged, this, [=] (const QString &key, const QVariant &value) {
        // Change keymap to customize once user change any keyshortcut.
        if (!m_userChangeKey && key.startsWith("shortcuts.") && key != "shortcuts.keymap.keymap" && !key.contains("_keymap_")) {
            m_userChangeKey = true;

            QString currentKeymap = settings->option("shortcuts.keymap.keymap")->value().toString();

            QStringList keySplitList = key.split(".");
            keySplitList[1] = QString("%1_keymap_customize").arg(keySplitList[1]);
            QString customizeKey = keySplitList.join(".");

            // Just dealWithData customize key user input, don't change keymap.
            if (currentKeymap == "customize") {
                settings->option(customizeKey)->setValue(value);
            }
            // If current kemap is not "customize".
            // Copy all customize keys from current keymap, and then dealWithData customize key just user input.
            // Then change keymap name.
            else {
                copyCustomizeKeysFromKeymap(currentKeymap);
                settings->option(customizeKey)->setValue(value);
                keymap->setValue("customize");
            }

            m_userChangeKey = false;
        }
    });
}

Settings::~Settings()
{
}

// This function is workaround, it will remove after DTK fixed SettingDialog theme bug.
void Settings::dtkThemeWorkaround(QWidget *parent, const QString &theme)
{
    parent->setStyle(QStyleFactory::create(theme));

    for (auto obj : parent->children()) {
        auto w = qobject_cast<QWidget *>(obj);
        if (!w) {
            continue;
        }

        dtkThemeWorkaround(w, theme);
    }
}

void Settings::updateAllKeysWithKeymap(QString keymap)
{
    m_userChangeKey = true;

    for (auto option : settings->group("shortcuts.MainWindow")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("%1_keymap_%2").arg(keySplitList[1]).arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    for (auto option : settings->group("shortcuts.editor")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("%1_keymap_%2").arg(keySplitList[1]).arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    m_userChangeKey = false;
}

void Settings::copyCustomizeKeysFromKeymap(QString keymap)
{
    m_userChangeKey = true;

    for (auto option : settings->group("shortcuts.MainWindow_keymap_customize")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("MainWindow_keymap_%1").arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    for (auto option : settings->group("shortcuts.editor_keymap_customize")->options()) {
        QStringList keySplitList = option->key().split(".");
        keySplitList[1] = QString("editor_keymap_%1").arg(keymap);
        option->setValue(settings->option(keySplitList.join("."))->value().toString());
    }

    m_userChangeKey = false;
}