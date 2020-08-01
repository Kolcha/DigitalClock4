/*
    custom icon engine Qt plugin
    Copyright (C) 2017-2020  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QIconEnginePlugin>

#include "palette_icon_engine.h"

class PaletteIconPlugin : public QIconEnginePlugin
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QIconEngineFactoryInterface" FILE "paletteicon.json")

public:
  QIconEngine* create(const QString& filename = QString()) Q_DECL_OVERRIDE;
};

QIconEngine* PaletteIconPlugin::create(const QString& filename)
{
  PaletteIconEngine* engine = new PaletteIconEngine();
  if (!filename.isNull()) {
    engine->addFile(filename, QSize(), QIcon::Normal, QIcon::Off);
  }
  return engine;
}

#include "plugin.moc"
