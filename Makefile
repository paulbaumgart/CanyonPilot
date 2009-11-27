LIBS = -framework GLUT -framework OpenGL -framework Carbon
HEADERS = Support/*.h Support/Scene/*.h
SOURCES = Support/*.cpp Support/Scene/*.cpp

default: tester

tester: rasterizer.cpp $(SOURCES) $(HEADERS)
	g++ -O3 -o tester rasterizer.cpp $(SOURCES) $(LIBS)

airplane: airplane.cpp airplane.h $(SOURCES) $(HEADERS)
	g++ -O3 -o airplane airplane.cpp $(SOURCES) $(LIBS)