/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include "cli/LineReaderLineNoise.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string>
#include <utility>

#include "third_party/linenoise/linenoise.h"

namespace quickstep {

LineReaderLineNoise::LineReaderLineNoise(const std::string &default_prompt,
                                         const std::string &continue_prompt)
    : LineReader(default_prompt, continue_prompt) {
}

std::string LineReaderLineNoise::getLineInternal(const bool continuing) {
  const char *prompt;
  if (continuing) {
    prompt = continue_prompt_.c_str();
  } else {
    prompt = default_prompt_.c_str();
  }
  char *input_line;

  if ((input_line = linenoise(prompt)) != nullptr) {
    std::string retval(input_line);
    if (continuing
        || !std::all_of(retval.begin(), retval.end(), ::isspace)) {
      // Don't add a blank line that's not part of a command to history.
      linenoiseHistoryAdd(input_line);
    }
    std::free(input_line);
    return retval;
  } else {
    return std::string("");
  }
}

}  // namespace quickstep
