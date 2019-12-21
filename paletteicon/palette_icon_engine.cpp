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

#include "palette_icon_engine.h"

#include <QPainter>
#include <QPalette>
#include <QPixmapCache>
#include <QSvgRenderer>


static QColor getIconColor(QIcon::Mode mode, QIcon::State state)
{
  Q_UNUSED(state);
  QPalette::ColorGroup color_group = QPalette::Active;
  if (mode == QIcon::Disabled) color_group = QPalette::Disabled;
  return QPalette().color(color_group, QPalette::WindowText);
}


PaletteIconEngine::PaletteIconEngine()
{
  renderer_ = new QSvgRenderer();
}

PaletteIconEngine::PaletteIconEngine(const PaletteIconEngine& other) : QIconEngine(other)
{
  src_file_ = other.src_file_;
  renderer_ = new QSvgRenderer();
  if (other.renderer_->isValid())
    renderer_->load(other.src_file_);
}

PaletteIconEngine::~PaletteIconEngine()
{
  delete renderer_;
}

void PaletteIconEngine::addFile(const QString& fileName, const QSize& size, QIcon::Mode mode, QIcon::State state)
{
  Q_UNUSED(size);
  Q_UNUSED(mode);
  Q_UNUSED(state);
  if (renderer_->load(fileName))
    src_file_ = fileName;
}

QIconEngine* PaletteIconEngine::clone() const
{
  return new PaletteIconEngine(*this);
}

void PaletteIconEngine::paint(QPainter* painter, const QRect& rect, QIcon::Mode mode, QIcon::State state)
{
  paintIcon(painter, rect, getIconColor(mode, state));
}

QPixmap PaletteIconEngine::pixmap(const QSize& size, QIcon::Mode mode, QIcon::State state)
{
  QColor color = getIconColor(mode, state);

  QString pmckey("pie_%1:%2x%3:%4-%5");
  pmckey = pmckey.arg(src_file_).arg(size.width()).arg(size.height()).arg(mode).arg(state);
  pmckey += color.name();

  QPixmap pxm(size);
  if (QPixmapCache::find(pmckey, &pxm)) return pxm;

  pxm.fill(Qt::transparent);
  {
    QPainter p(&pxm);
    paintIcon(&p, pxm.rect(), color);
  }
  QPixmapCache::insert(pmckey, pxm);
  return pxm;
}

void PaletteIconEngine::paintIcon(QPainter* painter, const QRect& rect, const QColor& color)
{
  if (!renderer_ || !renderer_->isValid()) return;

  QPixmap glyph(rect.size());
  glyph.fill(Qt::transparent);
  {
    QPainter svg_painter(&glyph);
    renderer_->render(&svg_painter);

    QPixmap texture(4, 4);
    texture.fill(color);

    svg_painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    svg_painter.drawTiledPixmap(rect, texture);
  }

  painter->drawPixmap(rect, glyph);
}
