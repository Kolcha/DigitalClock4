TS_IN_NOEXT = $$replace(TRANSLATIONS,".ts","")

isEmpty(QMAKE_LRELEASE) {
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
    unix {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease-qt4 }
    } else {
        !exists($$QMAKE_LRELEASE) { QMAKE_LRELEASE = lrelease }
    }
}

#message("Building translations")
for(L,TS_IN_NOEXT) {
#    message("Processing $${_PRO_FILE_PWD_}/$${L}")
    system("$$QMAKE_LRELEASE -silent $${_PRO_FILE_PWD_}/$${L}.ts -qm $${_PRO_FILE_PWD_}/$${L}.qm")
#    !exists("$${_PRO_FILE_PWD_}/$${L}.qm"):error("Building translations failed, cannot continue")
}
