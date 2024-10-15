QT = core
QT += core sql

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        clases/categoria.cpp \
        clases/cliente.cpp \
        clases/combustible.cpp \
        clases/estacion.cpp \
        clases/nave.cpp \
        clases/red.cpp \
        clases/region.cpp \
        clases/surtidor.cpp \
        clases/tanque.cpp \
        clases/venta.cpp \
        conexion/conexion.cpp \
        controladores/gestionEstaciones.cpp \
        controladores/gestionRed.cpp \
        controladores/simulacionVenta.cpp \
        controladores/verificacionFugas.cpp \
        main.cpp \
        validaciones/entradas.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    clases/categoria.h \
    clases/cliente.h \
    clases/combustible.h \
    clases/estacion.h \
    clases/nave.h \
    clases/red.h \
    clases/region.h \
    clases/surtidor.h \
    clases/tanque.h \
    clases/venta.h \
    conexion/conexion.h \
    controladores/gestionEstaciones.h \
    controladores/gestionRed.h \
    controladores/simulacionVenta.h \
    controladores/verificacionFugas.h \
    validaciones/entradas.h
