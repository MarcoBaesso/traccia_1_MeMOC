DEFINES += CPX_INCDIR
DEFINES += CPX_LIBDIR
DEFINES += CPX_LDFLAGS

CPX_INCDIR  = /opt/ibm/ILOG/CPLEX_Studio126/cplex/include
CPX_LIBDIR  = /opt/ibm/ILOG/CPLEX_Studio126/cplex/lib/x86-64_linux/static_pic
CPX_LDFLAGS = -lcplex -lm -pthread

unix:LIBS += -L$$CPX_LIBDIR $$CPX_LDFLAGS
unix:INCLUDEPATH += $$CPX_INCDIR

TARGET = tsp_pannelli_forati
OBJECTS_DIR = ../obj

DESTDIR = ../

SOURCES += \
    main.cpp \
    data.cpp \
    message_error.cpp \
    coordinata.cpp

HEADERS += \
    data.h \
    message_error.h \
    coordinata.h \
    cpxmacro.h \
    modello_tsp.h


