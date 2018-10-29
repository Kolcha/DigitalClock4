/*
    Digital Clock: variable translucency plugin
    Copyright (C) 2013-2018  Nick Korotysh <nick.korotysh@gmail.com>

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

#include "var_translucency.h"

#include <QtMath>

namespace var_translucency {

class OpacityChanger : public IOpacityChanger
{
public:
  void setOpacity(const qreal opacity) override
  {
    angle_ = qRadiansToDegrees(qAcos((opacity - 0.55) / 0.45) / k);
  }

  qreal opacity() const override
  {
    return 0.55 + 0.45 * qCos(k * qDegreesToRadians(angle_));
  }

  IOpacityChanger& operator ++() override
  {
    angle_ += 0.5;
    if (angle_ >= 360) angle_ -= 360;
    return *this;
  }

private:
  qreal angle_ = 0.0;
  const qreal k = 2.5;
};


VarTranslucency::VarTranslucency() :
  old_opacity_(1.0), changer_(nullptr)
{
  InitTranslator(QLatin1String(":/var_translucency/lang/var_translucency_"));
  info_.display_name = tr("Variable translucency");
  info_.description = tr("Changes clock opacity level during time.");
}

void VarTranslucency::Init(const QMap<Option, QVariant>& current_settings)
{
  old_opacity_ = current_settings[OPT_OPACITY].toReal();
}

void VarTranslucency::Start()
{
  changer_ = new OpacityChanger();
  changer_->setOpacity(old_opacity_);
}

void VarTranslucency::Stop()
{
  emit OptionChanged(OPT_OPACITY, old_opacity_);
  delete changer_;
  changer_ = nullptr;
}

void VarTranslucency::TimeUpdateListener()
{
  if (!changer_) return;
  ++(*changer_);
  emit OptionChanged(OPT_OPACITY, changer_->opacity());
}

} // namespace var_translucency
