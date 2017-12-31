/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "context_menu.h"

#include <functional>

#include <QMenu>
#include <QIcon>

namespace digital_clock {
namespace gui {

ContextMenu::ContextMenu(QObject* parent) : QObject(parent)
{
  menu_ = new QMenu();
  visibility_action_ = menu_->addAction(tr("&Visible"), this, &ContextMenu::VisibilityChanged);
  visibility_action_->setCheckable(true);
  visibility_action_->setChecked(true);
  menu_->addAction(QIcon(":/clock/images/settings.svg.p"), tr("&Settings"),
                   this, &ContextMenu::ShowSettingsDlg);

  QMenu* p_menu = menu_->addMenu(tr("Position"));
  QMenu* t_menu = p_menu->addMenu(tr("Top"));
  t_menu->addAction(tr("Left"), [this] () { emit PositionChanged(Qt::AlignTop | Qt::AlignLeft); });
  t_menu->addAction(tr("Middle"), [this] () { emit PositionChanged(Qt::AlignTop | Qt::AlignHCenter); });
  t_menu->addAction(tr("Right"), [this] () { emit PositionChanged(Qt::AlignTop | Qt::AlignRight); });
  QMenu* c_menu = p_menu->addMenu(tr("Middle"));
  c_menu->addAction(tr("Left"), [this] () { emit PositionChanged(Qt::AlignVCenter | Qt::AlignLeft); });
  c_menu->addAction(tr("Middle"), [this] () { emit PositionChanged(Qt::AlignVCenter | Qt::AlignHCenter); });
  c_menu->addAction(tr("Right"), [this] () { emit PositionChanged(Qt::AlignVCenter | Qt::AlignRight); });
  QMenu* b_menu = p_menu->addMenu(tr("Bottom"));
  b_menu->addAction(tr("Left"), [this] () { emit PositionChanged(Qt::AlignBottom | Qt::AlignLeft); });
  b_menu->addAction(tr("Middle"), [this] () { emit PositionChanged(Qt::AlignBottom | Qt::AlignHCenter); });
  b_menu->addAction(tr("Right"), [this] () { emit PositionChanged(Qt::AlignBottom | Qt::AlignRight); });
  menu_->addSeparator();

  menu_->addAction(QIcon(":/clock/images/info.svg.p"), tr("&About"),
                   this, &ContextMenu::ShowAboutDlg);
  menu_->addSeparator();
  menu_->addAction(QIcon(":/clock/images/update.svg.p"), tr("&Update"),
                   this, &ContextMenu::CheckForUpdates);
  menu_->addSeparator();
  menu_->addAction(QIcon(":/clock/images/quit.svg.p"), tr("&Quit"),
                   this, &ContextMenu::AppExit);
}

ContextMenu::~ContextMenu()
{
  delete menu_;
}

} // namespace gui
} // namespace digital_clock
