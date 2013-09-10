#ifndef PLUGIN_LIST_ITEM_H
#define PLUGIN_LIST_ITEM_H

#include <QCheckBox>
#include <QToolButton>

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
   * @param parent - parent widget
   */
  explicit PluginListItem(QWidget* parent = 0);
  /*!
   * Constructor.
   * @param text - text to display (plugin name)
   * @param enabled - plugin state (true means enabled)
   * @param congurable - is plugin configurable
   * @param parent - parent widget
   */
  PluginListItem(const QString& text, bool enabled, bool congurable, QWidget* parent = 0);

  /*!
   * Get item text.
   * @return item text
   */
  QString GetText() const;
  /*!
   * Get item checked state.
   * @return item checked state
   */
  bool IsChecked() const;
  /*!
   * Ask is item have settings button.
   * @return true if settings button is enabled
   */
  bool IsConfigurable() const;

public slots:
  /*!
   * Set item text
   * @param text - text to set
   */
  void SetText(const QString& text);
  /*!
   * Set item checked state
   * @param checked - new state
   */
  void SetChecked(bool checked);
  /*!
   * Set configurable property.
   * @param configurable - true to enable
   */
  void SetConfigurable(bool configurable);

signals:
  /*!
   * Signal to notify that user changed checkbox state.
   * @param text - current item text
   * @param new_state - new checkbox state
   */
  void StateChanged(const QString& text, bool new_state);
  /*!
   * This signal is emited when user click on settings button.
   * @param text - current item text
   */
  void ConfigureRequest(const QString& text);

private slots:
  /*!
   * Slot to handle checkbox toggled signal.
   * @param checked - new checked state
   */
  void CheckBoxToggled(bool checked);
  /*!
   * Slot to handle button clicked signal.
   */
  void BtnClicked();

private:
  /*!
   * Init widget.
   */
  void Init();
  QCheckBox* chk_;
  QToolButton* btn_;
};

#endif // PLUGIN_LIST_ITEM_H
