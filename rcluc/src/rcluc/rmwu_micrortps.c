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

/**
 *  @file
 *  @brief This file implements the ROS MiddleWare Micro (rmwu) interface for the rcluc client library using the
 *  library micro-RTPS.
 */

#include "rcluc/rmwu_types.h"
#include "rcluc/rcluc_types.h"

struct rmwu_node_s {

};

struct rmwu_subscription_s {

};

struct rmwu_publisher_s {

};

rcluc_ret_t rmwu_init(const rcluc_client_config_t * config) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rmwu_node_create(const char * name, const char * namespace_, rmwu_node_t * node) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rmwu_node_destroy(rmwu_node_t * node) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rmwu_subscription_create(rmwu_node_t * node, const rcluc_message_type_support_t * message_type,
    const char * topic_name, rcluc_subscription_callback_t callback, const size_t queue_length, uint8_t *message_buffer,
    const rcluc_subscription_config_t * config, rmwu_subscription_t * subscription) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rmwu_subscription_destroy(rmwu_subscription_t * subscription) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rmwu_publisher_create(rmwu_node_t * node, const rcluc_message_type_support_t * message_type,
    size_t queue_length, uint8_t * message_buffer, const rcluc_publisher_config_t * config,
    rmwu_publisher_t * publisher) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rmwu_publisher_destroy(rmwu_publisher_t * publisher) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rmwu_publisher_publish(rmwu_publisher_t * publisher, const void * message) {
    return RCLUC_RET_ERROR;
}
