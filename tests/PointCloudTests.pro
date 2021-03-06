# This specifies the exe name
TARGET=PointCloudTests
# where to put the .o files
OBJECTS_DIR=obj
# core Qt Libs to use add more here if needed.
QT+=gui opengl core
CONFIG+=c++17
# we are going to use GLFW for Offscreen OpenGL context creation so we can test GL elements
 cache()
# where to put moc auto generated files
MOC_DIR=moc
# on a mac we don't create a .app bundle file ( for ease of multiplatform use)
CONFIG-=app_bundle
# Auto include all .cpp files in the project src directory (can specifiy individually if required)
SOURCES+= $$PWD/main.cpp \
          $$PWD/PointCloudTests.cpp \
          $$PWD/BoundingBoxTests.cpp \
          $$PWD/../src/PointCloud.cpp \
          $$PWD/../src/BoundingBox.cpp

HEADERS += $$PWD/../include/PointCloud.h \
           $$PWD/../include/BoundingBox.h
OTHER_FILES+=$$PWD/files/*

# same for the .h files

DEPENDPATH+=$$PWD/../include
# and add the include dir into the search path for Qt and make
INCLUDEPATH +=../include
# where our exe is going to live (root of project)
DESTDIR=./
# add the glsl shader files
OTHER_FILES+= README.md
# were are going to default to a console app
CONFIG += console
LIBS += -L/public/devel/lib
LIBS+=-lgtest
# note each command you add needs a ; as it will be run as a single line
# first check if we are shadow building or not easiest way is to check out against current
#!equals(PWD, $${OUT_PWD}){
#	copydata.commands = echo "creating destination dirs" ;
#	# now make a dir
#	copydata.commands += mkdir -p $$OUT_PWD/shaders ;
#	copydata.commands += echo "copying files" ;
#	# then copy the files
#	copydata.commands += $(COPY_DIR) $$PWD/shaders/* $$OUT_PWD/shaders/ ;
#	# now make sure the first target is built before copy
#	first.depends = $(first) copydata
#	export(first.depends)
#	export(copydata.commands)
#	# now add it as an extra target
#	QMAKE_EXTRA_TARGETS += first copydata
#}
NGLPATH=$$(NGLDIR)
isEmpty(NGLPATH){ # note brace must be here
  message("including $HOME/NGL")
  include($(HOME)/NGL/UseNGL.pri)
}
else{ # note brace must be here
  message("Using custom NGL location")
  include($(NGLDIR)/UseNGL.pri)
}
