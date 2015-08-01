#include <QLabel>
#include <QGridLayout>
#include <QInputDialog>
#include <QFontMetricsF>
#include "plugin_settings.h"
#include "skin_drawer.h"
#include "text_skin.h"
#include "gui/message_widget.h"
#include "quick_note_settings.h"
#include "quick_note.h"

namespace quick_note {

QuickNote::QuickNote() : avail_width_(0), last_zoom_(1.0) {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  drawer_ = new skin_draw::SkinDrawer(this);

  InitTranslator(QLatin1String(":/quick_note/quick_note_"));
  info_.display_name = tr("Quick note");
  info_.description = tr("Allows to display any short message under clock.");
  InitIcon(":/quick_note/icon.svg");
}

void QuickNote::Init(const QMap<Options, QVariant>& current_settings) {
  for (auto iter = current_settings.begin(); iter != current_settings.end(); ++iter) {
    switch (iter.key()) {
      case OPT_FONT:
        font_ = iter.value().value<QFont>();
        drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
        break;

      case OPT_ZOOM:
        last_zoom_ = iter.value().toReal();
        break;

      case OPT_COLOR:
        drawer_->SetColor(iter.value().value<QColor>());
        break;

      case OPT_TEXTURE:
        drawer_->SetTexture(iter.value().toString());
        break;

      case OPT_TEXTURE_TYPE:
        drawer_->SetCustomizationType(static_cast<skin_draw::SkinDrawer::CustomizationType>(iter.value().toInt()));
        break;

      case OPT_TEXTURE_PER_ELEMENT:
        drawer_->SetTexturePerElement(iter.value().toBool());
        break;

      case OPT_TEXTURE_DRAW_MODE:
        drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)iter.value().toInt());
        break;

      case OPT_CUSTOMIZATION:
      {
        Customization cust = static_cast<Customization>(iter.value().toInt());
        switch (cust) {
          case Customization::C_NONE:
          case Customization::C_COLORIZE:
            drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
            break;

          default:
            break;
        }
        break;
      }

      default:
        break;
    }
  }
  drawer_->SetSpace(0);
}

void QuickNote::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  main_wnd_ = main_wnd;
  avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 7;

  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();
}

void QuickNote::ExportSettings(QSettings::SettingsMap* settings) {
  if (!settings) return;
  *settings = settings_->GetSettingsMap();
}

void QuickNote::ImportSettings(const QSettings::SettingsMap& settings) {
  settings_->SetValues(settings);
  settings_->Save();
}

void QuickNote::Start() {
  msg_label_ = new MessageWidget();
  main_layout_->addWidget(msg_label_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
  connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
    msg_label_->setPixmap(QPixmap::fromImage(img));
    main_wnd_->adjustSize();
  });
  connect(msg_label_.data(), &MessageWidget::textChanged, this, &QuickNote::ApplyString);
  connect(msg_label_.data(), &MessageWidget::textChanged, [=] (const QString& str) {
    settings_->SetOption(OPT_QUICK_NOTE_MSG, str);
    settings_->Save();
  });
  msg_label_->setText(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
}

void QuickNote::Stop() {
  main_layout_->removeWidget(msg_label_);
  main_wnd_->adjustSize();
  disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  delete msg_label_;
}

void QuickNote::Configure() {
  QInputDialog* settings_dlg = new QInputDialog();
  settings_dlg->setAttribute(Qt::WA_DeleteOnClose);
  settings_dlg->setModal(true);

  settings_dlg->setWindowTitle(tr("Quick note settings"));
  settings_dlg->setLabelText(tr("message:"));

  settings_dlg->setInputMode(QInputDialog::TextInput);
  settings_dlg->setTextValue(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());

  connect(settings_dlg, &QInputDialog::textValueSelected, [=] (const QString& str) {
    settings_->SetOption(OPT_QUICK_NOTE_MSG, str);
    settings_->Save();
    msg_label_->setText(str);
  });
  connect(settings_dlg, &QInputDialog::rejected, [=] () {
    settings_->Load();
    msg_label_->setText(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
  });

  settings_dlg->show();
}

void QuickNote::SettingsListener(Options option, const QVariant& new_value) {
  switch (option) {
    case OPT_SKIN_NAME:
      avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 16;
      msg_label_->setText(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
      break;

    case OPT_FONT:
      font_ = new_value.value<QFont>();
      drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
      break;

    case OPT_ZOOM:
    {
      last_zoom_ = new_value.toReal();
      QFontMetricsF fmf(font_);
      qreal tw = fmf.width(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
      drawer_->SetZoom(avail_width_ * last_zoom_ / tw);
      break;
    }

    case OPT_COLOR:
      drawer_->SetColor(new_value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_TYPE:
      drawer_->SetCustomizationType(static_cast<skin_draw::SkinDrawer::CustomizationType>(new_value.toInt()));
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)new_value.toInt());
      break;

    case OPT_CUSTOMIZATION:
    {
      Customization cust = static_cast<Customization>(new_value.toInt());
      switch (cust) {
        case Customization::C_NONE:
        case Customization::C_COLORIZE:
          drawer_->SetCustomizationType(::skin_draw::SkinDrawer::CT_NONE);
          break;

        default:
          break;
      }
      break;
    }

    default:
      break;
  }
}

void QuickNote::ApplyString(const QString& str) {
  drawer_->SetString(QString());
  QFontMetricsF fmf(font_);
  drawer_->SetZoom(avail_width_ * last_zoom_ / fmf.width(str));
  drawer_->SetString(str);
}

} // namespace quick_note
