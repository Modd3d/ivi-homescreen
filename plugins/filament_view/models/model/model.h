/*
 * Copyright 2020-2023 Toyota Connected North America
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <functional>
#include <optional>
#include <string>

#include "models/model/animation/animation.h"
#include "models/scene/geometry/position.h"
#include "models/scene/scene.h"

namespace plugin_filament_view {

class Animation;

class Model {
 public:
  Model(void* parent,
        const std::string& flutter_assets_path,
        const flutter::EncodableValue& params);
  ~Model() = default;
  void Print(const char* tag);

  bool isGlb() { return is_glb_.has_value() && is_glb_.value(); }

  std::string GetAssetPath() { return assetPath_; }
  std::string GetUrl() { return url_; }

  float GetScale() { return scale_; }

  Position* GetCenterPosition() {
    if (center_position_.has_value()) {
      return center_position_.value().get();
    }
    return nullptr;
  }

  std::optional<Model*> GetFallback() const {
    if (fallback_.has_value()) {
      return {fallback_.value().get()};
    } else {
      return std::nullopt;
    }
  }

  std::optional<Animation*> GetAnimation() const {
    if (fallback_.has_value()) {
      return {animation_.value().get()};
    } else {
      return std::nullopt;
    }
  }

  // Disallow copy and assign.
  Model(const Model&) = delete;
  Model& operator=(const Model&) = delete;

 private:
  const std::string& flutterAssetsPath_;
  std::string url_;
  std::string pathPrefix_;
  std::string pathPostfix_;
  std::optional<std::unique_ptr<Model>> fallback_;
  float scale_ = 1.0;
  std::optional<std::unique_ptr<Position>> center_position_;
  std::optional<std::unique_ptr<Animation>> animation_;
  std::optional<std::unique_ptr<Scene>> scene_;
  std::optional<bool> is_glb_;
  std::string assetPath_;
};

}  // namespace plugin_filament_view
