# ----------------------------------------------------------------------------
# AlMake
# ----------------------------------------------------------------------------
# Generic Allegro build system
#
# This is a group of scripts and files that helps the developer and end-user
# to configure, update, and build Allegro and AllegroGL projects, 
# on many platforms, and with different compilers.
#
# Get updates of AlMake at official website
# http://almake.sf.net/
# ----------------------------------------------------------------------------
# Project main developer : Kronoman
# Project started: 26/May/2005
# ----------------------------------------------------------------------------
# <Kronoman> In loving memory of my father
#
# <MattyMatt> almake, pure and simple, caught in a world where love survives
# ----------------------------------------------------------------------------
# Thanks to Schwarzung for the help on making the legacy first makefile system.
#
# Thanks to roaet (Justin Hammond) and Solak for the great help with the Macintosh target.
#
# Greetz to KittyCat, Sevalecan, and #allegro in general.
#
# Greetz to Harley Owners Group Argentina.
# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
# ** Configuration stuff **
# ----------------------------------------------------------------------------
# Please set this to fit your project
# Also check the compiler dependent section.

# Project name
PROJECT_NAME = BezigonDemo

# Binary to produce
BINARY = bezigondemo

# Source directory
SRCDIR = src

# Include directory
INCDIR = include

# Source code suffix (Values: .c or .cpp ; will also set the compiler to use)
SRCSUF = .cpp

# this should be /bin/sh
SHELL = /bin/sh

# ----------------------------------------------------------------------------
# ** Compiler-dependent section **
# SET THIS TO FIT YOUR PROJECT
# ----------------------------------------------------------------------------
# Here you can set if your project uses Allegro or AllegroGL,
# the compiler to use, flags, other libraries to link, etc...

# The platform is autodetected (DJGPP, Mingw, Unix(Linux, Darwin, etc), in that order)

ifdef DJDIR
	# -------------------------------------
	# Here goes DJGPP stuff
	# -------------------------------------
	PLATFORMDIR=djgpp
	TARGET=DOS/DJGPP

	# compiler to invoque (C or C++ compiler)
	ifeq "$(SRCSUF)" ".c"
		CC = gcc
	else
		CC = gxx
	endif

	# Binary file suffix
	BINSUF = .exe
	# object suffix
	OBJSUF = .o

	# ** IMPORTANT - CONFIGURE **
	# ** If you need extra link options (like more librarys, add them HERE!)
	LFLAGS = -s -lalleg

	# ** Compiler flags
	CFLAGS = -I$(INCDIR) -Wall -O3 -fomit-frame-pointer
	
	# command for deletion
	DELETE_CMD = del
else
ifdef MINGDIR
	# -------------------------------------
	# Here goes MingW32 stuff
	# -------------------------------------
	PLATFORMDIR=mingw32
	TARGET=Win32/Mingw32
	
	# compiler to invoque (C or C++ compiler)
	ifeq "$(SRCSUF)" ".c"
		CC = gcc
	else
		CC = g++
	endif

	# Binary file suffix
	BINSUF = _w32.exe
	# object suffix
	OBJSUF = .o

	# ** IMPORTANT - CONFIGURE **
	# ** If you need extra link options (like more librarys, add them HERE!)
	LFLAGS = -Wl,--subsystem,windows -s -lalleg

	# ** Compiler flags
	CFLAGS = -I$(INCDIR) -Wall -O3 -fomit-frame-pointer
	
	# command for deletion
	DELETE_CMD = del
else
	# -------------------------------------
	# Here goes GCC (Linux) stuff
	#
	# This section also works for *nix and *BSD variants.
	# Also, Darwin (Mac) stuff, because is a *nix variant.
	# -------------------------------------
	
	PLATFORMDIR=linux
	TARGET=Unix/gcc
	
	# compiler to invoque (C or C++ compiler)
	ifeq "$(SRCSUF)" ".c"
		CC = gcc
	else
		CC = g++
	endif

	# Binary file suffix
	BINSUF = _unix
	# object suffix
	OBJSUF = .o

	# ** IMPORTANT - CONFIGURE **
	# ** If you need extra link options (like more librarys, add them HERE!)
	LFLAGS = -s `allegro-config --libs`

	# ** Compiler flags
	CFLAGS = -I$(INCDIR) -Wall -O3 -fomit-frame-pointer
	
	# command for deletion
	DELETE_CMD = rm

	# check if this is Linux
	ifeq ($(shell uname -s), Linux)
		TARGET=Linux/gcc
		BINSUF=_linux
	endif

	# check if this is Darwin (Macintosh)
	# if this is Darwin, we adjust some parameters
	ifeq ($(shell uname -s), Darwin)
		TARGET=Darwin/gcc
		BINSUF=_darwin
	endif

endif
endif



# ---------------------------------
# Platform non-specific stuff
# ---------------------------------
OBJDIR = obj/$(PLATFORMDIR)
BINDIR = bin

# ----------------------------------------------------------------------------
# Build process from here
# ----------------------------------------------------------------------------

