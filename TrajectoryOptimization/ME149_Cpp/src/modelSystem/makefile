CXX = clang++
CXX_FLAGS = -std=c++17 -stdlib=libc++

ROOT_FOLDER = /Users/ammarkothari/Projects/ControlsLearning/TrajectoryOptimization/ME149_Cpp
SOURCEDIR = $(ROOT_FOLDER)/src/modelSystem
BUILDDIR = $(ROOT_FOLDER)/build/modelSystem

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
OBJ:=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: directory

all: directory $(OBJ)

directory: ${BUILDDIR}

${BUILDDIR}:
	mkdir -p $(BUILDDIR)

$(OBJ): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR)/*.o