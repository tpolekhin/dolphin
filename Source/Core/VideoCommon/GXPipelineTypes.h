// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "VideoCommon/GeometryShaderGen.h"
#include "VideoCommon/NativeVertexFormat.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/RenderState.h"
#include "VideoCommon/UberShaderPixel.h"
#include "VideoCommon/UberShaderVertex.h"
#include "VideoCommon/VertexShaderGen.h"

class NativeVertexFormat;

namespace VideoCommon
{
// This version number must be incremented whenever any of the shader UID structures change.
// As pipelines encompass both shader UIDs and render states, changes to either of these should
// also increment the pipeline UID version. Incrementing the UID version will cause all UID
// caches to be invalidated.
constexpr u32 GX_PIPELINE_UID_VERSION = 1;  // Last changed in PR 6431

struct GXPipelineUid
{
  const NativeVertexFormat* vertex_format;
  VertexShaderUid vs_uid;
  GeometryShaderUid gs_uid;
  PixelShaderUid ps_uid;
  RasterizationState rasterization_state;
  DepthState depth_state;
  BlendingState blending_state;

  // We use memcmp() for comparing pipelines as std::tie generates a large number of instructions,
  // and this map lookup can happen every draw call. However, as using memcmp() will also compare
  // any padding bytes, we have to ensure these are zeroed out.
  GXPipelineUid() { std::memset(static_cast<void*>(this), 0, sizeof(*this)); }
  GXPipelineUid(const GXPipelineUid& rhs)
  {
    std::memcpy(static_cast<void*>(this), &rhs, sizeof(*this));
  }
  GXPipelineUid& operator=(const GXPipelineUid& rhs)
  {
    std::memcpy(static_cast<void*>(this), &rhs, sizeof(*this));
    return *this;
  }
  bool operator<(const GXPipelineUid& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(*this)) < 0;
  }
  bool operator==(const GXPipelineUid& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(*this)) == 0;
  }
  bool operator!=(const GXPipelineUid& rhs) const { return !operator==(rhs); }
};
struct GXUberPipelineUid
{
  const NativeVertexFormat* vertex_format;
  UberShader::VertexShaderUid vs_uid;
  GeometryShaderUid gs_uid;
  UberShader::PixelShaderUid ps_uid;
  RasterizationState rasterization_state;
  DepthState depth_state;
  BlendingState blending_state;

  GXUberPipelineUid() { std::memset(static_cast<void*>(this), 0, sizeof(*this)); }
  GXUberPipelineUid(const GXUberPipelineUid& rhs)
  {
    std::memcpy(static_cast<void*>(this), &rhs, sizeof(*this));
  }
  GXUberPipelineUid& operator=(const GXUberPipelineUid& rhs)
  {
    std::memcpy(static_cast<void*>(this), &rhs, sizeof(*this));
    return *this;
  }
  bool operator<(const GXUberPipelineUid& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(*this)) < 0;
  }
  bool operator==(const GXUberPipelineUid& rhs) const
  {
    return std::memcmp(this, &rhs, sizeof(*this)) == 0;
  }
  bool operator!=(const GXUberPipelineUid& rhs) const { return !operator==(rhs); }
};

// Disk cache of pipeline UIDs. We can't use the whole UID as a type as it contains pointers.
// This structure is safe to save to disk, and should be compiler/platform independent.
#pragma pack(push, 1)
struct SerializedGXPipelineUid
{
  PortableVertexDeclaration vertex_decl;
  VertexShaderUid vs_uid;
  GeometryShaderUid gs_uid;
  PixelShaderUid ps_uid;
  u32 rasterization_state_bits;
  u32 depth_state_bits;
  u32 blending_state_bits;
};
#pragma pack(pop)

struct EFBClearPipelineConfig
{
  EFBClearPipelineConfig(bool color_enable_, bool alpha_enable_, bool depth_enable_)
      : color_enable(color_enable_), alpha_enable(alpha_enable_), depth_enable(depth_enable_)
  {
  }

  bool color_enable;
  bool alpha_enable;
  bool depth_enable;

  bool operator<(const EFBClearPipelineConfig& rhs) const
  {
    return std::tie(color_enable, alpha_enable, depth_enable) <
           std::tie(rhs.color_enable, rhs.alpha_enable, rhs.depth_enable);
  }
};
struct BlitPipelineConfig
{
  BlitPipelineConfig(AbstractTextureFormat dst_format_, bool stereo_ = false)
      : dst_format(dst_format_), stereo(stereo_)
  {
  }

  AbstractTextureFormat dst_format;
  bool stereo;

  bool operator<(const BlitPipelineConfig& rhs) const
  {
    return std::tie(dst_format, stereo) < std::tie(rhs.dst_format, rhs.stereo);
  }
};

struct UtilityVertex
{
  float position[4] = {};
  float uv[3] = {};
  u32 color = 0;

  UtilityVertex() = default;
  void SetPosition(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f)
  {
    position[0] = x;
    position[1] = y;
    position[2] = z;
    position[3] = w;
  }
  void SetTextureCoordinates(float u = 0.0f, float v = 0.0f, float layer = 0.0f)
  {
    uv[0] = u;
    uv[1] = v;
    uv[2] = layer;
  }
  void SetColor(u32 color_) { color = color_; }
  void SetColorRGBA(u32 r = 0, u32 g = 0, u32 b = 0, u32 a = 255)
  {
    color = (r) | (g << 8) | (b << 16) | (a << 24);
  }
};

}  // namespace VideoCommon
