#ifndef PLUGIN_LIST_ITEM_H
#define PLUGIN_LIST_ITEM_H

#include <QWidget>

/*! @brief Plugins list item widget.
 *
 * This class provides widget used to display every plugin. Widget has checkbox
 * (to enabled/disable plugin) and button (to call plugins settings dialog).
 * Button is disabled when plugin doesn't have settings.
 */
class PluginListItem : public QWidget {
  Q_OBJECT

public:
  /*!
   * Constructor.
   * @param text - text to display (plugin name)
   * @param enabled - plugin state (true means enabled)
   * @param congurable - is plugin configurable
   * @param parent - parent widget
   */
  PluginListItem(const QString& text, bool enabled, bool congurable, QWidget* parent = 0);

signals:
  /*!
   * Signal to notify that user changed checkbox state.
   * @param new_state - new checkbox state
   */
  void StateChanged(bool new_state);
  /*!
   * This signal is emited when user click on settings button.
   */
  void ConfigureRequest();
};

#endif // PLUGIN_LIST_ITEM_H
