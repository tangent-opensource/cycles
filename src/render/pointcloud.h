/*
 * Copyright 2011-2020 Blender Foundation
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

#ifndef __POINTCLOUD_H__
#define __POINTCLOUD_H__

#include "render/geometry.h"

CCL_NAMESPACE_BEGIN

enum PointCloudPointStyle {
  POINT_CLOUD_POINT_SPHERE,
  POINT_CLOUD_POINT_DISC,
  POINT_CLOUD_POINT_DISC_ORIENTED
};

class PointCloud : public Geometry {
 public:
  NODE_DECLARE

  /* PointCloud Point */
  struct Point {
    int index;

    void bounds_grow(const float3 *points, const float *radius, BoundBox &bounds) const;
    void bounds_grow(const float3 *points,
                     const float *radius,
                     const Transform &aligned_space,
                     BoundBox &bounds) const;
    void bounds_grow(const float4 &point, BoundBox &bounds) const;

    float4 motion_key(const float3 *points,
                      const float *radius,
                      const float3 *point_steps,
                      size_t num_points,
                      size_t num_steps,
                      float time,
                      size_t p) const;
    float4 point_for_step(const float3 *points,
                          const float *radius,
                          const float3 *point_steps,
                          size_t num_points,
                          size_t num_steps,
                          size_t step,
                          size_t p) const;
  };

  NODE_SOCKET_API(PointCloudPointStyle, point_style);
  NODE_SOCKET_API_ARRAY(array<float3>, points);
  NODE_SOCKET_API_ARRAY(array<float>, radius);
  NODE_SOCKET_API_ARRAY(array<int>, shader);

  /* Constructor/Destructor */
  PointCloud();
  ~PointCloud();

  /* Geometry */
  void clear(const bool preserver_shaders = false) override;

  void resize(int numpoints);
  void reserve(int numpoints);
  void add_point(float3 loc, float radius, int shader = 0);

  void copy_center_to_motion_step(const int motion_step);

  void compute_bounds() override;
  void apply_transform(const Transform &tfm, const bool apply_to_motion) override;

  /* Points */
  Point get_point(int i) const
  {
    Point point = {i};
    return point;
  }

  size_t num_points() const
  {
    return points.size();
  }

  size_t num_attributes() const
  {
    return point_style == POINT_CLOUD_POINT_DISC_ORIENTED ? 2 : 1;
  }

  /* UDIM */
  void get_uv_tiles(ustring map, unordered_set<int> &tiles) override;

  /* BVH */
  void pack(Scene *scene, float4 *packed_points, uint *packed_shader);
  void pack_primitives(PackedBVH *pack, int object, uint visibility, PackFlags pack_flag) override;
};

CCL_NAMESPACE_END

#endif /* __POINTCLOUD_H__ */
