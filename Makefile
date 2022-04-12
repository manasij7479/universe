TARGET:= universe
SRC:=Main.cpp
CXX:=g++
CXXFLAGS :=-Wall -std=c++17 -g
LDFLAGS := -lpthread -lGL -lGLEW -lsfml-system -lsfml-window -lsfml-graphics
default: $(TARGET)
	 
$(TARGET): $(SRC) 
	g++ $(SRC) -I$(DIR) $(LDFLAGS) $(CXXFLAGS) -o $(TARGET)
clean:
	rm -f $(TARGET)

cleanall:
	rm -f $(TARGET) gen*
