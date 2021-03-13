QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TEMPLATE = app

CONFIG += STATIC

TARGET = EDF_header_modifier

SOURCES += main.cpp \
          edf_hdr.cpp \
          create_anon_file.cpp \
          drop_window.cpp

HEADERS += edf_hdr.h \
          create_anon_file.h \
          drop_window.h

FORMS = drop_window.ui

ICON += app_icon.icns

win32:RC_ICONS += app_icon.ico
