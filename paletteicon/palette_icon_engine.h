/*
    custom icon engine Qt plugin
    Copyright (C) 2017-2019  Nick Korotysh <nick.korotysh@gmail.com>

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

#ifndef PALETTE_ICON_ENGINE_H
#define PALETTE_ICON_ENGINE_H

#include <QIconEngine>

class QSvgRenderer;

class PaletteIconEngine : public QIconEngine
{
public:
  PaletteIconEngine();
  PaletteIconEngine(const PaletteIconEngine& other);
  ~PaletteIconEngine();

  void addFile(const QString& fileName, const QSize& size, QIcon::Mode mode, QIcon::State state) Q_DECL_OVERRIDE;

  QIconEngine* clone() const Q_DECL_OVERRIDE;

  void paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state) Q_DECL_OVERRIDE;
  QPixmap pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state) Q_DECL_OVERRIDE;

  void virtual_hook(int id, void *data) Q_DECL_OVERRIDE;

private:
  PaletteIconEngine& operator=(const PaletteIconEngine& other) Q_DECL_EQ_DELETE;

  void paintIcon(QPainter* painter, const QRect& rect, const QColor& color);

  QSvgRenderer* renderer_;
  QString src_file_;
};

#endif // PALETTE_ICON_ENGINE_H
