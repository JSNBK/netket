// Copyright 2018 The Simons Foundation, Inc. - All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NETKET_PYSUPERVISED_HPP
#define NETKET_PYSUPERVISED_HPP

#include <mpi.h>
#include <pybind11/complex.h>
#include <pybind11/eigen.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <complex>
#include <vector>
#include "supervised.hpp"

namespace py = pybind11;

namespace netket {

void AddSupervisedModule(py::module &m) {
  auto subm = m.def_submodule("supervised");

  py::class_<Supervised>(subm, "supervised")
      .def(py::init([](MachineType &ma, AbstractOptimizer &op, int batch_size,
                       std::vector<Eigen::VectorXd> samples,
                       std::vector<Eigen::VectorXcd> targets) {
             return Supervised{ma,
                               op,
                               batch_size,
                               std::move(samples),
                               std::move(targets)};
           }),
           py::keep_alive<1, 2>(), py::keep_alive<1, 3>(), py::arg("machine"),
           py::arg("optimizer"), py::arg("batch_size"), py::arg("samples"),
           py::arg("targets"))
      .def_property_readonly("loss_log_overlap", &Supervised::GetLogOverlap)
      .def_property_readonly("loss_mse", &Supervised::GetMse)
      .def_property_readonly("loss_mse_log", &Supervised::GetMseLog)
      .def("run", &Supervised::Run, py::arg("niter_opt"),
           py::arg("loss_function") = "MSE", py::arg("output_prefix") = "output",
	   py::arg("save_params_every") = 50)
      .def("iterate", &Supervised::Iterate, py::arg("loss_function") = "MSE");
}

}  // namespace netket

#endif
