# Variables
CXX := cl.exe
LD := link.exe

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CXXFLAGS 	:= /Zi /MTd /Zc:__cplusplus /std:c++20 /EHsc /nologo /Iinclude
	LDFLAGS 	:= /libpath:lib /incremental:no /debug
	LIBS 		:= gdi32.lib user32.lib kernel32.lib glfw3dll.lib bimg_decodeDebug.lib bimgDebug.lib bxDebug.lib bgfxDebug.lib
	BUILDDIR 	:= build-debug
else
	CXXFLAGS 	:= /Zi /Zc:__cplusplus /std:c++20 /EHsc /nologo /Iinclude
	LDFLAGS		:= /libpath:lib /incremental:no
	LIBS 		:= gdi32.lib user32.lib kernel32.lib glfw3dll.lib bimg.lib bimg_decode.lib bx.lib bgfx.lib
	BUILDDIR 	:= build-release
endif

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, $(BUILDDIR)/%.obj, $(SRCS))
TARGET := LandmassGeneratorDemo.exe

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) /out:"$(BUILDDIR)/"$@ $^ $(LIBS)

# Object file rules
$(BUILDDIR)/%.obj: %.cpp
	$(CXX) $(CXXFLAGS) -c $< /Fo$@ /Fd$(BUILDDIR)/

# Clean rule
clean:
	powershell -command Remove-Item -Path $(BUILDDIR)/*.* -Exclude *.dll

# Phony targets
.PHONY: all clean
