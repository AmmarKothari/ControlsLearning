CXX = clang++
CXX_FLAGS = -std=c++17 -stdlib=libc++

ROOT_FOLDER = /Users/ammarkothari/Projects/ControlsLearning/TrajectoryOptimization/ME149_Cpp
SOURCEDIR = $(ROOT_FOLDER)/src/simulation
BUILDDIR = $(ROOT_FOLDER)/build/simulation

SOURCES:=$(wildcard $(SOURCEDIR)/*.cpp)
OBJ:=$(patsubst $(SOURCEDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

MAIN_EXEC_DEPENDS:= $(OBJ)

DYNAMICS_MODEL_SRC_DIR = $(ROOT_FOLDER)/src/modelSystem
DYNAMICS_MODEL_BUILD_DIR = $(ROOT_FOLDER)/build/modelSystem
MAIN_EXEC_DEPENDS += $(DYNAMICS_MODEL_BUILD_DIR)/dynamics_model.o


.PHONY: directory all clean

all: directory $(DYNAMICS_MODEL_BUILD_DIR)/dynamics_model.o $(OBJ) euler_method_sim_test.out

directory: ${BUILD_FOLDER}

${BUILD_FOLDER}:
	mkdir -p $(BUILD_FOLDER)

$(OBJ): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

$(DYNAMICS_MODEL_BUILD_DIR)/dynamics_model.o:
	$(CXX) $(CXX_FLAGS) -c $(DYNAMICS_MODEL_SRC_DIR)/dynamics_model.cpp -o $@

euler_method_sim_test.out: $(MAIN_EXEC_DEPENDS)
	# echo "SOURCES"
	# echo $(SOURCES)
	# echo "OBJECTS"
	# echo $(OBJ)
	$(CXX) $(CXX_FLAGS) $^ -o $(BUILDDIR)/$@

# dynamics_model.o: ../modelSystem/dynamics_model.cpp
# 	$(CXX) $(CXX_FLAGS) -c -o $(BUILDDIR)/../modelSystem/$@ $<

# utils.o: utils.cpp
# 	$(CXX) $(CXX_FLAGS) -c -o $(BUILDDIR)/$@ $<

# euler_method_sim.o: euler_method_sim.cpp utils.cpp
# 	$(CXX) $(CXX_FLAGS) -c -o $(BUILDDIR)/$@ $<

# euler_method_sim_test.o: euler_method_sim_test.cpp
# 	$(CXX) $(CXX_FLAGS) -c -o $(BUILDDIR)/$@ $<

clean:
	rm -rf $(BUILDDIR)/*.o $(BUILDDIR)/*.out