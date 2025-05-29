TARGET = lab3
CXX = g++
SRCS = main.cpp lab3.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) -o $@ $^

clean:
	rm -f $(TARGET)
	rm -rf output/