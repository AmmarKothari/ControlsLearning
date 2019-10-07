#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python.hpp>
#include <boost/python/list.hpp>
#include <boost/python/numpy.hpp>

#include "quaternion.h"


BOOST_PYTHON_MODULE(quaternion_wrap)
{
    using namespace boost::python;
    // Initialize numpy
    Py_Initialize();
    boost::python::numpy::initialize();

	class_<quaternion>("quaternion")
		.def(init<float, float, float, float>())
        .def(init<py::list>())
    	.def("get_x", &quaternion::get_x)
    	.def("get_y", &quaternion::get_y)
    	.def("get_z", &quaternion::get_z)
    	.def("get_w", &quaternion::get_w)
    	.def("get_xyzw", &quaternion::py_np_get_xyzw)
        .def("get_conjugate", &quaternion::get_conjugate)
        .def("set_xyzw", &quaternion::set_xyzw)

    	.def("normailze", &quaternion::normalize)
    	// .def("__repr__", &quaternion::repr())
        .def("__eq__", &quaternion::operator==)
		;
};
