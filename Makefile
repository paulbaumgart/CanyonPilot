LIBS = -framework GLUT -framework OpenGL -framework Carbon
HEADERS = Support/*.h Support/Scene/*.h *.h
SOURCES = Support/*.cpp Support/Scene/*.cpp

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LIBS = -lglut -lGLU -lGL -lXmu -lXi
endif
ifeq ($(UNAME), Darwin)
	LIBS = -framework GLUT -framework OpenGL -framework Carbon
endif

default: canyonpilot

tester: rasterizer.cpp $(SOURCES) $(HEADERS)
	g++ -O3 -o tester rasterizer.cpp $(SOURCES) $(LIBS)

airplane:
canyonpilot: canyonpilot.cpp $(SOURCES) $(HEADERS)
	g++ -O3 -o canyonpilot canyonpilot.cpp $(SOURCES) $(LIBS)
