CXX = g++ -v
CXXFLAGS = -std=c++0x -Wall
BOOSTFLAGS = -I/usr/local/include/boost -I/usr/local/include -lboost_python27-mt -lpython2.7 -ldl -framework CoreFoundation -I/usr/local/Cellar/python@2/2.7.16/Frameworks/Python.framework/Versions/2.7/include/python2.7 -I/usr/local/Cellar/python@2/2.7.16/Frameworks/Python.framework/Versions/2.7/include/python2.7

quaternion.o: quaternion.cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -c quaternion.cpp

quaternion_basic_test.o: quaternion_basic_test.cpp
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -c quaternion_basic_test.cpp color.h

quaternion_basic_test: quaternion_basic_test.o quaternion.o
	$(CXX) $(CXXFLAGS) $(BOOSTFLAGS) -o quaternion_basic_test quaternion_basic_test.o quaternion.o

all: quaternion_basic_test

clean:
	rm *.so *.o



# compile mesh classes
# TARGET = hello_ext
# $(TARGET).so: $(TARGET).o
# 	g++ -shared -Wl,--export-dynamic $(TARGET).o -L$(BOOST_LIB) -lboost_python-$(PYTHON_VERSION) -L/usr/lib/python$(PYTHON_VERSION)/config -lpython$(PYTHON_VERSION) -o $(TARGET).so
# $(TARGET).o: $(TARGET).C
# 	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $(TARGET).C
