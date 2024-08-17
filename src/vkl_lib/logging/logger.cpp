// NOLINTBEGIN(*-include-cleaner, *-make-member-function-const)
#include "vkl/logging/logger.hpp"
#include "vkl/VlukanLogInfoCallback.hpp"
Logger* Logger::logger; // NOLINT(*-avoid-non-const-global-variables)

// NOLINTBEGIN(*-diagnostic-unused-parameter,*-unused-parameters)
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
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
// NOLINTEND(*-diagnostic-unused-parameter, *-unused-parameters)

void Logger::set_mode(bool mode) { enabled = mode; }

bool Logger::is_enabled() { return enabled; }

Logger *Logger::get_logger() {
    if(!logger) { logger = new Logger(); }  // NOLINT(*-owning-memory)

    return logger;
}

std::string report_version_numberstr(uint32_t version) {
    return FORMAT("Variant: {}, Major: {}, Minor: {}, Patch: {}", vk::apiVersionVariant(version),
                  vk::apiVersionMajor(version), vk::apiVersionMinor(version), vk::apiVersionPatch(version));
}
void report_version_number(uint32_t version) {

    LINFO("System can support vulkan Variant: {}, Major: {}, Minor: {}, Patch: {}", vk::apiVersionVariant(version),
          vk::apiVersionMajor(version), vk::apiVersionMinor(version), vk::apiVersionPatch(version));
}

void print_list(const char **list, uint32_t count) {

    for(uint32_t i = 0; i < count; ++i) { LINFO("\t\"{}\"", list[i]); }
}

void print_extensions(std::vector<vk::ExtensionProperties> &extensions) {

    for(vk::ExtensionProperties extension : extensions) { LINFO("\t \'{}\'", std::string(extension.extensionName.data())); }
}

void print_layers(std::vector<vk::LayerProperties> &layers) {

    for(vk::LayerProperties layer : layers) { LINFO("\t \'{}\'", std::string(layer.layerName.data())); }
}

// NOLINTNEXTLINE(*-diagnostic-unused-parameter,*-unused-parameters)
vk::DebugUtilsMessengerEXT make_debug_messenger([[maybe_unused]]vk::Instance &instance, vk::DispatchLoaderDynamic &dldi,
                                                        std::deque<std::function<void(vk::Instance)>> &deletionQueue) {

    /*
    * DebugUtilsMessengerCreateInfoEXT( VULKAN_HPP_NAMESPACE::DebugUtilsMessengerCreateFlagsEXT flags_           = {},
                                    VULKAN_HPP_NAMESPACE::DebugUtilsMessageSeverityFlagsEXT messageSeverity_ = {},
                                    VULKAN_HPP_NAMESPACE::DebugUtilsMessageTypeFlagsEXT     messageType_     = {},
                                    PFN_vkDebugUtilsMessengerCallbackEXT                    pfnUserCallback_ = {},
                                    void * pUserData_ = {} )
    */
    // TODO: describe the messenger
    vk::DebugUtilsMessengerCreateInfoEXT createInfo; // NOLINT(*-diagnostic-unused-variable)

    // TODO:create the messenger
    vk::DebugUtilsMessengerEXT messenger;
    VkDebugUtilsMessengerEXT handle = messenger;
    // NOLINTNEXTLINE(*-diagnostic-unused-parameter, *-diagnostic-unused-lambda-capture)
    deletionQueue.emplace_back([handle, dldi]([[maybe_unused]] vk::Instance instance) {
        // TODO:destroy the messenger
            LINFO("Destroying debug messenger...");  // NOLINT(bugprone-lambda-function-name)

    });

    return messenger;
}

// NOLINTEND(*-include-cleaner, *-make-member-function-const)