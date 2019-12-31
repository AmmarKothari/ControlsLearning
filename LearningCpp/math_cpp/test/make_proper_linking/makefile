CXX = g++
CXX_FLAGS = -std=c++17

SOURCEDIR = .
SUBSOURCEDIR = $(SOURCEDIR)/class_folder
BUILDDIR = ./build

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
SOURCES +=$(wildcard $(SUBSOURCEDIR)/*.cpp)
OBJ:=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

MAIN_EXEC_DEPENDS:= $(OBJ)

.PHONY: all clean

all: clean $(OBJ) main.out

directory: ${BUILDDIR}

${BUILDDIR}:
	mkdir -p $(BUILDDIR)

$(OBJ): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

main.out: $(MAIN_EXEC_DEPENDS)
	echo $(SOURCES)
	echo $(OBJ)
	$(CXX) $(CXX_FLAGS) $^ -o $(BUILDDIR)/$@


clean:
	rm -rf *.o *.out build/*.o build/*.out

