// Copyright 2018 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include "VideoCommon/VideoBackendBase.h"

namespace Metal
{
class VideoBackend : public VideoBackendBase
{
public:
  bool Initialize(const WindowSystemInfo& wsi) override;
  void Shutdown() override;

  std::string GetName() const override { return "Metal"; }
  std::string GetDisplayName() const override { return "Metal"; }
  void InitBackendInfo() override;
  //void PrepareWindow(const WindowSystemInfo& wsi) override;
};
}  // namespace Metal
