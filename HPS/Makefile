
##  Makefile for Vamp plugin using GNU tools on Linux.
##
##  Edit this to adjust compiler and library settings when
##  building for Linux.
##
##  Note that the list of source files, etc, goes in Makefile.inc
##  instead so that it can be included by all platform Makefiles.


# For a debug build...

#CFLAGS		:= -Wall -Wextra -g -fPIC -lm -lfftw3

# ... or for a release build

CFLAGS		:= -fPIC -lfftw3 -Wall -Wextra -O3 -msse -msse2 -mfpmath=sse -ftree-vectorize
#CXXFLAGS	:= -lfftw3


# Location of Vamp plugin SDK relative to the project directory

VAMPSDK_DIR	:= /home/kokos/Pobrane/vamp-plugin-sdk-2.7.1


# Libraries and linker flags required by plugin: add any -l<library>
# options here

PLUGIN_LDFLAGS	:= -lfftw3 -shared -Wl,-Bsymbolic -Wl,-z,defs -Wl,--version-script=vamp-plugin.map $(VAMPSDK_DIR)/libvamp-sdk.a
# -fPIC -lfftw3


# File extension for plugin library on this platform

PLUGIN_EXT	:= .so


include Makefile.inc


