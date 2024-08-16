//
// Created by gbian on 18/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "vulkanCheck.hpp"

DISABLE_WARNINGS_PUSH(26429 26481)

inline static void logQueueLabel(const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                 const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) {
    for(uint32_t i = 0; i < pCallbackData->queueLabelCount; ++i) {
        std::string_view labelName = pCallbackData->pQueueLabels[i].pLabelName ? pCallbackData->pQueueLabels[i].pLabelName : "Unknown";
        const auto msg = FORMAT("Queue Label [{}]: {}", i, labelName);

        switch(messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            LTRACE(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            LINFO(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LWARN(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LERROR(msg);
            break;
        default:
            LDEBUG(msg);
            break;
        }
    }
}

inline static void logCmdBuffers(const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                 const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) {
    for(uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; ++i) {
        std::string_view labelName = pCallbackData->pCmdBufLabels[i].pLabelName ? pCallbackData->pCmdBufLabels[i].pLabelName : "Unknown";
        const auto msg = FORMAT("Command Buffer Label [{}]: {}", i, labelName);

        switch(messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            LTRACE(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            LINFO(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LWARN(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LERROR(msg);
            break;
        default:
            LDEBUG(msg);
            break;
        }
    }
}

inline static void logObjects(const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                              const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) {
    for(uint32_t i = 0; i < pCallbackData->objectCount; ++i) {
        std::string_view objectName = pCallbackData->pObjects[i].pObjectName ? pCallbackData->pObjects[i].pObjectName : "Unknown";
        const auto msg = FORMAT("Object [{}]: Type: {} (Handle: {}) Name: {}", i,
                                string_VkObjectType(pCallbackData->pObjects[i].objectType), pCallbackData->pObjects[i].objectHandle,
                                objectName);

        switch(messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            LTRACE(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            LINFO(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            LWARN(msg);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            LERROR(msg);
            break;
        default:
            LDEBUG(msg);
            break;
        }
    }
}

inline void logDebugValidationLayerInfo(const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                        const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity) {
    // Log queue labels if available
    logQueueLabel(pCallbackData, messageSeverity);

    // Log command buffer labels if available
    logCmdBuffers(pCallbackData, messageSeverity);

    // Log objects if available
    logObjects(pCallbackData, messageSeverity);
}

[[nodiscard]] inline static constexpr std::string_view debugCallbackString(VkDebugUtilsMessageTypeFlagsEXT messageType) noexcept {
  switch(messageType) {
  case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
    return "[General] ";
  case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
    return "[Validation] ";
  case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
    return "[Performance] ";
  default:
    return "";
  }
}

inline static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                           VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                           const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                           [[maybe_unused]] void *pUserData) {
  // Determine the message type
  const std::string_view type = debugCallbackString(messageType);

  // Format and log the message
  const auto msg = FORMAT("{}Message ID: {}({}): {}", type, pCallbackData->pMessageIdName ? pCallbackData->pMessageIdName : "Unknown",
                          pCallbackData->messageIdNumber, pCallbackData->pMessage);

  switch(messageSeverity) {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    LTRACE(msg);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    LINFO(msg);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    LWARN(msg);
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    LERROR(msg);
    break;
  default:
    LDEBUG(msg);
    break;
  }

  logDebugValidationLayerInfo(pCallbackData, messageSeverity);

  return VK_FALSE;
}
DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
