.DEFAULT: all
.PHONY: all clean

CFLAGS :=	--std=c++14 -O0 -ggdb3 -fdiagnostics-color=always -Wall

SOURCE_FILES := $(wildcard *.cpp)
HEADER_FILES := $(wildcard *.h)
OBJECT_FILES := $(SOURCE_FILES:.cpp=.o)

%.o: %.cpp $(HEADER_FILES)
	g++ $(CFLAGS) -o $@ -c $<

final: $(OBJECT_FILES)
	g++ $(CFLAGS) -o $@ $^

all: final

clean:
	rm -f *.o final


