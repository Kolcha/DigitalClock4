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
