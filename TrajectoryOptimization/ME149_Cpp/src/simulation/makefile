CXX = clang++
CXX_FLAGS = -std=c++17 -stdlib=libc++

ROOT_FOLDER = /Users/ammarkothari/Projects/ControlsLearning/TrajectoryOptimization/ME149_Cpp
LOCALDIR = simulation
SOURCEDIR = $(ROOT_FOLDER)/src/$(LOCALDIR)
BUILDDIR = $(ROOT_FOLDER)/build/$(LOCALDIR)

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
OBJ:=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: directory all clean

all: directory

directory: ${BUILDDIR} $(OBJ) $(BUILDDIR)/utils_test.out

${BUILDDIR}:
	mkdir -p $(BUILDDIR)

$(OBJ): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(BUILDDIR)/utils_test.out: $(OBJ)
	$(CXX) $^ -o $@