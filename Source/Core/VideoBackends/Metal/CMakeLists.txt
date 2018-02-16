message(STATUS "Enabling experimental Metal graphics backend")

add_library(videometal
  CommandBufferManager.cpp
  MetalContext.cpp
  MetalFramebuffer.cpp
  MetalPipeline.cpp
  MetalShader.cpp
  MetalTexture.cpp
  MetalVertexFormat.cpp
  Render.cpp
  ShaderCompiler.cpp
  StateTracker.cpp
  StreamBuffer.cpp
  TextureCache.cpp
  Util.cpp
  VertexManager.cpp
  VideoBackend.cpp
  WindowFramebuffer.mm
)

target_link_libraries(videometal PRIVATE mtlpp videocommon common glslang spirv-cross)

# We require linking with QuartzCore to access the CoreAnimation Metal Layer.
# TODO: Is there a cmake command for linking with a framework?
target_link_libraries(videometal PRIVATE "-framework QuartzCore")

