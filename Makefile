#############################################################################
# Automatic configuration
#
# Setup by the ./configure script. If you want to use the manual
# configuration edit the Makefile.usr file and NOT this file.
#

prefix=/usr/local
datadir=${prefix}/share
sysconfdir=${prefix}/etc
exec_prefix=${prefix}
bindir=${exec_prefix}/bin
mandir=${prefix}/share/man
docdir=${prefix}/share/doc
CONF_EMU=mame
CONF_SYSTEM=unix
CONF_HOST=linux
CONF_BUILD=linux
CONF_LIB_DIRECT=yes
CONF_LIB_ZLIB=yes
CONF_LIB_EXPAT=yes
CONF_LIB_SVGALIB=no
CONF_LIB_FB=yes
CONF_LIB_ALSA=yes
CONF_LIB_OSS=yes
CONF_LIB_SDL=yes
CONF_LIB_RPI=yes
CONF_LIB_FREETYPE=yes
CONF_LIB_SVGAWIN=no
CONF_LIB_PTHREAD=yes
CONF_LIB_SLANG=no
CONF_LIB_NCURSES=yes
CONF_LIB_KRAW=yes
CONF_LIB_JRAW=yes
CONF_LIB_MRAW=yes
CONF_LIB_KEVENT=yes
CONF_LIB_JEVENT=yes
CONF_LIB_MEVENT=yes
CONF_LIB_MRAWINPUT=no
CONF_LIB_MCPN=no
CONF_CFLAGS_ARCH= -DUSE_LSB
CONF_CFLAGS_OPT= -march=native -O2 -fomit-frame-pointer -fno-merge-constants -Wall -Wno-sign-compare -Wno-unused
CONF_LDFLAGS=-s
CONF_DEBUGGER=no
CONF_DEBUG=no
CONF_DEFS=-DHAVE_CONFIG_H

# Checks
ifeq (CONF_EMU,$(findstring CONF_EMU,$(CONF_EMU)))
$(error For manual configuration you must use Makefile.usr)
endif

#############################################################################
# Extra configuration common for ./configure and manual

# Enable the creation of the map files
ifndef CONF_MAP
CONF_MAP=no
endif

# Name of the architecture. Used in the distribution file names.
ifndef CONF_ARCH
ifeq ($(CONF_DEBUG),yes)
CONF_ARCH=debug
else
CONF_ARCH=blend
endif
endif

# Pack also the emulator source in the dist package
ifndef CONF_DIFFSRC
CONF_DIFFSRC=no
endif

############################################################################
# Tools configuration for ./configure

srcdir=.
# Don't add the prefix @. This command must be used also in a shell script
INSTALL=/usr/bin/install -c
CC=@gcc
CXX=@g++
LD=@gcc
LDXX=@g++
AR=@ar
ASM=@
RC=@
LN_S=@ln -s
MD=-@mkdir -p
RM=@rm -f
ECHO=@echo
CC_FOR_BUILD=@gcc
LD_FOR_BUILD=@gcc
CXX_FOR_BUILD=@g++
LDXX_FOR_BUILD=@g++
EXE=
EXE_FOR_BUILD=
SDLCFLAGS=-I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
SDLLIBS=-L/usr/lib -lSDL -lpthread
FREETYPECFLAGS=-I/usr/include/freetype2
FREETYPELIBS=-lfreetype -lz -lbz2
ASMFLAGS=-f elf
CFLAGS_FOR_BUILD=-O0 -DUSE_COMPILER_GNUC -DUSE_OBJ_ELF -DUSE_OS_UNIX
INSTALL_PROGRAM_DIR = $(INSTALL) -d -m 755
INSTALL_MAN_DIR = $(INSTALL) -d -m 755
INSTALL_DATA_DIR = $(INSTALL) -d -m 755
INSTALL_PROGRAM = $(INSTALL) -c -m 755
INSTALL_MAN = $(INSTALL) -c -m 644
INSTALL_DATA = $(INSTALL) -c -m 644

#############################################################################
# Root makefile

include $(srcdir)/root.mak


