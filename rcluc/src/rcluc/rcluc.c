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
 *  @brief Implementation of the RCLUC library
 */

#include "rcluc/rcluc.h"
#include "rcluc/rmwu.h"
#include "rcluc/rcluc_types.h"
#include "rcluc/rmwu_types.h"
#include <string.h>

struct rcluc_subscription_s {
    uint8_t is_used;
    rmwu_subscription_t rmwu_subscription;
    void * user_metadata;
};

struct rcluc_publisher_s {
    uint8_t is_used;
    rmwu_publisher_t rmwu_publisher;
    void * user_metadata;
};

struct rcluc_node_s {
    uint8_t is_used;
    rmwu_node_t rmwu_node;
    struct rcluc_subscription_s subscriptions[configRCLUC_MAX_SUBSCRIPTIONS_PER_NODE];
    struct rcluc_publisher_s publishers[configRCLUC_MAX_PUBLISHERS_PER_NODE];
};

static struct rcluc_node_s nodes[configRCLUC_MAX_NUM_NODES] = {0};

rcluc_ret_t rcluc_init(const rcluc_client_config_t * config) {
    return rmwu_init(config);
}

rcluc_ret_t rcluc_node_create(const char * name, const char * namespace_, rcluc_node_handle_t * node_handle) {
    rcluc_node_handle_t new_node = NULL;
    rcluc_ret_t status = RCLUC_RET_OK;

    // Check inputs to make sure they're not null
    if (NULL == name || NULL == namespace_ || NULL == node_handle) {
        return RCLUC_RET_NULL_PTR;
    }

    // Find the next free node on the list
    status = RCLUC_RET_ERR_SPACE;
    for (size_t i = 0; i < configRCLUC_MAX_NUM_NODES; ++i) {
        if (0 == nodes[i].is_used) {
            nodes[i].is_used = 1;
            new_node = &nodes[i];
        }
    }

    if (NULL != new_node) {
        status = rmwu_node_create(name, namespace_, &(new_node->rmwu_node));

        // If the node was created successfully then set the return value, otherwise mark the node as free again.
        if (RCLUC_RET_OK == status) {
            *node_handle = new_node;
        } else {
            new_node->is_used = 0;
        }
    }

    return status;
}

rcluc_ret_t rcluc_node_destroy(rcluc_node_handle_t node_handle) {
    rcluc_ret_t status = RCLUC_RET_ERROR;

    if (NULL == node_handle) {
        return RCLUC_RET_NULL_PTR;
    } else if (0 == node_handle->is_used) {
        return RCLUC_RET_ERR_ALREADY;
    }

    status = rmwu_node_destroy(&(node_handle->rmwu_node));

    // Only mark it as free if successfully desetroyed so that we can try again if not
    if (RCLUC_RET_OK == status) {
        node_handle->is_used = 0;
    }

    return status;
}

void rcluc_node_spin_once(rcluc_node_handle_t node_handle) {
    if (NULL == node_handle) {
        return;
    }
    //TODO

}

void rcluc_node_spin_forever(rcluc_node_handle_t node_handle) {
    if (NULL == node_handle) {
        return;
    }
    //TODO

}

rcluc_ret_t rcluc_subscription_create(rcluc_node_handle_t node_handle, const rcluc_message_type_support_t * message_type,
        const char * topic_name, rcluc_subscription_callback_t callback, const size_t queue_length, uint8_t *message_buffer,
        const rcluc_subscription_config_t * config, rcluc_subscription_handle_t * subscription_handle) {

    rcluc_ret_t status = RCLUC_RET_OK;
    rcluc_subscription_handle_t new_subscription = NULL;
    if (NULL == node_handle || NULL == message_type || NULL == topic_name || NULL == callback || NULL == message_buffer
            || NULL == config || NULL == subscription_handle) {
        return RCLUC_RET_NULL_PTR;
    }

    status = RCLUC_RET_ERR_SPACE;
    for (size_t i = 0; i < configRCLUC_MAX_SUBSCRIPTIONS_PER_NODE; ++i) {
        if (0 == node_handle->subscriptions[i].is_used) {
            node_handle->subscriptions[i].is_used = 1;
            new_subscription = &node_handle->subscriptions[i];
        }
    }

    if (NULL != new_subscription) {
        status = rmwu_subscription_create(&(node_handle->rmwu_node), message_type, topic_name, callback, queue_length,
                message_buffer, config, &new_subscription->rmwu_subscription);

        // If the subscription was created successfully then set the return value, otherwise mark it as free again.
        if (RCLUC_RET_OK == status) {
            new_subscription->user_metadata = config->user_metadata;
            *subscription_handle = new_subscription;
        } else {
            new_subscription->is_used = 0;
        }
    }

    return status;
}

