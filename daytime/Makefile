CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = daytime_client

$(TARGET): daytime_client.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) daytime_client.cpp

clean:
	rm -f $(TARGET)

.PHONY: clean
