CXX = clang++
CXX_FLAGS = -std=c++17 -stdlib=libc++

ROOT_FOLDER = /Users/ammarkothari/Projects/ControlsLearning/TrajectoryOptimization/ME149_Cpp
LOCALDIR = modelSystem/simple_pendulum
SOURCEDIR = $(ROOT_FOLDER)/src/$(LOCALDIR)
BUILDDIR = $(ROOT_FOLDER)/build/$(LOCALDIR)

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
OBJ:=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

MAIN_EXEC = $(BUILDDIR)/simple_pendulum.out

MAIN_EXEC_DEPENDS = $(BUILDDIR)/../dynamics_model.o \
  $(BUILDDIR)/../../simulation/eulers_method/euler_method_sim.o \
  $(BUILDDIR)/../../simulation/utils.o

.PHONY: directory

all: directory $(OBJ) $(MAIN_EXEC)

directory: ${BUILDDIR}

${BUILDDIR}:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/../dynamics_model.o:
	cd $(SOURCEDIR)/.. && make all

$(BUILDDIR)/../../simulation/utils.o:
	cd $(SOURCEDIR)/../../simulation && make all

$(BUILDDIR)/../../simulation/eulers_method/euler_method_sim.o:
	cd $(SOURCEDIR)/../../simulation && make all

$(OBJ): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(MAIN_EXEC): $(OBJ)
	$(CXX) $^ $(MAIN_EXEC_DEPENDS) -o $@

clean:
	rm -rf $(BUILDDIR)/*.o