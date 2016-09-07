#include "widget_plugin_settings.h"

#include <QFont>

namespace plugin {

QString OptionKey(const plugin::WidgetPluginOption opt, const QString& plg_name)
{
  QString fmt_str("plugins/%1/%2");

  switch (opt) {
    case OPT_USE_CLOCK_FONT: return fmt_str.arg(plg_name, "use_clock_font");
    case OPT_CUSTOM_FONT: return fmt_str.arg(plg_name, "custom_font");
    case OPT_ZOOM_MODE: return fmt_str.arg(plg_name, "zoom_mode");
  }

  Q_ASSERT(false);
  return QString();
}

void InitDefaults(QMap<WidgetPluginOption, QVariant>* defaults)
{
  Q_ASSERT(defaults);
  defaults->insert(OPT_USE_CLOCK_FONT, true);
  defaults->insert(OPT_CUSTOM_FONT, QFont());
  defaults->insert(OPT_ZOOM_MODE, static_cast<int>(ZoomMode::ZM_AUTOSIZE));
}

} // namespace plugin
