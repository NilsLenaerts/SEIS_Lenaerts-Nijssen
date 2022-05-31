QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ByteStream.cpp \
    Compiler.cpp \
    Instruction.cpp \
    Lexer.cpp \
    Parser.cpp \
    Token.cpp \
    WASMVM.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ByteStream.h \
    Character.h \
    Compiler.h \
    Instruction.h \
    InstructionSet.h \
    Lexer.h \
    Parser.h \
    Token.h \
    WASMVM.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
