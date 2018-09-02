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
 *  @brief ROS Message type for HelloWorld
 *
 *  The file is writen manually for now, but will eventually be automatically generated.
 */

#ifndef RCLUC__RCLUC_HELLOWORLD_H_
#define RCLUC__RCLUC_HELLOWORLD_H_

#include <stdint.h>
#include "rcluc/rcluc_types.h"

typedef struct {
    uint32_t index;
    char message[255];
} rcluc_HelloWorld_t;

rcluc_ret_t rcluc_HelloWorld_deserialize(void * message_buffer, size_t message_buffer_size,
        rcluc_HelloWorld_t * deserialized_message, size_t deserialized_message_size) {
    //TODO
    return RCLUC_RET_ERROR;
}

rcluc_ret_t rcluc_HelloWorld_serialize(rcluc_HelloWorld_t * message, void * serializedBuffer) {
    //TODO:
    return RCLUC_RET_ERROR;
}

static const rcluc_message_type_support_t rcluc_HelloWorld_type_support = {
    sizeof(rcluc_HelloWorld_t),
    (rcluc_message_serialization_func_t)rcluc_HelloWorld_serialize,
    (rcluc_message_deserialization_func_t)rcluc_HelloWorld_deserialize
};

const rcluc_message_type_support_t* rcluc_HelloWorld_get_type_support() {
    return &rcluc_HelloWorld_type_support;
}

#endif /* ifndef RMWU__RMWU_HELLOWORLD_H_ */
