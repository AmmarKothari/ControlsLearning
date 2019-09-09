CXX = g++ -v
CXXFLAGS = -std=c++0x -Wall
PYTHON_INCLUDE = /usr/local/Cellar/python@2/2.7.16/Frameworks/Python.framework/Versions/2.7/include/python2.7
BOOST_INC = /usr/local/include
BOOST_NUMPY_FLAG = boost_numpy27
BOOSTFLAGS = -I/usr/local/include/boost -I/usr/local/include/boost/python/numpy -I$(BOOST_INC) -lboost_python27-mt -lpython2.7 -l$(BOOST_NUMPY_FLAG) -ldl -framework CoreFoundation -I$(PYTHON_INCLUDE)
BOOST_LIB = /usr/lib
PYTHON_VERSION = 2.7

# default PTRHON_VERSION is 3.6 on Mac, 3.5 in other os
# if you installed other python version, Please specific your Python Version below
PYTHON_VERSION = None

CFLAGS = -lm -pthread -O3 -std=c++11 -march=native -Wall -funroll-loops -Wno-unused-result
osname := $(shell uname)

# Only need to change if you install boost-python from source
BOOST_INC = /usr/local/include/boost
BOOST_LIB = /usr/local/lib

# $(info $$osname is [${osname}])

# if PYTHON_VERSION not set, set default PYTHON_VERSION
ifeq ($(osname), Darwin)
	EXPORT_DYNAMIC_NAME = export_dynamic
	ifeq ($(PYTHON_VERSION), None)
		PYTHON_VERSION = 2.7
	endif
else
	EXPORT_DYNAMIC_NAME = -export-dynamic
	ifeq ($(PYTHON_VERSION), None)
		PYTHON_VERSION = 3.5
	endif
endif	

$(eval REMAINDER := $$$(PYTHON_VERSION))
FIRST := $(subst $(REMAINDER),,$(PYTHON_VERSION))

# set default PYTHON_INCLUDE and LIBPYTHON_PATH for different os
# PYTHON_INCLUDE should be the path contain pyconfig.h
# LIBPYTHON_PATH should be the path contain libpython3.6 or libpython3.5 or libpython2.7 or whichever your python version
ifeq ($(osname), Darwin)
	ifeq ($(FIRST), 3)
		PYTHON_INCLUDE = /usr/local/Cellar/python3/3.6.3/Frameworks/Python.framework/Versions/3.6/include/python3.6m/
		LIBPYTHON_PATH = /usr/local/Cellar/python3/3.6.3/Frameworks/Python.framework/Versions/3.6/lib/
	else
		PYTHON_INCLUDE = /usr/local/Cellar/python@2/2.7.16/Frameworks/Python.framework/Versions/2.7/include/python2.7/
		LIBPYTHON_PATH = /usr/local/Cellar/python@2/2.7.16/Frameworks/Python.framework/Versions/2.7/lib/
	endif
else
	PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
	LIBPYTHON_PATH = /usr/lib/python$(PYTHON_VERSION)/config
endif

# boost python lib, on mac, default is lboost_python for python2.x, lboost_python3 for python3.x
# on ubuntu, name ===> python3.5m
# on mac, name ===> python3.6
ifeq ($(FIRST), 3)
	BOOST = lboost_python3
	ifeq ($(osname), Darwin)
		PYTHON_VERSION_FINAL = $(PYTHON_VERSION)
	else
		PYTHON_VERSION_FINAL = $(PYTHON_VERSION)m
	endif
else
	BOOST = lboost_python27-mt
	PYTHON_VERSION_FINAL = $(PYTHON_VERSION)
endif

TARGET_WRAP = quaternion_wrap
CPP_OBJECT_TARGET = quaternion

$(TARGET_WRAP).o: $(TARGET_WRAP).cpp
	g++ $(CFLAGS) -I$(PYTHON_INCLUDE) -I /usr/local/Cellar/python@2/2.7.16/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I$(BOOST_INC) -fPIC -c $(TARGET_WRAP).cpp

$(TARGET_WRAP).so: $(TARGET_WRAP).o
	g++ $(CFLAGS) -shared -Wl,-$(EXPORT_DYNAMIC_NAME) $(TARGET_WRAP).o $(CPP_OBJECT_TARGET).o -L$(BOOST_LIB) -$(BOOST) -L$(LIBPYTHON_PATH) -lpython$(PYTHON_VERSION_FINAL) -l$(BOOST_NUMPY_FLAG) -o $(TARGET_WRAP).so

$(CPP_OBJECT_TARGET).o: $(CPP_OBJECT_TARGET).cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -c $(CPP_OBJECT_TARGET).cpp

quaternion_basic_test.o: quaternion_basic_test.cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -c quaternion_basic_test.cpp

quaternion_basic_test: quaternion_basic_test.o quaternion.o
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -o quaternion_basic_test quaternion_basic_test.o quaternion.o

# quaternion_wrap.o: quaternion_wrap.cpp
# 	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c quaternion_wrap.cpp

# quaternion_ammar_ext: quaternion_wrap.o
# 	g++ -v -shared -Wl quaternion_wrap.o -L$(BOOST_LIB) -lboost_python27-mt -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -o $(TARGET).so

all: quaternion_basic_test $(TARGET_WRAP).so

clean:
	rm *.so *.o
