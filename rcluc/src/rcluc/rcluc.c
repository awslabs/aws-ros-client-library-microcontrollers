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
 *  @brief Implementation of the RCLUC library
 */

#include "rcluc/rcluc.h"
#include "rcluc/rmwu.h"
#include "rcluc/rcluc_types.h"
#include "rcluc/rmwu_types.h"

rcluc_ret_t rcluc_init(const rcluc_client_config_t * config) {
    return rmwu_init(config);
}

rcluc_ret_t rcluc_node_create(const char * name, const char * namespace_, rcluc_node_handle_t * node_handle) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rcluc_node_destroy(rcluc_node_handle_t node_handle) {
    return RCLUC_RET_ERROR;
}

void rcluc_node_spin_once(rcluc_node_handle_t node_handle) {
}

void rcluc_node_spin_forever(rcluc_node_handle_t node_handle) {
}

rcluc_ret_t rcluc_subscription_create(rcluc_node_handle_t node_handle, const rcluc_message_type_support_t * message_type,
    const char * topic_name, rcluc_subscription_callback_t callback, const size_t queue_length, uint8_t *message_buffer,
    const rcluc_subscription_config_t * config, rcluc_subscription_handle_t * subscription_handle) {
    return RCLUC_RET_ERROR;
}

void rcluc_subscription_get_default_config(rcluc_subscription_config_t * config) {
}

void * rcluc_subscription_get_user_metadata(const rcluc_subscription_handle_t subscription_handle) {
    return NULL;
}

rcluc_ret_t rcluc_subscription_destroy(rcluc_subscription_handle_t subscription_handle) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rcluc_publisher_create(rcluc_node_handle_t node_handle,
    const rcluc_message_type_support_t * message_type, size_t queue_length, uint8_t * message_buffer,
    const rcluc_publisher_config_t * config, rcluc_publisher_handle_t * publisher_handle) {
    return RCLUC_RET_ERROR;
}

void rcluc_publisher_get_default_config(rcluc_publisher_config_t * config) {
}

void * rcluc_publisher_get_user_metadata(const rcluc_publisher_handle_t publisher_handle) {
    return NULL;
}

rcluc_ret_t rcluc_publisher_destroy(rcluc_publisher_handle_t publisher_handle) {
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rcluc_publisher_publish(rcluc_publisher_handle_t publisher_handle, const void * message) {
    return RCLUC_RET_ERROR;
}
