CXX = clang++
CXX_FLAGS = -std=c++17 -stdlib=libc++

ROOT_FOLDER = /Users/ammarkothari/Projects/ControlsLearning/TrajectoryOptimization/ME149_Cpp
LOCALDIR = simulation/eulers_method
SOURCEDIR = $(ROOT_FOLDER)/src/$(LOCALDIR)
BUILDDIR = $(ROOT_FOLDER)/build/$(LOCALDIR)

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
OBJ:=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

MAIN_EXEC_DEPENDS:= $(OBJ)

DYNAMICS_MODEL_SRC_DIR = $(ROOT_FOLDER)/src/modelSystem
DYNAMICS_MODEL_BUILD_DIR = $(ROOT_FOLDER)/build/modelSystem
MAIN_EXEC_DEPENDS += $(DYNAMICS_MODEL_BUILD_DIR)/dynamics_model.o

UTILS_SRC_DIR = $(ROOT_FOLDER)/src/simulation
UTILS_BUILD_DIR = $(ROOT_FOLDER)/build/simulation


EULER_METHOD_SIM_TEST_DEPS = $(BUILDDIR)/euler_method_sim.o \
							$(DYNAMICS_MODEL_BUILD_DIR)/dynamics_model.o \
							$(OBJ) \
							$(UTILS_BUILD_DIR)/utils.o \

.PHONY: directory all clean

all: directory $(OBJ) $(EULER_METHOD_SIM_TEST_DEPS) euler_method_sim_test.out

directory: ${BUILDDIR}

${BUILDDIR}:
	mkdir -p $(BUILDDIR)

$(DYNAMICS_MODEL_BUILD_DIR)/dynamics_model.o:
	$(CXX) $(CXX_FLAGS) -c $(DYNAMICS_MODEL_SRC_DIR)/dynamics_model.cpp -o $@

$(UTILS_BUILD_DIR)/utils.o:
	$(CXX) $(CXX_FLAGS) -c $(UTILS_SRC_DIR)/utils.cpp -o $@

$(OBJ): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

euler_method_sim_test.out: $(EULER_METHOD_SIM_TEST_DEPS)
	$(CXX) $(CXX_FLAGS) $^ -o $(BUILDDIR)/$@

clean:
	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/*.out