TEMP = $(wildcard $(SRCDIR)/*$(SRCSUF))

ifeq ($(strip $(TEMP)),)
$(error ERROR: No source code found at $(SRCDIR)/*$(SRCSUF) )
endif

FILES = $(TEMP)
OBJS = $(addprefix $(OBJDIR)/,$(addsuffix $(OBJSUF), $(basename $(notdir $(FILES) ) ) ) )


# ---------------------------------
# Main target
# ---------------------------------
.PHONY: all 
all: $(BINDIR)/$(BINARY)$(BINSUF) adbanner
	@echo "Finished $(PROJECT_NAME)"
	@echo "Product is at $(BINDIR)/$(BINARY)$(BINSUF)."


# ---------------------------------
# binary	
# ---------------------------------
$(BINDIR)/$(BINARY)$(BINSUF) : $(OBJS) 
	$(CC) $^ -o $@ $(LFLAGS)
	@echo "The $(BINDIR)/$(BINARY)$(BINSUF) is ready!"


# ---------------------------------
# objects
# ---------------------------------
$(OBJDIR)/%$(OBJSUF) : $(SRCDIR)/%$(SRCSUF)
	$(CC) $(CFLAGS) -c $< -o $@


# ---------------------------------
# status report
# ---------------------------------
.PHONY: status
status: adbanner
	@echo "-----------------"
	@echo "- Status Report -"
	@echo "-----------------"
	@echo "  Build : $(PROJECT_NAME)"
	@echo "  Detected platform : $(TARGET)"
	@echo "  Compiler to use : $(CC)"
	@echo "  Link Flags : $(LFLAGS)"
	@echo "  Compilation Flags: $(CFLAGS)"
	@echo "  Files : $(FILES)"
	@echo "  Obj : $(OBJS)"
	@echo "  Target : $(BINDIR)/$(BINARY)$(BINSUF)"


# ---------------------------------
# clean
# ---------------------------------
.PHONY: clean
clean:
	-$(DELETE_CMD) $(BINDIR)/$(BINARY)$(BINSUF) $(OBJS)

	
# ---------------------------------
# Strip symbols and compress with upx packer (http://upx.sf.net/)
# ---------------------------------
.PHONY: upx
upx: $(BINDIR)/$(BINARY)$(BINSUF)
	strip --strip-all $(BINDIR)/$(BINARY)$(BINSUF)
	-upx --best $(BINDIR)/$(BINARY)$(BINSUF)

	
# ---------------------------------
# Install - THIS NEEDS TO BE DONE YET... HOLD PLEASE...
# TODO -- SEE HERE --> http://www.gnu.org/prep/standards/html_node/Command-Variables.html#Command-Variables
# SEE ALSO  : http://www.gnu.org/prep/standards/html_node/Directory-Variables.html#Directory-Variables
# SEE ALSO : http://www.gnu.org/prep/standards/html_node/Standard-Targets.html#Standard-Targets
# ---------------------------------
.PHONY: install
install: $(BINDIR)/$(BINARY)$(BINSUF)
	@echo Sorry, the install feature is not done yet.


# ---------------------------------
# install compressed and stripped (saves disk space)
# ---------------------------------
.PHONY: install-strip
install-strip: upx install
	
# ---------------------------------
# uninstall - - THIS NEEDS TO BE DONE
# TODO TODO TODO TODO
# ---------------------------------
.PHONY: uninstall
uninstall: $(BINDIR)/$(BINARY)$(BINSUF)
	@echo Sorry, the uninstall feature is not done yet.

# ---------------------------------
# distribution - - THIS NEEDS TO BE POLISHED
# TODO TODO TODO TODO
# check
# http://www.gnu.org/prep/standards/html_node/Releases.html
# ---------------------------------
.PHONY: dist
dist: $(BINDIR)/$(BINARY)$(BINSUF)
	@echo Creating a binary distribution... please wait...
	mkdir $(PROJECT_NAME)_relase
	cp $(BINDIR)/* $(PROJECT_NAME)_relase
	-rm $(PROJECT_NAME)_relase/remove.me
	-rm $(PROJECT_NAME).tar.gz
	tar -czvf $(PROJECT_NAME).tar.gz $(PROJECT_NAME)_relase
	rm -rf $(PROJECT_NAME)_relase
	@echo Done. The file is at $(PROJECT_NAME).tar.gz

# ---------------------------------
# Cleans and do again (rebuild)
# ---------------------------------
.PHONY: rebuild
rebuild: clean all
	
	
# ---------------------------------
# Help -- shows available targets
# ---------------------------------
.PHONY: help
help: adbanner
	@echo "--------"
	@echo "- Help -"
	@echo "--------"
	@echo "make all            } Compile the entire program."
	@echo "make rebuild        } Cleans and builds."
	@echo "make clean          } Clean objects and binary."
	@echo "make install        } Install."
	@echo "make install-strip  } Install compressed and stripped (saves disk space)."
	@echo "make uninstall      } Uninstall."
	@echo "make upx            } Strip, and compress binary with UPX."
	@echo "make dist           } Create a distribution file for this program."
	#@echo "make check          } Perform self-tests (if any)."
	@echo "make status         } Show configuration status and detected platform."
	@echo "make help           } Show this help."


# ---------------------------------
# Little ad for my website and updates. 
# ---------------------------------
.PHONY: adbanner
adbanner:
	@echo "~ AlMake makefile system - http://almake.sf.net/ ~"
