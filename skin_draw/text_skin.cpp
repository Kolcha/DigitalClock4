/*
    Digital Clock: skin draw engine
    Copyright (C) 2013-2017  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "text_skin.h"

#include <QPainter>
#include <QFontMetrics>

namespace skin_draw {

class TextImageCache : public IImageCache
{
public:
  explicit TextImageCache(TextSkin* parent) : parent_(parent) {}

  QPixmap GetImage(const QString& str, int idx) override
  {
    req_text_ += str[idx];
    if (idx == str.length() - 1 || parent_->char_map_.contains(str[idx + 1]) || parent_->char_map_.contains(str[idx])) {
      auto iter = cached_data_.find(req_text_);
      req_text_.clear();
      if (iter != cached_data_.end()) {
        parent_->curr_text_.clear();
        return iter.value();
      }
    }
    return QPixmap();
  }

  void AddImage(const QString& str, int idx, const QPixmap& img) override
  {
    if (!img) return;
    Q_UNUSED(str);
    Q_UNUSED(idx);
    // don't keep cache forever
    if (cached_data_.size() > 100) cached_data_.clear();
    cached_data_[parent_->last_text_] = img;
    parent_->last_text_.clear();
  }

  void Clear() override { cached_data_.clear(); }

private:
  QMap<QString, QPixmap> cached_data_;
  QString req_text_;
  TextSkin* parent_;
};


TextSkin::TextSkin(const QFont& font) : font_(font)
{
  img_cache_ = ImageCachePtr(new TextImageCache(this));
}

QPixmap TextSkin::ResizeImage(const QString& str, int idx, Zoom zoom)
{
  auto iter = char_map_.find(str[idx]);
  curr_text_ += iter != char_map_.end() ? *iter : str[idx];

  if (idx == str.length() - 1 || str[idx + 1] == '\n' || char_map_.contains(str[idx + 1]) || char_map_.contains(str[idx])) {
    QFont new_font(font_);
    new_font.setPointSizeF(font_.pointSizeF() * zoom.zoom_y * GetDevicePixelRatio());
    QFontMetrics fm(new_font);
    int res_w = fm.width(curr_text_);
    // add some extra spacing for italic fonts
    if (new_font.italic()) res_w += 0.25 * fm.width(*curr_text_.rbegin());
    QPixmap result(res_w, fm.height());
    // use replaced symbol only to calculate width, but draw actual one
    if (char_map_.contains(str[idx])) curr_text_ = str[idx];

    QPainter painter(&result);
    painter.setFont(new_font);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.fillRect(result.rect(), Qt::transparent);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawText(result.rect(), Qt::AlignCenter, curr_text_);
    painter.end();

    last_text_ = curr_text_;
    curr_text_.clear();
    return result;
  }

  return QPixmap();
}

} // namespace skin_draw
