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
 *  @brief Primary header file for the rcluc library. Include this file to use the rcluc library
 */

#ifndef RCLUC__RCLUC_H_
#define RCLUC__RCLUC_H_

#include "rcluc/rcluc_default_configs.h"
#include "rcluc/rcluc_types.h"
#include "rcluc/rmwu.h"

/**
 *  @brief Initializes the client library. Must be called before calls to any other rcluc library functions
 *
 *  @param config The configuration data for the rcl client to use when establishing a connection.
 *  @return Returns an error code that will be RCLUC_RET_OK if initialized successfully
 */
rcluc_ret_t rcluc_init(const rcluc_client_config_t * config);

/**
 *  @brief Initializes a new ROS Node
 *  Used to initialize a preallocated ROS Node structure.
 *
 *  @param name The name of the Node. It is expected for this string to be null terminated.
 *  @param namespace_ The namespace for the Node. It is expected for this string to be null terminated.
 *  @param node_handle (output) A pointer to a node_handle that will be set to point to the newly created node
 *  @return Returns an error code that will be RCLUC_RET_OK if creation is successful
 */
rcluc_ret_t rcluc_node_create(const char * name, const char * namespace_, rcluc_node_handle_t * node_handle);

/**
 *  @brief Tears down the ROS node
 *  Used to delete an existing ROS node and free any resources that are tied to the node.
 *
 *  @param node_handle The handle for the node that you want to destroy
 *  @return Returns an error code that will be RCLUC_RET_OK if destroy is successful
 */
rcluc_ret_t rcluc_node_destroy(rcluc_node_handle_t node_handle);

/**
 *  @brief Runs tasks related to the node.
 *  This will run all the tasks related to a node, such as publishing data and receiving subscribed data. Any callbacks
 *  that are tied to these tasks will be executed as part of this.
 *
 *  TODO: After the implementation of this function come back and document what kind of guarantees we can make about
 *      how much is actually done when you call this function (ie. does it limit the number of received or sent messages
 *      that are transmitted to guarantee it will return). It's hard to give exact time guarantees without risking
 *      interrupting a callback function
 *
 *  @param node_handle The handle for the node with tasks that will be serviced.
 */
void rcluc_node_spin_once(rcluc_node_handle_t node_handle);

/**
 *  @brief Runs tasks related to the node forever or until the node is destroyed.
 *  This will run all the tasks related to a node, similar to rcluc_spin_node_once. However, this function will continue
 *  to service the node's tasks forever.
 *
 *  @param node_handle The handle for the node with tasks that will be serviced.
 */
void rcluc_node_spin_forever(rcluc_node_handle_t node_handle);

/**
 *  @brief Creates a new topic subscription on a node.
 *  Creates a new topic subscription on a node. Messages that come in on this topic will be desierialized using the
 *  provided function and then invoke the provided callback function with the desierialized message.
 *  //TODO: Decide if the topic should become imediately active (do the communication in this function) or wait until
 *      the next time spin is called. This can be decided during implementation and the behavior documented here
 *
 *  @param node_handle The handle for the node that this subscription will be created on
 *  @param topic_name The name of the topic that will be subscribed to. Expected to be a null terminated string
 *  @param message_type The message type information used by the library to handle the message type.
 *  @param callback The function to invoke when a message is received on this subscription
 *  @param queue_length The number of messages to queue for the incoming subscription
 *  @param message_buffer A pointer to a uint8_t array buffer that contains enough space for at least
 *      (message_size * queue_length). This buffer will be used by the library for the lifetime of the subscription
 *  @param config The subscription configuration. If NULL then the default configuration will be used
 *  @param subscription_handle (output) A pointer to a subscription handle that will be set with the handle for the
 *      subscription
 *  @return Returns an error code that will be RCLUC_RET_OK if create is successful
 */
rcluc_ret_t rcluc_subscription_create(rcluc_node_handle_t node_handle, const rcluc_message_type_support_t * message_type,
    const char * topic_name, rcluc_subscription_callback_t callback, const size_t queue_length, uint8_t *message_buffer,
    const rcluc_subscription_config_t * config, rcluc_subscription_handle_t * subscription_handle);