void rcluc_subscription_get_default_config(rcluc_subscription_config_t * config) {
    if (NULL != config) {
        memset(config, 0, sizeof(rcluc_subscription_config_t));
        config->qos.reliability = RCLUC_TOPIC_RELIABILITY_BEST_EFFORT;
        config->exception_callback = NULL;
        config->user_metadata = NULL;
    }
}

void * rcluc_subscription_get_user_metadata(const rcluc_subscription_handle_t subscription_handle) {
    void * metadata = NULL;
    if (NULL != subscription_handle) {
        metadata = subscription_handle->user_metadata;
    }
    return metadata;
}

rcluc_ret_t rcluc_subscription_destroy(rcluc_subscription_handle_t subscription_handle) {
    rcluc_ret_t status = RCLUC_RET_OK;
    if (NULL == subscription_handle) {
        return RCLUC_RET_NULL_PTR;
    }

    if (0 == subscription_handle->is_used) {
        status = RCLUC_RET_ERR_ALREADY;
    }

    if (RCLUC_RET_OK == status) {
        status = rmwu_subscription_destroy(&subscription_handle->rmwu_subscription);
    }

    if (RCLUC_RET_OK == status) {
        subscription_handle->is_used = 0;
    }

    return status;
}

rcluc_ret_t rcluc_publisher_create(rcluc_node_handle_t node_handle,
        const rcluc_message_type_support_t * message_type, size_t queue_length, uint8_t * message_buffer,
        const rcluc_publisher_config_t * config, rcluc_publisher_handle_t * publisher_handle) {
    rcluc_ret_t status = RCLUC_RET_OK;
    rcluc_publisher_handle_t new_publisher = NULL;
    if (NULL == node_handle || NULL == message_type || NULL == message_buffer || NULL == config || NULL == publisher_handle) {
        return RCLUC_RET_NULL_PTR;
    } else if (queue_length <= 0) {
        return RCLUC_RET_ERR_PARAM;
    }

    status = RCLUC_RET_ERR_SPACE;
    for (size_t i = 0; i < configRCLUC_MAX_PUBLISHERS_PER_NODE; ++i) {
        if (0 == node_handle->publishers[i].is_used) {
            node_handle->publishers[i].is_used = 1;
            new_publisher = &node_handle->publishers[i];
        }
    }

    if (NULL != new_publisher) {
        status = rmwu_publisher_create(&(node_handle->rmwu_node), message_type, queue_length, message_buffer, config,
                &new_publisher->rmwu_publisher);

        // If the publisher was created successfully then set the return value, otherwise mark it as free again.
        if (RCLUC_RET_OK == status) {
            new_publisher->user_metadata = config->user_metadata;
            *publisher_handle = new_publisher;
        } else {
            new_publisher->is_used = 0;
        }
    }

    return status;
}

void rcluc_publisher_get_default_config(rcluc_publisher_config_t * config) {
    if (NULL == config) {
        return;
    }
    config->qos.reliability = RCLUC_TOPIC_RELIABILITY_BEST_EFFORT;
    config->exception_callback = NULL;
    config->user_metadata = NULL;
}

void * rcluc_publisher_get_user_metadata(const rcluc_publisher_handle_t publisher_handle) {
    if (NULL == publisher_handle) {
        return NULL;
    }
    return publisher_handle->user_metadata;
}

rcluc_ret_t rcluc_publisher_destroy(rcluc_publisher_handle_t publisher_handle) {
    rcluc_ret_t status = RCLUC_RET_OK;
    if (NULL == publisher_handle) {
        return RCLUC_RET_NULL_PTR;
    }

    if (0 == publisher_handle->is_used) {
        status = RCLUC_RET_ERR_ALREADY;
    }

    if (RCLUC_RET_OK == status) {
        status = rmwu_publisher_destroy(&publisher_handle->rmwu_publisher);
    }

    if (RCLUC_RET_OK == status) {
        publisher_handle->is_used = 0;
    }

    return status;
}

rcluc_ret_t rcluc_publisher_publish(rcluc_publisher_handle_t publisher_handle, const void * message) {
    if (NULL == publisher_handle || NULL == message) {
        return RCLUC_RET_NULL_PTR;
    }
    return rmwu_publisher_publish(&publisher_handle->rmwu_publisher, message);
}
