testapp: src/sandbox.cpp src/testapp.cpp
	g++ --std=c++11 src/sandbox.cpp src/testapp.cpp -I./include -L/usr/lib64 -lGLEW -lglfw3 -lGL -lX11 -lXxf86vm -lpthread -ldl -lXrandr -lXinerama -lXcursor
