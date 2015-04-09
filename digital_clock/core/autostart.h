#ifndef AUTOSTART_H
#define AUTOSTART_H

/*!
 * Check autostart status.
 * @return true if enabled, false if disabled
 */
bool IsAutoStartEnabled();
/*!
 * Change autostart status (enable/disable).
 * @param enable - true to enable, false to disable
 */
void SetAutoStart(bool enable);

#endif // AUTOSTART_H
