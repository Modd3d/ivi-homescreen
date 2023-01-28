/*
 * @copyright Copyright (c) 2022 Woven Alpha, Inc.
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

#include <map>
#include <memory>
#include <string>

#include "flutter/fml/macros.h"
#include <flutter/binary_messenger.h>
#include <flutter/basic_message_channel.h>
#include <flutter/shell/platform/common/json_message_codec.h>

#include <flutter_embedder.h>
#include <rapidjson/document.h>
#include <xkbcommon/xkbcommon.h>

#include "constants.h"

class Engine;

class KeyEvent : public flutter::BinaryMessenger {
public:
  static constexpr char kChannelName[] = "flutter/keyevent";

  explicit KeyEvent();

  /**
  * @brief Callback function for platform messages about keyevent
  * @param[in] message Received message
  * @param[in] userdata Pointer to User data
  * @return void
  * @relation
  * flutter
  *
  * When a key event is sent through "flutter/keyevent",
  * any platform messages seems not to be received.
  * So, this callback is defined, but, not registered.
  */
  MAYBE_UNUSED static void OnPlatformMessage(const FlutterPlatformMessage* message,
                                             void* userdata);

  /**
  * @brief Set the flutter engine for key event
  * @param[in] engine Pointer to Flutter engine
  * @return void
  * @relation
  * flutter
  */
  void SetEngine(const std::shared_ptr<Engine>& engine);

  /**
  * @brief the handler for key event
  * @param[in] data Pointer to Ueer data
  * @param[in] type the type of Flutter KeyEvent
  * @param[in] code key code
  * @param[in] sym key symbol
  * @return void
  * @relation
  * flutter
  *
  * TODO: support modifiers
  * WARNING:
  *   this api's interface is not stable.
  *   After supporting modifiers, the interface will be changed.
  */
  static void keyboard_handle_key(void* data,
                                  FlutterKeyEventType type,
                                  uint32_t code,
                                  xkb_keysym_t sym);

  FML_DISALLOW_COPY_AND_ASSIGN(KeyEvent);

private:
  static constexpr char kKeyCodeKey[] = "keyCode";
  static constexpr char kKeyMapKey[] = "keymap";
  static constexpr char kScanCodeKey[] = "scanCode";
  static constexpr char kModifiersKey[] = "modifiers";
  static constexpr char kTypeKey[] = "type";
  static constexpr char kToolkitKey[] = "toolkit";
  static constexpr char kUnicodeScalarValues[] = "unicodeScalarValues";
  static constexpr char kLinuxKeyMap[] = "linux";
  static constexpr char kGLFWKey[] = "glfw";
  static constexpr char kKeyUp[] = "keyup";
  static constexpr char kKeyDown[] = "keydown";

  std::shared_ptr<Engine> engine_;

  std::unique_ptr<flutter::BasicMessageChannel<rapidjson::Document>> channel_;

  /**
  * @brief convert key event from xkb to flutter and send it to flutter engine
  * @param[in] pressed true if a key is pressed and vice versa
  * @param[in] sym key symbol
  * @param[in] code key code
  * @param[in] modifiers key modifiers
  * @return void
  * @relation
  * flutter, wayland
  */
  void SendKeyEvent(bool pressed, xkb_keysym_t sym, uint32_t code, uint32_t modifiers);

  /**
  * @brief Send a message to the Flutter engine on this channel
  * @param[in] channel This channel
  * @param[in] message a message
  * @param[in] message_size the size of a message
  * @param[in] reply Binary message reply callback
  * @return void
  * @relation
  * flutter
  */
  void Send(const std::string& channel,
            const uint8_t* message,
            size_t message_size,
            flutter::BinaryReply reply) const override;

  /**
  * @brief Registers a handler to be invoked when the Flutter application sends a message to its host platform
  * @param[in] channel This channel
  * @param[in] handler a handler for incoming binary messages from Flutter
  * @return void
  * @relation
  * flutter
  */
  MAYBE_UNUSED NODISCARD void SetMessageHandler(
                                  const std::string& channel,
                                  flutter::BinaryMessageHandler handler) override;
};
