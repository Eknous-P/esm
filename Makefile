# CXX=/usr/bin/g++
# CXXFLAGS= -c -Wall

BUILDDIR = ./build
OBJDIR = ./build/obj

#shamefully copied from the examples
IMGUI_DIR = ./imgui
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat
LIBS =

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

imgui:
	$(CXX) $(CXXFLAGS) $(IMGUI_DIR)/imgui.cpp -o $(OBJDIR)/imgui.o

imgui_demo:
	$(CXX) $(CXXFLAGS) $(IMGUI_DIR)/imgui_demo.cpp -o $(OBJDIR)/imgui_demo.o

imgui_draw:
	$(CXX) $(CXXFLAGS) $(IMGUI_DIR)/imgui_draw.cpp -o $(OBJDIR)/imgui_draw.o

imgui_tables:
	$(CXX) $(CXXFLAGS) $(IMGUI_DIR)/imgui_tables.cpp -o $(OBJDIR)/imgui_tables.o

imgui_widgets:
	$(CXX) $(CXXFLAGS) $(IMGUI_DIR)/imgui_widgets.cpp -o $(OBJDIR)/imgui_widgets.o

imgui_be_glfw:
	$(CXX) $(CXXFLAGS) $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp -o $(OBJDIR)/imgui_glfw.o

imgui_be_opgl3:
	$(CXX) $(CXXFLAGS) $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp -o $(OBJDIR)/imgui_opgl3.o

buildcli: maincli esm
	$(CXX) $(OBJDIR)/esm.o $(OBJDIR)/maincli.o -o $(BUILDDIR)/esmtestcli

buildgui: esm maingui imgui imgui_demo imgui_draw imgui_tables imgui_widgets imgui_be_glfw imgui_be_opgl3
	$(CXX) $(CXXFLAGS) $(OBJDIR)/maingui.o $(OBJDIR)/esm.o $(OBJDIR)/imgui.o $(OBJDIR)/imgui_demo.o $(OBJDIR)/imgui_draw.o $(OBJDIR)/imgui_tables.o $(OBJDIR)/imgui_widgets.o $(OBJDIR)/imgui_glfw.o $(OBJDIR)/imgui.opgl3.o -o $(BUILDDIR)/esmtestgui

prep:
	mkdir build
	mkdir build/obj

esm: 
	$(CXX) -c ./esm.cpp -o $(OBJDIR)/esm.o

maincli:
	$(CXX) -c ./maincli.cpp -o $(OBJDIR)/maincli.o

maingui:
	$(CXX) $(CXXFLAGS) ./maingui.cpp -o $(OBJDIR)/maingui.o

clean:
	rm -rf $(BUILDDIR)/*.o $(OBJDIR)/*.o

runcli:
	$(BUILDDIR)/esmtestcli