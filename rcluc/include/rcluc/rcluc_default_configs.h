/*
 * Copyright 2010-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */


#include "rcluc/types.h"
/**
 *  @file
 *  @brief Defines default values for the library to use if not overriden by a configuration from the source application
 */

#ifndef RCLUC__RCLUC_DEFAULT_CONFIGS_H_
#define RCLUC__RCLUC_DEFAULT_CONFIGS_H_

#ifndef configRCLUC_MAX_NUM_NODES
/**
 *  @brief The maximum number of Ros Nodes that can be created
 */
#define configRCLUC_MAX_NUM_NODES 1
#endif

#ifndef configRCLUC_MAX_SUBSCRIPTIONS_PER_NODE
/**
 *  @brief The maximum number of subscriptions that can be created on each ROS Node instance
 */
#define configRCLUC_MAX_SUBSCRIPTIONS_PER_NODE 1
#endif

#ifndef configRCLUC_MAX_PUBLISHERS_PER_NODE
/**
 *  @brief The maximum number of publishers that can be created on each ROS Node instance
 */
#define configRCLUC_MAX_PUBLISHERS_PER_NODE 1
#endif

#ifndef configRCLUC_MAX_MESSAGE_SIZE_BYTES
/**
 *  @brief The maximum size (in bytes) for messages being sent or received on Topics.
 */
#define configRCLUC_MAX_MESSAGE_SIZE_BYTES 1024
#endif

#ifndef configRCLUC_MAX_TOPIC_NAME_LEN
/**
 *  @brief The maximum length of a Topic name or namespace used for a ROS Topic.
 */
#define configRCLUC_MAX_TOPIC_NAME_LEN 32
#endif

#ifndef configRCLUC_SUPPORT_SUBSCRIPTION_DESERIALIZATION
/**
 *  @brief Defines what type of deserialization is supported. By default deserialization is disabled.
 *  There are two supported modes of deserialization supported. You can define deserialization support to be either of
 *  the following:
 *      1) RCLUC_SUBSCRIPTION_DESERIALIZATION_STATIC_ALLOCATION - This mode will statically allocate memory for every
 *          subscription during compile time.
 *      2) RCLUC_SUBSCRIPTION_DESERIALIZATION_STACK_ALLOCATION - This mode will allocate memory on the static to
 *          deserialize a message before invoking the callback.
 *      3) RCLUC_SUBSCRIPTION_DESERIALIZATION_DISABLED - This mode disables deserialization of subscription messages
 *          in the rcluc library. In this case clients will be given the raw message in the subscription callback and
 *          will be expected to perform their own deserialization.
 */
#define configRCLUC_SUBSCRIPTION_DESERIALIZATION_SUPPORT RCLUC_SUBSCRIPTION_DESERIALIZATION_DISABLED
#endif

#endif
