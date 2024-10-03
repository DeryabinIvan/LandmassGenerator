# Variables
BUILDDIR := build
CXX := cl.exe
CXXFLAGS := /Zi /Zc:__cplusplus /std:c++20 /EHsc /nologo /Iinclude
LD := link.exe
LDFLAGS := /libpath:lib /incremental:no
LIBS := gdi32.lib user32.lib kernel32.lib glfw3dll.lib bimg.lib bimg_decode.lib bx.lib bgfx.lib
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, $(BUILDDIR)/%.obj, $(SRCS))
TARGET := LandmassGeneratorDemo.exe

# Default target
all: $(TARGET)

# Build rule
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) /out:"build/"$@ $^ $(LIBS)

# Object file rules
$(BUILDDIR)/%.obj: %.cpp
	$(CXX) $(CXXFLAGS) -c $< /Fo$@ /Fdbuild/
# Clean rule
clean:
	powershell -command Remove-Item -Path $(BUILDDIR)/*.*

.PHONY: all clean
