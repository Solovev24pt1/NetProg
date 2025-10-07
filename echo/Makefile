CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = echo_server

$(TARGET): echo_server.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) echo_server.cpp

clean:
	rm -f $(TARGET)

.PHONY: clean
