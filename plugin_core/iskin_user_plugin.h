#ifndef ISKIN_USER_PLUGIN_H
#define ISKIN_USER_PLUGIN_H

#include "iclock_plugin.h"

#include "iskin.h"
#include "plugin_core_global.h"

class PLUGIN_CORE_SHARED_EXPORT ISkinUserPlugin : public IClockPlugin {
    Q_OBJECT

public slots:
  virtual void SetSkin(skin_draw::ISkin::SkinPtr new_skin) = 0;
};

/*! Skin user plugin interface IID */
#define SKIN_USER_PLUGIN_INTERFACE_IID   "nick-korotysh.digital-clock.skin-user-plugin"
Q_DECLARE_INTERFACE(ISkinUserPlugin, SKIN_USER_PLUGIN_INTERFACE_IID)

#endif // ISKIN_USER_PLUGIN_H
