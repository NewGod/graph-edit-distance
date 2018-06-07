IDIR = ./include


CXXFLAGS = -I $(IDIR) -Wall -std=c++11 -O3 -ltinyxml

BINDIR = ./bin
SCRIPT_DIR=./script
ODIR = ./obj
SRCDIR = ./src

DEPS = $(wildcard $(IDIR)/*.h)
SCRIPTS = $(wildcard $(SCRIPT_DIR)/*.cpp)

TARGETS = $(patsubst %.cpp, %, $(SCRIPTS))

debug: CXXFLAGS += -DDEBUG -g
debug: $(TARGETS)

$(TARGETS): $(OBJ) $(SCRIPTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~  $(BINDIR)/* $(TARGETS)
