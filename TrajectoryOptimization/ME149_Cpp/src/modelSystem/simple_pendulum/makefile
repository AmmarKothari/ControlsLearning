CXX = clang++
CXX_FLAGS = -std=c++17 -stdlib=libc++

ROOT_FOLDER = /Users/ammarkothari/Projects/ControlsLearning/TrajectoryOptimization/ME149_Cpp
LOCALDIR = modelSystem/simple_pendulum
SOURCEDIR = $(ROOT_FOLDER)/src/$(LOCALDIR)
BUILDDIR = $(ROOT_FOLDER)/build/$(LOCALDIR)
SIMULATIONDIR = $(ROOT_FOLDER)/build/simulation

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
OBJ:=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

.PHONY: directory

all: directory $(OBJ) simple_pendulum_test.out

directory: ${BUILDDIR}

${BUILDDIR}:
	mkdir -p $(BUILDDIR)

$(OBJ): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

simple_pendulum_test.out: simple_pendulum_test.o simple_pendulum.o
	$(CXX) $(CXX_FLAGS) $< -o $@ $(BUILDDIR)/../dynamics_model.o $(SIMULATIONDIR)/euler_method_sim.o

clean:
	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/*.out