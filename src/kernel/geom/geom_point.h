/*
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

CCL_NAMESPACE_BEGIN

/* Point Primitive
 *
 * Point primitive for rendering point clouds.
 */

#ifdef __POINTCLOUD__

/* Reading attributes on various point elements */

ccl_device float point_attribute_float(
    KernelGlobals *kg, const ShaderData *sd, const AttributeDescriptor desc, float *dx, float *dy)
{
#  ifdef __RAY_DIFFERENTIALS__
  if (dx)
    *dx = 0.0f;
  if (dy)
    *dy = 0.0f;
#  endif

  if (desc.element == ATTR_ELEMENT_VERTEX) {
    return kernel_tex_fetch(__attributes_float, desc.offset + sd->prim);
  }
  else {
    return 0.0f;
  }
}

ccl_device float2 point_attribute_float2(KernelGlobals *kg,
                                         const ShaderData *sd,
                                         const AttributeDescriptor desc,
                                         float2 *dx,
                                         float2 *dy)
{
#  ifdef __RAY_DIFFERENTIALS__
  if (dx)
    *dx = make_float2(0.0f, 0.0f);
  if (dy)
    *dy = make_float2(0.0f, 0.0f);
#  endif

  if (desc.element == ATTR_ELEMENT_VERTEX) {
    return kernel_tex_fetch(__attributes_float2, desc.offset + sd->prim);
  }
  else {
    return make_float2(0.0f, 0.0f);
  }
}

ccl_device float3 point_attribute_float3(KernelGlobals *kg,
                                         const ShaderData *sd,
                                         const AttributeDescriptor desc,
                                         float3 *dx,
                                         float3 *dy)
{
#  ifdef __RAY_DIFFERENTIALS__
  if (dx)
    *dx = make_float3(0.0f, 0.0f, 0.0f);
  if (dy)
    *dy = make_float3(0.0f, 0.0f, 0.0f);
#  endif

  if (desc.element == ATTR_ELEMENT_VERTEX) {
    return float4_to_float3(kernel_tex_fetch(__attributes_float3, desc.offset + sd->prim));
  }
  else {
    return make_float3(0.0f, 0.0f, 0.0f);
  }
}

ccl_device float4 point_attribute_float4(KernelGlobals *kg,
                                         const ShaderData *sd,
                                         const AttributeDescriptor desc,
                                         float4 *dx,
                                         float4 *dy)
{
#  ifdef __RAY_DIFFERENTIALS__
  if (dx)
    *dx = make_float4(0.0f, 0.0f, 0.0f, 0.0f);
  if (dy)
    *dy = make_float4(0.0f, 0.0f, 0.0f, 0.0f);
#  endif

  if (desc.element == ATTR_ELEMENT_VERTEX) {
    return kernel_tex_fetch(__attributes_float3, desc.offset + sd->prim);
  }
  else {
    return make_float4(0.0f, 0.0f, 0.0f, 0.0f);
  }
}

/* Point radius */

ccl_device float point_radius(KernelGlobals *kg, ShaderData *sd)
{
  if (sd->type & PRIMITIVE_ALL_POINT) {
    return kernel_tex_fetch(__points, sd->prim).w;
  }

  return 0.0f;
}

/* Point location for motion pass, linear interpolation between keys and
 * ignoring radius because we do the same for the motion keys */

ccl_device float3 point_motion_center_location(KernelGlobals *kg, ShaderData *sd)
{
  return float4_to_float3(kernel_tex_fetch(__points, sd->prim));
}

#endif /* __POINTCLOUD__ */

CCL_NAMESPACE_END