/*
 *  @brief Initializes the provided struct with the default config for a subscription.
 *  When passed a preallocated struct it will set all the fields to be the default values. You can then override
 *  these values with your own configuration. You should always initialize your configuration options with this
 *  function so that if new config options are added in the future your code will continue to work with the new default
 *  behaviors without the need to go back and fix your configuration setup.
 *
 *  @param config The config struct to be filed with the default values
 */
void rcluc_subscription_get_default_config(rcluc_subscription_config_t * config);

/**
 *  @brief Gets the reference to the user metadata for the subscription.
 *  Gets the reference to the user metadata for the subscription. The user can specify this metadata by setting it in the
 *  rcluc_subscription_config_t that it passes into the create function.
 *
 *  @param subscription_handle The handle to the subscription
 *  @return The reference to the user metadata.
 */
void * rcluc_subscription_get_user_metadata(const rcluc_subscription_handle_t subscription_handle);

/**
 *  @brief Destroys a subscription to a topic
 *  Destorys a subscription and unregisters it from the node it was created on.
 *
 *  @param subscription_handle The handle for the subscription that is being destroyed
 *  @return Returns an error code that will be RCLUC_RET_OK if destroy is successful
 */
rcluc_ret_t rcluc_subscription_destroy(rcluc_subscription_handle_t subscription_handle);

/**
 *  @brief Creates a new ROS publisher
 *  Creates a new ROS publisher. Publishers are used to publish messages on topics in ROS Nodes.
 *
 *  @param node_handle The handle for the ROS Node that this publisher will be created on.
 *  @param message_type The message type information used by the library to handle the message type.
 *  @param queue_length The number of messages to queue for the outgoing publish
 *  @param message_buffer A pointer to a uint8_t array buffer that contains enough space for at least
 *      (message_size * queue_length). This buffer will be used by the library for the lifetime of the publisher.
 *  @param config The publisher configuration. If NULL then the default configuration will be used
 *  @param publisher_handle (output) A reference to a publisher_handle that will be set to the handle for the new publisher
 *  @return Returns an error code that will be RCLUC_RET_OK if create is successful
 */
rcluc_ret_t rcluc_publisher_create(rcluc_node_handle_t node_handle,
    const rcluc_message_type_support_t * message_type, size_t queue_length, uint8_t * message_buffer,
    const rcluc_publisher_config_t * config, rcluc_publisher_handle_t * publisher_handle);

/*
 *  @brief Initializes the provided struct with the default configuration for a publisher.
 *  When passed a preallocated struct it will set all the fields to be the default values. You can then override
 *  these values with your own configuration. You should always initialize your configuration options with this
 *  function so that if new options are added in the future your code will continue to work with the new default
 *  behaviors without the need to go back and fix your configuration setup.
 *
 *  @param config The config struct to be filed with the default values
 */
void rcluc_publisher_get_default_config(rcluc_publisher_config_t * config);

/**
 *  @brief Gets the reference to the user metadata for the publisher.
 *  Gets the reference to the user metadata for the publisher. The user can specify this metadata by setting it in the
 *  rcluc_publisher_config_t that it passes into the create function.
 *
 *  @param publisher_handle The handle to the publisher
 *  @return The reference to the user metadata.
 */
void * rcluc_publisher_get_user_metadata(const rcluc_publisher_handle_t publisher_handle);

/**
 *  @brief Destroys the provided ROS publisher
 *  Destroys the ROS publisher and removes it from the Node it was created on
 *
 *  @param publisher_handle The handle for the publisher that is being destroyed
 *  @return Returns an error code that will be RCLUC_RET_OK if destroy is successful
 */
rcluc_ret_t rcluc_publisher_destroy(rcluc_publisher_handle_t publisher_handle);

/**
 *  @brief Publishes a message on a ROS Topic
 *  Publishes the provided message out on the ROS Topic that is referenced by the publisher_handle.
 *
 *  @param publisher_handle The handle for the ROS Topic this message will be published on
 *  @param message The message that is going to be published on the topic
 *  @return Returns an error code that will be RCLUC_RET_OK if publish is successful
 */
rcluc_ret_t rcluc_publisher_publish(rcluc_publisher_handle_t publisher_handle, const void * message);
#endif
