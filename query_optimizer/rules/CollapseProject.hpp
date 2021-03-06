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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_COLLAPSE_PROJECT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_COLLAPSE_PROJECT_HPP_

#include <string>
#include <vector>

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Merges nested Projects into one single Project.
 */
class CollapseProject : public BottomUpRule<logical::Logical> {
 public:
  CollapseProject() {}

  std::string getName() const override { return "CollapseProject"; }

 protected:
  logical::LogicalPtr applyToNode(const logical::LogicalPtr &input) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(CollapseProject);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_COLLAPSE_PROJECT_HPP_ */
