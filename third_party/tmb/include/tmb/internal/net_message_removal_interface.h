//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#ifndef TMB_INTERNAL_NET_MESSAGE_REMOVAL_INTERFACE_H_
#define TMB_INTERNAL_NET_MESSAGE_REMOVAL_INTERFACE_H_

#include <cstdint>
#include <vector>

#include "tmb/id_typedefs.h"

namespace tmb {
namespace internal {

/**
 * @brief Abstract interface class that allows deletion and cancellation of
 *        messages by their IDs.
 **/
class NetMessageRemovalInterface {
 public:
  NetMessageRemovalInterface() {
  }

  virtual ~NetMessageRemovalInterface() {
  }

  virtual void DeleteById(
      const client_id receiver_id,
      const std::vector<std::int64_t> &message_ids) = 0;

  virtual void SenderCancelById(
      const client_id sender_id,
      const std::vector<std::int64_t> &message_ids) = 0;

  virtual void ReceiverCancelById(
      const client_id receiver_id,
      const std::vector<std::int64_t> &message_ids) = 0;

 private:
  // Disallow copy and assign:
  NetMessageRemovalInterface(const NetMessageRemovalInterface &orig) = delete;
  NetMessageRemovalInterface& operator=(const NetMessageRemovalInterface &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_NET_MESSAGE_REMOVAL_INTERFACE_H_
