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
 *  @brief This file defines the ROS MiddleWare Micro (rmwu) interface for the rcluc client library to use without
 *      knowing the specifics of the underlying DSS/transport layer implementation.
 */

#ifndef RCLUC__RMWU_H_
#define RCLUC__RMWU_H_

#include "rcluc/rmwu_types.h"
#include "rcluc/rcluc_types.h"

/**
 *  @brief Initializes the client library. Must be called before calls to any other rmwu library functions
 *
 *  @param config The configuration data for the rcl client to use when establishing a connection.
 *  @return Returns an error code that will be RCLUC_RET_OK if initialized successfully
 */
rcluc_ret_t rmwu_init(const rcluc_client_config_t * config);

/**
 *  @brief Initializes the RMWU information for a ROS Node
 *  Used to create a new ROS Node by the underlying RMWU implementation
 *
 *  @param name The name of the Node. It is expected for this string to be null terminated.
 *  @param namespace_ The namespace for the Node. It is expected for this string to be null terminated.
 *  @param node (output) A pointer to a preallocated rmwu node that should be initialized by the rmwu library
 *  @return Returns an error code that will be RCLUC_RET_OK if created successfully
 */
rcluc_ret_t rmwu_node_create(const char * name, const char * namespace_, rmwu_node_t * node);

/**
 *  @brief Tears down the ROS node
 *  Used to delete an existing ROS node and free any resources that are tied to the node.
 *
 *  @param node The handle for the node that you want to destroy
 *  @return Returns an error code that will be RCLUC_RET_OK if destroy is successful
 */
rcluc_ret_t rmwu_node_destroy(rmwu_node_t * node);

/**
 *  @brief Creates a new topic subscription in the underyling rmwu library
 *  Creates a new topic subscription in the underlying rmwu library.
 *
 *  @param node A reference to the node the subscription is being created on.
 *  @param topic_name The name of the topic that will be subscribed to. Expected to be a null terminated string
 *  @param message_type The message type information used by the library to handle the message type.
 *  @param callback The function to invoke when a message is received on this subscription
 *  @param queue_length The number of messages to queue for the incoming subscription
 *  @param message_buffer A pointer to a uint8_t array buffer that contains enough space for at least
 *      (message_size * queue_length). This buffer will be used by the library for the lifetime of the subscription
 *  @param config The subscription configuration. If NULL then the default configuration will be used
 *  @param subscription (output) A preallocated subscription object that will be initialized and hold the rmwu context
 *      for the new subscription
 *  @return Returns an error code that will be RCLUC_RET_OK if create is successful
 *
 *  TODO: Decide how many of these rcluc structs we want to pass down. It might make more sense keep the cnfiguration and
 *      message_type at the rcluc library layer. When doing the implementation for the RMWU we should think if it's somethig
 *      every RMWU implementation will need to do in the same way and if so then we should move it up a layer.
 *  TODO: During implementation decide if the callback passed down is the user level callback or if we should
 *      intercept it with a callback function in the rcluc layer to do message conversion etc there instead
 */
rcluc_ret_t rmwu_subscription_create(rmwu_node_t * node, const rcluc_message_type_support_t message_type,
    const char * topic_name, rcluc_subscription_callback_t callback, const size_t queue_length, uint8_t *message_buffer,
    const rcluc_subscription_config_t * config, rmwu_subscription_t * subscription);

/**
 *  @brief Destroys a subscription to a topic
 *  Destorys a subscription and unregisters it from the node it was created on.
 *
 *  @param subscription The reference for the subscription that is being destroyed
 *  @return Returns an error code that will be RCLUC_RET_OK if destroy is successful
 */
rcluc_ret_t rmwu_subscription_destroy(rmwu_subscription_t * subscription);

/**
 *  @brief Creates a new ROS publisher
 *  Creates a new ROS publisher. Publishers are used to publish messages on topics in ROS Nodes.
 *
 *  @param node A reference to the node the publisher is being created on.
 *  @param message_type The message type information used by the library to handle the message type.
 *  @param queue_length The number of messages to queue for the outgoing publish
 *  @param message_buffer A pointer to a uint8_t array buffer that contains enough space for at least
 *      (message_size * queue_length). This buffer will be used by the library for the lifetime of the publisher.
 *  @param config The publisher configuration. If NULL then the default configuration will be used
 *  @param publisher_handle (output) A reference to a publisher_handle that will be set to the handle for the new publisher
 *  @return Returns an error code that will be RCLUC_RET_OK if create is successful
 *
 *  TODO: Decide how many of these rcluc structs we want to pass down. It might make more sense keep the cnfiguration and
 *      message_type at the rcluc library layer. When doing the implementation for the RMWU we should think if it's somethig
 *      every RMWU implementation will need to do in the same way and if so then we should move it up a layer.
 */
rcluc_ret_t rmwu_publisher_create(rmwu_node_t * node, const rcluc_message_type_support_t message_type,
    size_t queue_length, uint8_t * message_buffer, const rcluc_publisher_config_t * config,
    rmwu_publisher_t * publisher);

/**
 *  @brief Destroys the provided ROS publisher
 *  Destroys the ROS publisher and removes it from the Node it was created on
 *
 *  @param publisher The reference for the publisher that is being destroyed
 *  @return Returns an error code that will be RCLUC_RET_OK if destroy is successful
 */
rcluc_ret_t rmwu_publisher_destroy(rmwu_publisher_t * publisher);

/**
 *  @brief Publishes a message on a ROS Topic
 *  Publishes the provided message out on the ROS Topic that is referenced by publisher
 *
 *  @param publisher_handle The handle for the ROS Topic this message will be published on
 *  @param message The message that is going to be published on the topic
 *  @return Returns an error code that will be RCLUC_RET_OK if publish is successful
 */
rcluc_ret_t rmwu_publisher_publish(rmwu_publisher_t * publisher, const void * message);

#endif /* ifndef RCLUC__RMWU_H_ */
