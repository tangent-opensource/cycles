/*
 * Copyright 2011-2013 Blender Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <pybind11/functional.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <graph/node_type.h>
#include <render/mesh.h>
#include <render/session.h>

namespace py = pybind11;

namespace pybind11 {
namespace detail {

template <> struct type_caster<ccl::float2> {
 public:
 PYBIND11_TYPE_CASTER(ccl::float2, _("float2"));
  bool load(handle src, bool) { return false; }

  static handle cast(const ccl::float2 &src, return_value_policy /* policy */,
                     handle /* parent */) {
    return py::make_tuple(src.x, src.y).inc_ref();
  }
};

template <> struct type_caster<ccl::float3> {
 public:
 PYBIND11_TYPE_CASTER(ccl::float3, _("float3"));
  bool load(handle src, bool) { return false; }

  static handle cast(const ccl::float3 &src, return_value_policy /* policy */,
                     handle /* parent */) {
    return py::make_tuple(src.x, src.y, src.z).inc_ref();
  }
};

template <> struct type_caster<ccl::float4> {
 public:
 PYBIND11_TYPE_CASTER(ccl::float4, _("float4"));
  bool load(handle src, bool) { return false; }

  static handle cast(const ccl::float4 &src, return_value_policy /* policy */,
                     handle /* parent */) {
    return py::make_tuple(src.x, src.y, src.z, src.w).inc_ref();
  }
};

template <> struct type_caster<ccl::Transform> {
 public:
 PYBIND11_TYPE_CASTER(ccl::Transform, _("Transform"));
  bool load(handle src, bool) { return false; }

  static handle cast(const ccl::Transform &src,
                     return_value_policy /* policy */, handle /* parent */) {
    return py::make_tuple(src.x, src.y, src.z).inc_ref();
  }
};

template <> struct type_caster<ccl::ustring> {
 public:
 PYBIND11_TYPE_CASTER(ccl::ustring, _("ustring"));
  bool load(handle src, bool) { return false; }

  static handle cast(const ccl::ustring &src, return_value_policy /* policy */,
                     handle /* parent */) {
    return py::str{src.string()}.inc_ref();
  }
};

template <typename Type, typename Alloc>
struct type_caster<ccl::vector<Type, Alloc>>
    : list_caster<ccl::vector<Type, Alloc>, Type> {};

template <> struct type_caster<ccl::NodeEnum> {
 public:
 PYBIND11_TYPE_CASTER(ccl::NodeEnum, _("ustring"));
  bool load(handle src, bool) { return false; }

  static handle cast(const ccl::NodeEnum &src, return_value_policy /* policy */,
                     handle /* parent */) {

    py::dict d;
    for (const auto & it : src) {
      d[py::cast(it.second)] = py::cast(it.first);
    }
    return d.inc_ref();
  }
};

} // namespace detail
} // namespace pybind11

