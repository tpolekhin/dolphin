// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <vector>

#include "VideoCommon/AbstractPipeline.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/RenderState.h"
#include "VideoCommon/VertexManagerBase.h"
#include "VideoCommon/VertexShaderGen.h"

namespace Metal
{
class StreamBuffer;

class VertexManager : public VertexManagerBase
{
public:
  VertexManager();
  ~VertexManager();

  static VertexManager* GetInstance();

  bool CreateStreamingBuffers();
  void InvalidateUniforms();

  std::unique_ptr<NativeVertexFormat>
  CreateNativeVertexFormat(const PortableVertexDeclaration& vtx_decl) override;

  void UploadUtilityUniforms(const void* uniforms, u32 uniforms_size) override;

protected:
  void PrepareDrawBuffers(u32 stride);
  void ResetBuffer(u32 vertex_stride, bool cull_all) override;
  void CommitBuffer(u32 num_vertices, u32 vertex_stride, u32 num_indices, u32* out_base_vertex,
                    u32* out_base_index) override;
  void UploadConstants() override;
  void DrawCurrentBatch(u32 base_index, u32 num_indices, u32 base_vertex) override;

private:
  void UploadUniforms();
  void UploadAllUniforms();
  void vFlush();

  std::vector<u8> m_cpu_vertex_buffer;
  std::vector<u16> m_cpu_index_buffer;

  // The vertex/index buffers are reserved for VertexManager/GX only.
  // This is because they're cached between draw calls.
  std::unique_ptr<StreamBuffer> m_vertex_stream_buffer;
  std::unique_ptr<StreamBuffer> m_index_stream_buffer;

  u32 m_current_draw_base_vertex = 0;
  u32 m_current_draw_index_offset = 0;
};
}  // namespace Metal
