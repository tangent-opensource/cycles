/*
 * Copyright 2020 Blender Foundation
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

#ifndef __KERNEL_OIIO_GLOBALS_H__
#define __KERNEL_OIIO_GLOBALS_H__

#include "util/util_thread.h"
#include "util/util_vector.h"
#include <OpenImageIO/texture.h>

CCL_NAMESPACE_BEGIN

struct OIIOTexture {
  OIIO::TextureSystem::TextureHandle *handle;
  OIIO::TextureOpt::InterpMode interpolation;
  OIIO::TextureOpt::Wrap extension;
  bool is_linear;
};

struct OIIOGlobals {
  OIIO::TextureSystem *tex_sys;
  float diffuse_blur;
  float glossy_blur;
};

CCL_NAMESPACE_END

#endif
