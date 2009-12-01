LIBS = -framework GLUT -framework OpenGL -framework Carbon
HEADERS = Support/*.h Support/Scene/*.h *.h
SOURCES = Support/*.cpp Support/Scene/*.cpp

default: canyonpilot

tester: rasterizer.cpp $(SOURCES) $(HEADERS)
	g++ -O3 -o tester rasterizer.cpp $(SOURCES) $(LIBS)

airplane:
canyonpilot: canyonpilot.cpp $(SOURCES) $(HEADERS)
	g++ -O3 -o canyonpilot canyonpilot.cpp $(SOURCES) $(LIBS)
