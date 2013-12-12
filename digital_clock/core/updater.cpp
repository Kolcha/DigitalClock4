#include "updater.h"

Updater::Updater(QObject* parent)
  : QObject(parent), check_beta_(false) {
}

void Updater::CheckForUpdates() {
}

void Updater::SetCheckForBeta(bool check) {
  check_beta_ = check;
}