namespace {

template <typename T> py::handle vector_cast(const ccl::vector<T> &v) {
  return py::object(py::detail::type_caster<ccl::vector<T>>::cast(
      v, py::return_value_policy::copy, py::handle()),
                    false)
      .inc_ref();
}

py::object default_value(const ccl::SocketType &socket_type) {
  using namespace ccl;

  // outputs do not have defined default value
  if (socket_type.default_value == nullptr) {
    return py::none();
  }

  // default values
  if (socket_type.type == SocketType::Type::BOOLEAN) {
    auto v = static_cast<const bool *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::FLOAT) {
    auto v = static_cast<const float *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::INT) {
    auto v = static_cast<const int *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::UINT) {
    auto v = static_cast<const unsigned int *>(socket_type.default_value);
    // return py::cast(v);
    return py::int_(static_cast<int>(*v));
  } else if (socket_type.type == SocketType::Type::COLOR ||
             socket_type.type == SocketType::Type::POINT ||
             socket_type.type == SocketType::Type::NORMAL ||
             socket_type.type == SocketType::Type::VECTOR) {
    auto v = static_cast<const ccl::float3 *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::POINT2) {
    auto v = static_cast<const ccl::float2 *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::STRING) {
    auto v = static_cast<const ustring *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::ENUM) {
    auto v = static_cast<const int *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::TRANSFORM) {
    auto v = static_cast<const ccl::Transform *>(socket_type.default_value);
    return py::cast(v);
  }

    // default arrays values
  else if (socket_type.type == SocketType::Type::BOOLEAN_ARRAY) {
    //    using t = ccl::vector<bool>;
    //    auto v = static_cast<const t *>(socket_type.default_value);
    return py::none();
  } else if (socket_type.type == SocketType::Type::FLOAT_ARRAY) {
    using t = ccl::vector<float>;
    auto v = static_cast<const t *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::INT_ARRAY) {
    using t = ccl::vector<int>;
    auto v = static_cast<const t *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::COLOR_ARRAY ||
             socket_type.type == SocketType::Type::POINT_ARRAY ||
             socket_type.type == SocketType::Type::NORMAL_ARRAY ||
             socket_type.type == SocketType::Type::VECTOR_ARRAY) {
    using t = ccl::vector<ccl::float3>;
    auto v = static_cast<const t *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::POINT2_ARRAY) {
    using t = ccl::vector<ccl::float2>;
    auto v = static_cast<const t *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::STRING_ARRAY) {
    using t = ccl::vector<ccl::ustring>;
    auto v = static_cast<const t *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::TRANSFORM_ARRAY) {
    using t = ccl::vector<ccl::Transform>;
    auto v = static_cast<const t *>(socket_type.default_value);
    return py::cast(v);
  } else if (socket_type.type == SocketType::Type::NODE_ARRAY) {
    return py::none();
  }

  return py::none();
}

} // namespace

PYBIND11_MODULE(BlackBird, m) {

  using namespace ccl;

  // session
  py::class_<SessionParams>(m, "SessionParams").def(py::init<>());
  py::class_<Session>(m, "Session").def(py::init<const SessionParams &>());

  // socket type
  py::class_<SocketType> socket_type(m, "SocketType");
  socket_type.def_readonly("name", &SocketType::name);
  socket_type.def_readonly("ui_name", &SocketType::ui_name);
  socket_type.def_readonly("type", &SocketType::type);
  socket_type.def_property_readonly("default_value", &default_value);
  socket_type.def_readonly("enum_values", &SocketType::enum_values);

  py::enum_<SocketType::Type>(socket_type, "Type")
      .value("UNDEFINED", SocketType::UNDEFINED)
      .value("BOOLEAN", SocketType::BOOLEAN)
      .value("FLOAT", SocketType::FLOAT)
      .value("INT", SocketType::INT)
      .value("UINT", SocketType::UINT)
      .value("COLOR", SocketType::COLOR)
      .value("VECTOR", SocketType::VECTOR)
      .value("POINT", SocketType::POINT)
      .value("NORMAL", SocketType::NORMAL)
      .value("POINT2", SocketType::POINT2)
      .value("CLOSURE", SocketType::CLOSURE)
      .value("STRING", SocketType::STRING)
      .value("ENUM", SocketType::ENUM)
      .value("TRANSFORM", SocketType::TRANSFORM)
      .value("NODE", SocketType::NODE)
      .value("BOOLEAN_ARRAY", SocketType::BOOLEAN_ARRAY)
      .value("FLOAT_ARRAY", SocketType::FLOAT_ARRAY)
      .value("INT_ARRAY", SocketType::INT_ARRAY)
      .value("COLOR_ARRAY", SocketType::COLOR_ARRAY)
      .value("VECTOR_ARRAY", SocketType::VECTOR_ARRAY)
      .value("POINT_ARRAY", SocketType::POINT_ARRAY)
      .value("NORMAL_ARRAY", SocketType::NORMAL_ARRAY)
      .value("POINT2_ARRAY", SocketType::POINT2_ARRAY)
      .value("STRING_ARRAY", SocketType::STRING_ARRAY)
      .value("TRANSFORM_ARRAY", SocketType::TRANSFORM_ARRAY)
      .value("NODE_ARRAY", SocketType::NODE_ARRAY)
      .export_values();

  // node type
  py::class_<NodeType> node_type(m, "NodeType");
  node_type.def_static("types", &NodeType::types);
  node_type.def_readonly("inputs", &NodeType::inputs);
  node_type.def_readonly("outputs", &NodeType::outputs);
}
