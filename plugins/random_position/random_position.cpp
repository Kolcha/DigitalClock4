/*
    Digital Clock: random position plugin
    Copyright (C) 2020  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "random_position.h"

#include <cstdlib>
#include <ctime>

#include <QScreen>
#include <QWidget>
#include <QWindow>

namespace random_position {

RandomPosition::RandomPosition()
  : is_active_(false)
  , interval_counter_(60)
{
  InitTranslator(QLatin1String(":/random_position/lang/random_position_"));
  info_.display_name = tr("Random position");
  info_.description = tr("Randomly changes clock position during time.");
}

void RandomPosition::Init(QWidget* main_wnd)
{
  windows_.insert(main_wnd->window(), main_wnd->window()->pos());
}

void RandomPosition::Start()
{
  std::srand(std::time(0));
  is_active_ = true;
}

void RandomPosition::Stop()
{
  is_active_ = false;
  for (auto iter = windows_.cbegin(); iter != windows_.cend(); ++iter)
    if (iter.key()->pos() != iter.value())
      iter.key()->move(iter.value());
  windows_.clear();
}

void RandomPosition::TimeUpdateListener()
{
  if (!is_active_ || interval_counter_-- > 0) return;

  interval_counter_ = std::rand() % 600 + 60;

  for (auto iter = windows_.cbegin(); iter != windows_.cend(); ++iter) {
    QWidget* window = iter.key();
    Q_ASSERT(window);
    QScreen* screen = window->windowHandle()->screen();
    Q_ASSERT(screen);
    QRect sg = screen->availableGeometry();
    QRect wg = window->frameGeometry();
    window->move(sg.x() + std::rand() % (sg.width() - wg.width()),
                 sg.y() + std::rand() % (sg.height() - wg.height()));
  }
}

} // namespace random_position
