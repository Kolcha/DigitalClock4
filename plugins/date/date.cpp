#include <QLabel>
#include <QGridLayout>
#include <QFontMetricsF>
#include <QDate>
#include "skin_drawer.h"
#include "text_skin.h"
#include "plugin_settings.h"
#include "date_settings.h"
#include "gui/settings_dialog.h"
#include "date.h"

namespace date {

Date::Date() : avail_width_(0), last_zoom_(1.0), last_date_("-") {
  drawer_ = new skin_draw::SkinDrawer(this);
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          this, SLOT(SettingsListener(QString,QVariant)));

  InitTranslator(QLatin1String(":/date/date_"));
  info_.display_name = tr("Date");
  info_.description = tr("Allows to display current date under clock.");
  info_.icon.load(":/date/icon.png");
}

void Date::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  main_wnd_ = main_wnd;
  drawer_->SetSpace(0);

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->TrackChanges(true);
  settings_->Load();
}

void Date::Start() {
  msg_label_ = new QLabel();
  msg_label_->setAlignment(Qt::AlignCenter);
  main_layout_->addWidget(msg_label_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
  connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
    msg_label_->setPixmap(QPixmap::fromImage(img));
    main_wnd_->adjustSize();
  });
}

void Date::Stop() {
  main_layout_->removeWidget(msg_label_);
  main_wnd_->adjustSize();
  disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  delete msg_label_;
}

void Date::Configure() {
  SettingsDialog* dialog = new SettingsDialog();
  // load current settings to dialog
  connect(settings_, SIGNAL(OptionChanged(QString,QVariant)),
          dialog, SLOT(SettingsListener(QString,QVariant)));
  settings_->Load();
  // connect main signals/slots
  connect(dialog, SIGNAL(OptionChanged(QString,QVariant)),
          settings_, SLOT(SetOption(QString,QVariant)));
  connect(dialog, SIGNAL(accepted()), settings_, SLOT(Save()));
  connect(dialog, SIGNAL(rejected()), settings_, SLOT(Load()));
  dialog->show();
}

void Date::SettingsListener(Options option, const QVariant& new_value) {
  switch (option) {
    case OPT_SKIN_NAME:
      if (!msg_label_) break;  // init, not started yet
      switch ((ZoomMode)settings_->GetOption(OPT_ZOOM_MODE).toInt()) {
        case ZoomMode::ZM_NOT_ZOOM:
          msg_label_->setAlignment(Qt::AlignCenter);
          break;

        case ZoomMode::ZM_AUTOSIZE:
          Stop();
          avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 20;
          last_date_ = "-";
          Start();
          msg_label_->setAlignment(Qt::AlignLeft);
          break;

        case ZoomMode::ZM_CLOCK_ZOOM:
          drawer_->SetZoom(last_zoom_);
          msg_label_->setAlignment(Qt::AlignCenter);
          break;
      }
      break;

    case OPT_FONT:
      clock_font_ = new_value.value<QFont>();
      if (!settings_->GetOption(OPT_USE_CLOCK_FONT).toBool()) break;
      font_ = new_value.value<QFont>();
      drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
      break;

    case OPT_ZOOM:
      last_zoom_ = new_value.toReal();
      if (avail_width_ == 0) {  // first init
        avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 20;
      }
      if (last_date_ == "-") break;
      switch ((ZoomMode)settings_->GetOption(OPT_ZOOM_MODE).toInt()) {
        case ZoomMode::ZM_NOT_ZOOM:
          msg_label_->setAlignment(Qt::AlignCenter);
          break;

        case ZoomMode::ZM_AUTOSIZE:
        {
          QFontMetricsF fmf(font_);
          qreal tw = fmf.width(last_date_);
          drawer_->SetZoom(avail_width_ * last_zoom_ / tw);
          msg_label_->setAlignment(Qt::AlignLeft);
          break;
        }

        case ZoomMode::ZM_CLOCK_ZOOM:
          drawer_->SetZoom(last_zoom_);
          msg_label_->setAlignment(Qt::AlignCenter);
          break;
      }
      break;

    case OPT_COLOR:
      drawer_->SetColor(new_value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)new_value.toInt());
      break;

    case OPT_CUSTOMIZATION:
      drawer_->SetCustomizationType((skin_draw::SkinDrawer::CustomizationType)new_value.toInt());
      break;
  }
}

void Date::TimeUpdateListener() {
  QString date;
  QDate d_date = QDate::currentDate();

  switch ((FormatType)settings_->GetOption(OPT_DATE_FORMAT_TYPE).toInt()) {
    case FormatType::FT_INT:
      date = d_date.toString((Qt::DateFormat)settings_->GetOption(OPT_DATE_FORMAT_INT).toInt());
      break;

    case FormatType::FT_STR:
      date = d_date.toString(settings_->GetOption(OPT_DATE_FORMAT_STR).toString());
      break;
  }

  if (date == last_date_ || !msg_label_) return;

  drawer_->SetString(QString());
  switch ((ZoomMode)settings_->GetOption(OPT_ZOOM_MODE).toInt()) {
    case ZoomMode::ZM_NOT_ZOOM:
      break;

    case ZoomMode::ZM_AUTOSIZE:
    {
      QFontMetricsF fmf(font_);
      drawer_->SetZoom(avail_width_ * last_zoom_ / fmf.width(date));
      break;
    }

    case ZoomMode::ZM_CLOCK_ZOOM:
      break;
  }
  drawer_->SetString(date);
  last_date_ = date;
}

void Date::SettingsListener(const QString& key, const QVariant& value) {
  if (key == OPT_USE_CLOCK_FONT) {
    SettingsListener(OPT_DATE_FONT, value.toBool() ?
                       clock_font_ : settings_->GetOption(OPT_DATE_FONT).value<QFont>());
  }
  if (key == OPT_DATE_FONT) {
    font_ = value.value<QFont>();
    drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
  }
  if (key == OPT_ZOOM_MODE) {
    switch ((ZoomMode)value.toInt()) {
      case ZoomMode::ZM_NOT_ZOOM:
        drawer_->SetZoom(1.0);
        break;

      case ZoomMode::ZM_AUTOSIZE:
        SettingsListener(OPT_SKIN_NAME, QString("Text Skin"));
        break;

      case ZoomMode::ZM_CLOCK_ZOOM:
        SettingsListener(OPT_ZOOM, last_zoom_);
        break;
    }
  }
}

} // namespace date
