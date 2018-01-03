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

namespace var_translucency {

VarTranslucency::VarTranslucency() :
  old_opacity_(1.0), cur_opacity_(1.0), op_step_(-0.01)
{
  InitTranslator(QLatin1String(":/var_translucency/var_translucency_"));
  info_.display_name = tr("Variable translucency");
  info_.description = tr("Changes clock opacity level during time.");
}

void VarTranslucency::Init(const QMap<Option, QVariant>& current_settings)
{
  old_opacity_ = current_settings[OPT_OPACITY].toReal();
  cur_opacity_ = old_opacity_;
}

void VarTranslucency::Start()
{
  emit OptionChanged(OPT_OPACITY, cur_opacity_);
}

void VarTranslucency::Stop()
{
  emit OptionChanged(OPT_OPACITY, old_opacity_);
}

void VarTranslucency::TimeUpdateListener()
{
  if (qAbs(cur_opacity_ - 1.0) < qAbs(op_step_ / 2)) op_step_ = -0.01;
  if (qAbs(cur_opacity_ - 0.1) < qAbs(op_step_ / 2)) op_step_ = +0.01;
  cur_opacity_ += op_step_;
  emit OptionChanged(OPT_OPACITY, cur_opacity_);
}

} // namespace var_translucency
