/* Copyright (C) 2015-2018 Michele Colledanchise -  All Rights Reserved
 * Copyright (C) 2018-2020 Davide Faconti, Eurecat -  All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <algorithm>
#include <cstddef>

#include "behaviortree_cpp_v3/controls/parallel_node.h"

namespace BT
{
constexpr const char* ParallelNode::THRESHOLD_FAILURE;
constexpr const char* ParallelNode::THRESHOLD_SUCCESS;

ParallelNode::ParallelNode(const std::string& name, int success_threshold,
                           int failure_threshold) :
  ControlNode::ControlNode(name, {}),
  success_threshold_(success_threshold),
  failure_threshold_(failure_threshold),
  read_parameter_from_ports_(false)
{
  setRegistrationID("Parallel");
}

ParallelNode::ParallelNode(const std::string& name, const NodeConfiguration& config) :
  ControlNode::ControlNode(name, config),
  success_threshold_(1),
  failure_threshold_(1),
  read_parameter_from_ports_(true)
{}

NodeStatus ParallelNode::tick()
{
  if (read_parameter_from_ports_)
  {
    if (!getInput(THRESHOLD_SUCCESS, success_threshold_))
    {
      throw RuntimeError("Missing parameter [", THRESHOLD_SUCCESS, "] in ParallelNode");
    }

    if (!getInput(THRESHOLD_FAILURE, failure_threshold_))
    {
      throw RuntimeError("Missing parameter [", THRESHOLD_FAILURE, "] in ParallelNode");
    }
  }

  size_t success_childred_num = 0;
  size_t failure_childred_num = 0;
  size_t log_childred_num = std::count_if(children_nodes_.begin(), children_nodes_.end(), [](const auto child_node) {
    return child_node->registrationName() == "Log";
  });

  const size_t children_count = children_nodes_.size();

  if (children_count - log_childred_num < successThreshold())
  {
    throw LogicError("Number of children is less than threshold. Can never succeed.");
  }

  if (children_count - log_childred_num < failureThreshold())
  {
    throw LogicError("Number of children is less than threshold. Can never fail.");
  }

  // Routing the tree according to the sequence node's logic:
  for (unsigned int i = 0; i < children_count; i++)
  {
    TreeNode* child_node = children_nodes_[i];

    bool in_skip_list = (skip_list_.count(i) != 0);

    NodeStatus child_status;
    if (in_skip_list)
    {
      child_status = child_node->status();
    }
    else
    {
      child_status = child_node->executeTick();
    }

    if (child_node->registrationName() == "Log")
    {
      if(!in_skip_list)
      {
        skip_list_.insert(i);
      }

      continue;
    }

    switch (child_status)
    {
      case NodeStatus::SUCCESS: {
        if (!in_skip_list)
        {
          skip_list_.insert(i);
        }
        success_childred_num++;

        if (success_childred_num == successThreshold())
        {
          skip_list_.clear();
          haltChildren();
          return NodeStatus::SUCCESS;
        }
      }
      break;

      case NodeStatus::FAILURE: {
        if (!in_skip_list)
        {
          skip_list_.insert(i);
        }
        failure_childred_num++;

        // It fails if it is not possible to succeed anymore or if
        // number of failures are equal to failure_threshold_
        if ((failure_childred_num > children_count - log_childred_num - successThreshold()) ||
            (failure_childred_num == failureThreshold()))
        {
          skip_list_.clear();
          haltChildren();
          return NodeStatus::FAILURE;
        }
      }
      break;

      case NodeStatus::RUNNING: {
        // do nothing
      }
      break;

      default: {
        throw LogicError("A child node must never return IDLE");
      }
    }
  }

  return NodeStatus::RUNNING;
}

void ParallelNode::halt()
{
  skip_list_.clear();
  ControlNode::halt();
}

size_t ParallelNode::successThreshold() const
{
  const size_t children_count = std::count_if(children_nodes_.begin(), children_nodes_.end(), [](const auto child_node) {
    return child_node->registrationName() != "Log";
  });

  return success_threshold_ < 0 ?
             std::max(children_count + success_threshold_ + 1, size_t(0)) :
             success_threshold_;
}

size_t ParallelNode::failureThreshold() const
{
  const size_t children_count = std::count_if(children_nodes_.begin(), children_nodes_.end(), [](const auto child_node) {
    return child_node->registrationName() != "Log";
  });

  return failure_threshold_ < 0 ?
             std::max(children_count + failure_threshold_ + 1, size_t(0)) :
             failure_threshold_;
}

void ParallelNode::setSuccessThreshold(int threshold_M)
{
  success_threshold_ = threshold_M;
}

void ParallelNode::setFailureThreshold(int threshold_M)
{
  failure_threshold_ = threshold_M;
}

}   // namespace BT
