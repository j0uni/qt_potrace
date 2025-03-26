QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# Ensure consistent architecture builds
# Use only x86_64 for now to match existing object files
# Specify C++ standard library explicitly for macOS
macx {
    QMAKE_CXXFLAGS += -stdlib=libc++
    QMAKE_LFLAGS += -stdlib=libc++
    # Add system include paths explicitly
    QMAKE_CXXFLAGS += "-isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/c++/v1"
}

INCLUDEPATH += potrace/src

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    traceview.cpp \
    imageprocessor.cpp \
    potrace/src/curve.c \
    potrace/src/decompose.c \
    potrace/src/potracelib.c \
    potrace/src/trace.c \
    potrace/src/render.c \
    potrace/src/bbox.c \
    potrace/src/trans.c

HEADERS += \
    mainwindow.h \
    traceview.h \
    imageprocessor.h \
    potrace/src/potracelib.h \
    potrace/src/trace.h \
    potrace/src/decompose.h \
    potrace/src/curve.h \
    potrace/src/render.h \
    potrace/src/bbox.h \
    potrace/src/trans.h \
    potrace/src/auxiliary.h \
    potrace/src/lists.h \
    potrace/src/bitmap.h \
    potrace/src/bitops.h \
    potrace/src/platform.h \
    potrace/src/progress.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
