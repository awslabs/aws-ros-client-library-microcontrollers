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
 *  @brief Defines common types used by the rcluc library
 */

#include <stddef.h>
#include <stdint.h>

#ifndef RCLUC__TYPES_H_
#define RCLUC__TYPES_H_

/**
 *  @brief An error code used by the rcluc library
 */
typedef uint16_t rcluc_ret_t;
/**
 * @brief Indicates that no error has occured
 */
#define RCLUC_RET_OK        0
/**
 * @brief Indicates a generic error
 */
#define RCLUC_RET_ERROR     1
/**
 * @brief Indicates an error related to a timeout
 */
#define RCLUC_RET_TIMEOUT   2
/**
 * @brief Indicates an error related to a null pointer
 */
#define RCLUC_RET_NULL_PTR  3
/**
 * @brief Indicates an error with a provided parameter
 */
#define RCLUC_RET_ERR_PARAM 4
/**
 * @brief Indicates an error due to not being initialized
 */
#define RCLUC_RET_ERR_INIT 5
/**
 * @brief Indicates an error due to an event having already happened
 */
#define RCLUC_RET_ERR_ALREADY 6



/**
 *  @brief An handle to a ROS Node instance being managed by the rcluc library
 */
typedef struct rcluc_node_s* rcluc_node_handle_t;

/**
 *  @brief An handle to a ROS Topic Subscription instance being managed by the rcluc library
 */
typedef struct rcluc_subscription_s* rcluc_subscription_handle_t;

/**
 *  @brief An handle to a ROS Topic Publisher instance being managed by the rcluc library
 */
typedef struct rcluc_publisher_s* rcluc_publisher_handle_t;

/**
 *  @struct rcluc_client_config_t
 *  @brief The configuration information for a rcluc client
 *  @var rcluc_client_config_t::dds_domain
 *      The domain for this client to participate in
 *  @var rcluc_client_config_t::transport_layer_config
 *      A reference to configuration data for the client's transport layer. Can be set to NULL
 *      if the transport layer does not require any configuration data.
 */
typedef struct {
    int16_t dds_domain;
    void * transport_layer_config;
} rcluc_client_config_t;

/**
 * @brief Represents the different levels of reliability for a ROS Topic.
 */
typedef enum {
    RCLUC_TOPIC_RELIABILITY_BEST_EFFORT,
    RCLUC_TOPIC_RELIABILITY_RELIABLE
} rcluc_topic_reliability_t;

/**
 *  @brief The construct for a message deserialization function.
 *  This is the function type for a deserialization function that can be used to deserialize raw message received
 *  by a topic into a format useful for the user.
 *
 *  @param message_buffer The raw serialized message that needs to be deserialized into data
 *  @param message_buffer_size The size (in bytes) of the data in the message_buffer
 *  @param deserialized_buffer (output) The deserialized message data will be set here
 *  @param deserialized_buffer_size The size of the buffer to deserialize the data into
 *  @return Returns an error code that will be RCLUC_RET_OK if the message is deserialized successfully
 */
typedef rcluc_ret_t (*rcluc_message_deserialization_func_t)(void * message_buffer, size_t message_buffer_size,
    void * deserialized_buffer, size_t deserialized_buffer_size);

/**
 *  @brief The construct for a message serialization function.
 *  This is the function type for a serialization function that can be used to serialize messages before they are sent
 *  out on a topic.
 *
 *  This function is responsible for ensuring that the serialized data is not more than configRCLUC_MAX_MESSAGE_SIZE_BYTES
 *  in length.
 *
 *  @param message The message that needs to be serialized into a buffer
 *  @param serializedBuffer (output) The serialized message data will be set here
 *  @return Returns an error code that will be RCLUC_RET_OK if the message is serialized successfully
 */
typedef rcluc_ret_t (*rcluc_message_serialization_func_t)(void * message, void * serializedBuffer);

/**
 *  @brief Contains the metadata about a ROS Message required for the rcluc library to operate on it
 *  Contains metadata and functions required by the rcluc library to work with ROS Messages from the generator.
 *  The rmwu generator should provide a getter function to retrieve this information for every compiled message type.
 *
 *  @var rcluc_message_type_support_t::message_size
 *      The size of the ROS Messae in bytes.
 *  @var rcluc_message_type_support_t::serialize
 *      A function used to serialize the ROS message into the format required by the RMWU layer
 *  @var rcluc_message_type_support_t::deserialize
 *      A function used to deserialize the ROS message into the format required by the RMWU layer
 */
typedef struct {
    size_t message_size;
    rcluc_message_serialization_func_t serialize;
    rcluc_message_deserialization_func_t deserialize;
} rcluc_message_type_support_t;

/**
 *  @brief the construct for a callback that is invoked when an exception occurs for a subscription.
 *
 *  @param subscription A handle to the subscription that this exception occurred for
 *  @param error The error code for the type of error that occured
 */
