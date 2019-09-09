

#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/extract.hpp>
#include <boost/python/numpy.hpp>

#include "quaternion.h"


using namespace boost::python;
BOOST_PYTHON_MODULE(quaternion_ammar_ext)
{
    // Initialize numpy
    Py_Initialize();
    boost::python::numpy::initialize();

	class_<quaternion>("quaternion")
		.def(init<float, float, float, float>())
    	.def("test", &quaternion::test)
    	.def("get_x", &quaternion::get_x)
    	.def("get_y", &quaternion::get_y)
    	.def("get_z", &quaternion::get_z)
    	.def("get_w", &quaternion::get_w)
    	.def("get_xyzw", &quaternion::get_xyzw)
    	.def("normailze", &quaternion::normalize)
    	// .def("__repr__", &quaternion::print)
		;
};
