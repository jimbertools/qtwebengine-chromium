// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_THEMES_AUTOGENERATED_THEME_UTIL_H_
#define CHROME_COMMON_THEMES_AUTOGENERATED_THEME_UTIL_H_

#include "third_party/skia/include/core/SkColor.h"

// Constants for autogenerated themes.
// Minimum contrast for active tab and frame color to avoid isolation line in
// the tab strip.
constexpr float kAutogeneratedThemeActiveTabMinContrast = 1.3f;
constexpr float kAutogeneratedThemeActiveTabPreferredContrast = 1.6f;
constexpr float kAutogeneratedThemeActiveTabPreferredContrastForDark = 1.7f;

// Contrast between foreground and background.
constexpr float kAutogeneratedThemeTextPreferredContrast = 7.0f;

struct AutogeneratedThemeColors {
  SkColor frame_color;
  SkColor frame_text_color;
  SkColor active_tab_color;
  SkColor active_tab_text_color;
};

// Generates theme colors for the given |color|.
AutogeneratedThemeColors GetAutogeneratedThemeColors(SkColor color);

#endif  // CHROME_COMMON_THEMES_AUTOGENERATED_THEME_UTIL_H_