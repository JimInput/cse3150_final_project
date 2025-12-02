#include <pybind11/pybind11.h>
#include "graph.h"
#include "file_reader.h"
#include "file_writer.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(_core, m, py::mod_gil_not_used(), py::multiple_interpreters::per_interpreter_gil()) {
    m.doc() = R"pbdoc(
        Calculator Package with C++ FileCalculator
        ------------------------------------------

        .. currentmodule:: calculator_pkg_ex_jp

        .. autosummary::
           :toctree: _generate

           Calculator
           CPPFileCalculator
    )pbdoc";

    // Bind Graph base class
    py::class_<Graph>(m, "Graph")
        .def(py::init<>())
        .def("propagate_announcements", 
            &Graph::propagate_announcements, 
            "Propagates seeded announcements through a Graph.");

    // Bind FileReader class
    py::class_<FileReader>(m, "FileReader")
        .def_static("readGraphFromCSV", 
            &FileReader::readGraphFromCSV,
            py::arg("path"))
        .def_static("seedAnnouncements",
            &FileReader::seedAnnouncements,
            py::arg("g"),
            py::arg("path"))
        .def_static("updateROV", 
            &FileReader::updateROV,
            py::arg("g"), 
            py::arg("path"));

    py::class_<FileWriter>(m, "FileWriter")
        .def_static("writeRibs", 
            &FileWriter::writeRibs,
            py::arg("g"),
            py::arg("path"));

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
