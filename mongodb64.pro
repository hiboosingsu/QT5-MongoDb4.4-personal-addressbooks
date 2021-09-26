QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    kc_dialog.cpp \
    kc_utility.cpp \
    m_append.cpp \
    m_index.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    kc_dialog.h \
    kc_utility.h \
    m_append.h \
    m_index.h \
    mainwindow.h

FORMS += \
    kc_dialog.ui \
    m_append.ui \
    m_index.ui \
    mainwindow.ui

TRANSLATIONS += \
    mongodb64_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lmongoc-1.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lmongoc-1.0
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lmongoc-1.0

INCLUDEPATH += $$PWD/../../../../usr/local/lib
DEPENDPATH += $$PWD/../../../../usr/local/lib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -lbson-1.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -lbson-1.0
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -lbson-1.0

INCLUDEPATH += $$PWD/../../../../usr/local/lib
DEPENDPATH += $$PWD/../../../../usr/local/lib