typedef void (*rcluc_subscription_exception_callback_t)(const rcluc_subscription_handle_t subscription, rcluc_ret_t error);

/**
 *  @brief The Quality of Service policy for a ROS Topic Subscription
 *  This struct defines the settings for the Topic Subscription quality of service settings.
 *
 *  @var rcluc_subscription_qos_policy_t::reliability
 *      The reliability option for the Topic. The default is BEST_EFFORT
 */
typedef struct {
    rcluc_topic_reliability_t reliability;
} rcluc_subscription_qos_policy_t;

/**
 *  @struct rcluc_subscription_config_t
 *  @brief The configuration information for a ROS subscription
 *  Contains all the configuration required for the ROS Topic subscription
 *
 *  @var rcluc_subscription_config_t::deserialize_func
 *      The function used for deserializing messages that come in on this Topic subscription. If
 *      set to NULL then it will not attempt to deserialize the message. Additionally, support for deserialization
 *      must be configured at compile time using the configRCLUC_SUBSCRIPTION_DESERIALIZATION_SUPPORT configuration.
 *  @var rcluc_subscription_config_t::qos
 *      The quality of service policy for the subscription
 *  @var rcluc_subscription_config_t::exception_callback
 *      A callback that is invoked if an exception occurs for this subscription. If NULL then no callback will be invoked
 *      when exceptions occur inside the library.
 *  @var rcluc_subscription_config_t::user_metadata
 *      A pointer to user supplied metadata that they want associated with the subscription. You can use the subscription
 *      handle to access this data from the callbacks. It is up to the user to ensure that the data at this pointer
 *      remains valid for the lifetime of the subscription.
 */
typedef struct {
    rcluc_message_deserialization_func_t deserialize_func;
    rcluc_subscription_qos_policy_t qos;
    rcluc_subscription_exception_callback_t exception_callback;
    void * user_metadata;
} rcluc_subscription_config_t;

/**
 *  @brief The construct for a subscription callback function
 *  This is the function type for a subscription callback. It will be invoked by a task handling the processing for a
 *  node. This means that the execution of this functions blocks additional processing in the Node handler task from
 *  happening.
 *
 *  @param subscription A handle to the subscription that the message the callback is being invoked for was received on.
 *  @param message A reference to the message received on the topic. This will either be the raw data received or the
 *      deserialized data if the subscription is configured to deserialize data.
 *  @param args A reference to the user provided data that was given when the subscription was created
 */
typedef void (*rcluc_subscription_callback_t)(const rcluc_subscription_handle_t subscription, const void * message,
    const void * args);

/**
 *  @brief the construct for a callback that is invoked when an exception occurs for a publisher.
 *
 *  @param publisher A handle to the publisher that this exception occurred for
 *  @param error The error code for the type of error that occured
 */
typedef void (*rcluc_publisher_exception_callback_t)(const rcluc_publisher_handle_t publisher, rcluc_ret_t error);

/**
 *  @brief The Quality of Service policy for a ROS Topic Publisher
 *  This struct defines the settings for the Topic Publisher quality of service settings.
 *
 *  @var rcluc_publisher_qos_policy_t::reliability
 *      The reliability option for the Topic. The default is BEST_EFFORT
 */
typedef struct {
    rcluc_topic_reliability_t reliability;
} rcluc_publisher_qos_policy_t;

/**
 *  @struct rcluc_publisher_config_t
 *  @brief The configuration information for a ROS Topic publisher
 *  Contains all the configuration required for the ROS Topic publisher
 *
 *  @var rcluc_publisher_config_t::serialize_func
 *      The function used for deserializing messages that come in on this Topic subscription. If
 *      set to NULL then it will not attempt to deserialize the message. Additionally, support for deserialization
 *      must be configured at compile time using the configRCLUC_SUBSCRIPTION_DESERIALIZATION_SUPPORT configuration
 *      option.
 *  @var rcluc_publisher_config_t::qos
 *      The quality of service policy to use for the publisher
 *  @var rcluc_publisher_config_t::exception_callback
 *      A callback that is invoked if an exception occurs for this publisher. If NULL then no callback will be invoked
 *      when exceptions occur inside the library.
 *  @var rcluc_publisher_config_t::user_metadata
 *      A pointer to user supplied metadata that they want associated with the publisher. You can use the publisher
 *      handle to access this data from the callbacks. It is up to the user to ensure that the data at this pointer
 *      remains valid for the lifetime of the publisher.
 */
typedef struct {
    rcluc_message_serialization_func_t serialize_func;
    rcluc_publisher_qos_policy_t qos;
    rcluc_publisher_exception_callback_t exception_callback;
    void * user_metadata;
} rcluc_publisher_config_t;

#define RCLUC_SUBSCRIPTION_DESERIALIZATION_DISABLED 0
#define RCLUC_SUBSCRIPTION_DESERIALIZATION_STATIC_ALLOCATION 1
#define RCLUC_SUBSCRIPTION_DESERIALIZATION_STACK_ALLOCATION 2

#endif
