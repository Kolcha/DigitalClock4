CONFIG += c++11

linux-g++* {
  contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L/usr/lib64/nvidia-bumblebee
    QMAKE_LFLAGS += -Wl,-rpath,/usr/lib64/nvidia-bumblebee
  } else {
    LIBS += -L/usr/lib/nvidia-bumblebee
    QMAKE_LFLAGS += -Wl,-rpath,/usr/lib/nvidia-bumblebee
  }
}

isEmpty(QMAKE_LRELEASE) {
  win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
  else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link

QMAKE_EXTRA_COMPILERS += updateqm

PRE_TARGETDEPS += compiler_updateqm_make_all
