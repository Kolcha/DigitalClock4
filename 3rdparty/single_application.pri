exists(../../3rdparty/SingleApplication/singleapplication.pri) {
  include(../../3rdparty/SingleApplication/singleapplication.pri)
  DEFINES += HAVE_SINGLEAPPLICATION
  DEFINES += QAPPLICATION_CLASS=QApplication
}
