/*
 * Copyright 2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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
 *  @brief An example of how to make a publisher
 */

#include "rcluc/rcluc.h"
#include "rcluc_HelloWorld.h"
#include "rcluc/rmwu_types.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <micrortps/client/client.h>

#define MAX_MESSAGES_IN_BUFFER      2
#define TIME_BETWEEN_PUBLISH_SEC    1

void exception_callback(const rcluc_publisher_handle_t publisher, rcluc_ret_t error) {
    printf("Exception Callback, Error: %d\n", error);
}

int main(int args, char** argv)
{
    rcluc_ret_t err = RCLUC_RET_OK;
    rmwu_transport_config_t transport_config = {0};
    rcluc_client_config_t client_config = {0};
    rcluc_node_handle_t node;
    rcluc_publisher_handle_t publisher;
    uint8_t buffer[MAX_MESSAGES_IN_BUFFER * sizeof(rcluc_HelloWorld_t)];
    rcluc_publisher_config_t  publisher_config = {0};
    time_t wait_till_time = 0;
    mrUDPTransport transport;
    rcluc_HelloWorld_t hello_world = {0};
    strncpy(hello_world.message, "Hello World!", 255);

    // Initialize the transport layer specific code. It would be good to get this behind an abstraction layer in the future
    if(!mr_init_udp_transport(&transport, "127.0.0.1", 2018)) {
        printf("Error at create transport.\n");
        return 1;
    }
    transport_config.comm = &transport.comm;
    client_config.transport_layer_config = &transport_config;
    client_config.client_key = 0xAAAABBBB;

    err = rcluc_init(&client_config);
    if (RCLUC_RET_OK != err) {
        printf("File: %s, $Line: %d, Error: %d\n", __FILE__, __LINE__, err);
        return err;
    }

    err = rcluc_node_create("HelloWorld", "", &node);
    if (RCLUC_RET_OK != err) {
        printf("File: %s, $Line: %d, Error: %d\n", __FILE__, __LINE__, err);
        return err;
    }

    rcluc_publisher_get_default_config(&publisher_config);
    publisher_config.exception_callback = exception_callback;

    err = rcluc_publisher_create(node, rcluc_HelloWorld_get_type_support(), MAX_MESSAGES_IN_BUFFER, buffer,
        &publisher_config, &publisher);
    if (RCLUC_RET_OK != err) {
        printf("File: %s, $Line: %d, Error: %d\n", __FILE__, __LINE__, err);
        return err;
    }

    while (1) {
        hello_world.index++;
        err = rcluc_publisher_publish(publisher, &hello_world);
        if (RCLUC_RET_OK != err) {
            printf("File: %s, $Line: %d, Error: %d\n", __FILE__, __LINE__, err);
            return err;
        }

        wait_till_time = time(NULL) + TIME_BETWEEN_PUBLISH_SEC;
        while (wait_till_time > time(NULL)) {
            rcluc_node_spin_once(node);
        }
    }

    return 0;
}
