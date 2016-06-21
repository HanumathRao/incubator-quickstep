/**
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_optimizer/expressions/WindowAggregateFunction.hpp"

#include <string>
#include <utility>
#include <vector>

#include "expressions/aggregation/AggregateFunction.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "types/Type.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace expressions {

bool WindowAggregateFunction::isNullable() const {
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments_) {
    argument_types.emplace_back(&argument->getValueType());
  }

  const Type *return_type = window_aggregate_.resultTypeForArgumentTypes(argument_types);
  DCHECK(return_type != nullptr);
  return return_type->isNullable();
}

const Type& WindowAggregateFunction::getValueType() const {
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments_) {
    argument_types.emplace_back(&argument->getValueType());
  }

  const Type *return_type = window_aggregate_.resultTypeForArgumentTypes(argument_types);
  DCHECK(return_type != nullptr);
  return *return_type;
}

WindowAggregateFunctionPtr WindowAggregateFunction::Create(
    const ::quickstep::AggregateFunction &window_aggregate,
    const std::vector<ScalarPtr> &arguments,
    const WindowInfo *window_info,
    const bool is_distinct) {
#ifdef QUICKSTEP_DEBUG
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments) {
    argument_types.emplace_back(&argument->getValueType());
  }
  DCHECK(window_aggregate.canApplyToTypes(argument_types));
#endif  // QUICKSTEP_DEBUG

  return WindowAggregateFunctionPtr(
      new WindowAggregateFunction(window_aggregate, arguments, window_info, "", is_distinct));
}

WindowAggregateFunctionPtr WindowAggregateFunction::Create(
    const ::quickstep::AggregateFunction &window_aggregate,
    const std::vector<ScalarPtr> &arguments,
    const std::string window_name,
    const bool is_distinct) {
#ifdef QUICKSTEP_DEBUG
  std::vector<const Type*> argument_types;
  for (const ScalarPtr &argument : arguments) {
    argument_types.emplace_back(&argument->getValueType());
  }
  DCHECK(window_aggregate.canApplyToTypes(argument_types));
#endif  // QUICKSTEP_DEBUG

  return WindowAggregateFunctionPtr(
      new WindowAggregateFunction(window_aggregate, arguments, nullptr, window_name, is_distinct));
}

ExpressionPtr WindowAggregateFunction::copyWithNewChildren(
    const std::vector<ExpressionPtr> &new_children) const {
  std::vector<ScalarPtr> new_arguments;
  for (const ExpressionPtr &expression_ptr : new_children) {
    ScalarPtr expr_as_scalar;
    CHECK(SomeScalar::MatchesWithConditionalCast(expression_ptr, &expr_as_scalar))
        << expression_ptr->toString();
    new_arguments.emplace_back(std::move(expr_as_scalar));
  }

  return Create(window_aggregate_, new_arguments, window_info_, is_distinct_);
}

std::vector<AttributeReferencePtr> WindowAggregateFunction::getReferencedAttributes() const {
  std::vector<AttributeReferencePtr> referenced_attributes;
  for (const ScalarPtr &argument : arguments_) {
    const std::vector<AttributeReferencePtr> referenced_attributes_in_argument =
        argument->getReferencedAttributes();
    referenced_attributes.insert(referenced_attributes.end(),
                                 referenced_attributes_in_argument.begin(),
                                 referenced_attributes_in_argument.end());
  }
  return referenced_attributes;
}

void WindowAggregateFunction::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("function");
  inline_field_values->push_back(window_aggregate_.getName());

  if (is_distinct_) {
    inline_field_names->push_back("is_distinct");
    inline_field_values->push_back("true");
  }

  container_child_field_names->push_back("");
  container_child_fields->emplace_back(CastSharedPtrVector<OptimizerTreeBase>(arguments_));
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
