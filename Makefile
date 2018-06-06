IDIR = ./include

TINYXML_DIR=../tinyxml

CXXFLAGS = -I $(IDIR) -I$(TINYXML_DIR) -Wall -std=c++11 -O3 

BINDIR = ./bin
SCRIPT_DIR=./script
ODIR = ./obj
SRCDIR = ./src

DEPS = $(wildcard $(IDIR)/*.h)
SCRIPTS = $(wildcard $(SCRIPT_DIR)/*.cpp)

TARGETS = $(patsubst %.cpp ,%, $(SCRIPTS))

debug: CXXFLAGS += -DDEBUG -g

$(TARGETS): $(OBJ) $(SCRIPTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~  $(BINDIR)/*
