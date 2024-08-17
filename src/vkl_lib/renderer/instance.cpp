//
// Created by gbian on 16/08/2024.
//
// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-init-variables, *-owning-memory, *-pro-type-cstyle-cast, *-no-malloc, *-narrowing-conversions,*-narrowing-conversions)
// clang-format on
#include "vkl/renderer/instance.hpp"
#include "vkl/logging/logger.hpp"
#include <GLFW/glfw3.h>

bool supported_by_instance(const char **extensionNames, int extensionCount, const char **layerNames, int layerCount) {
    // Logger *logger = Logger::get_logger();

    // check extension support
    std::vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties().value;

    LINFO("Instance can support the following extensions:");
    print_extensions(supportedExtensions);

    bool found;
    for(int i = 0; i < extensionCount; ++i) {
        const char *extension = extensionNames[i];
        found = false;
        for(vk::ExtensionProperties supportedExtension : supportedExtensions) {
            if(strcmp(extension, supportedExtension.extensionName) == 0) {
                found = true;
                LINFO("Extension \"{}\" is supported!", extension);
                break;
            }
        }
        if(!found) {
            LINFO("Extension \"{}\" is not supported!", extension);
            return false;
        }
    }

    // check layer support
    std::vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties().value;

    LINFO("Instance can support the following layers:");
    print_layers(supportedLayers);

    for(int i = 0; i < layerCount; ++i) {
        const char *layer = layerNames[i];
        found = false;
        for(vk::LayerProperties supportedLayer : supportedLayers) {
            if(strcmp(layer, supportedLayer.layerName) == 0) {
                found = true;
                LINFO("Layer \"{}\" is supported!", layer);
                break;
            }
        }
        if(!found) {
            LINFO("Layer \"{}\" is not supported!", layer);
            return false;
        }
    }

    return true;
}

vk::Instance make_instance(const char *applicationName, std::deque<std::function<void(vk::Instance)>> &deletionQueue) {
    Logger *logger = Logger::get_logger();

    LINFO("Making an instance...");
//#define USE_PREFERRED_VERSION

#ifdef USE_PREFERRED_VERSION
    auto preferred_version = VK_MAKE_VERSION(1, 3, 283);
#endif

    uint32_t version = vk::enumerateInstanceVersion().value;
#ifdef USE_PREFERRED_VERSION
    if(version <= preferred_version) {
        version = preferred_version;
        LINFO("Using preferred version - {}", report_version_numberstr(preferred_version));
    } else {
        LINFO("Preferred version not available. Using available system version - {}", report_version_numberstr(version));
    }
#else
    report_version_number(version);
#endif

    // set the patch to 0 for best compatibility/stability)
    version &= ~(0xFFFU);  // NOLINT(*-avoid-magic-numbers,*-magic-numbers)

    /*
    * from vulkan_structs.hpp:
    *
    * VULKAN_HPP_CONSTEXPR ApplicationInfo( const char * pApplicationName_   = {},
                                      uint32_t     applicationVersion_ = {},
                                      const char * pEngineName_        = {},
                                      uint32_t     engineVersion_      = {},
                                      uint32_t     apiVersion_         = {} )
    */
    vk::ApplicationInfo const appInfo = vk::ApplicationInfo(applicationName, VK_MAKE_VERSION(1, 0, 0), "No Engine",
                                                            VK_MAKE_VERSION(1, 0, 0), version);

    /*
     * Extensions
     */
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    uint32_t enabledExtensionCount = glfwExtensionCount;
    if(logger->is_enabled()) { enabledExtensionCount++; }
    const char **ppEnabledExtensionNames = (const char **)malloc(enabledExtensionCount * sizeof(char *));

    for(uint32_t i = 0; i < glfwExtensionCount; ++i) { ppEnabledExtensionNames[i] = glfwExtensions[i]; }
    if(logger->is_enabled()) {
        ppEnabledExtensionNames[glfwExtensionCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    }

    LINFO("extensions to be requested:");
    print_list(ppEnabledExtensionNames, enabledExtensionCount);

    /*
     * Layers
     */
    uint32_t enabledLayerCount = 0;
    if(logger->is_enabled()) { enabledLayerCount++; }
    const char **ppEnabledLayerNames = nullptr;
    if(enabledLayerCount > 0) { ppEnabledLayerNames = (const char **)malloc(enabledLayerCount * sizeof(char *)); }

    if(logger->is_enabled()) {
        // TODO: request validation layer
        ppEnabledLayerNames[0] = "VK_LAYER_KHRONOS_validation";
    }

    LINFO("layers to be requested:");
    print_list(ppEnabledLayerNames, enabledLayerCount);

    if(!supported_by_instance(ppEnabledExtensionNames, enabledExtensionCount, ppEnabledLayerNames, enabledLayerCount)) { return nullptr; }

    /*
    *
    * from vulkan_structs.hpp:
    *
    * InstanceCreateInfo( VULKAN_HPP_NAMESPACE::InstanceCreateFlags     flags_                 = {},
                                         const VULKAN_HPP_NAMESPACE::ApplicationInfo * pApplicationInfo_      = {},
                                         uint32_t                                      enabledLayerCount_     = {},
                                         const char * const *                          ppEnabledLayerNames_   = {},
                                         uint32_t                                      enabledExtensionCount_ = {},
                                         const char * const * ppEnabledExtensionNames_ = {} )
    */
    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(vk::InstanceCreateFlags(), &appInfo, enabledLayerCount, ppEnabledLayerNames,
                                                               enabledExtensionCount, ppEnabledExtensionNames);

    vk::ResultValue<vk::Instance> instanceAttempt = vk::createInstance(createInfo);
    if(instanceAttempt.result != vk::Result::eSuccess) {
        LINFO("Failed to create Instance!");
        return nullptr;
    }

    vk::Instance instance = instanceAttempt.value;

    deletionQueue.emplace_back([](vk::Instance instance) {
        instance.destroy();
        LINFO("Deleted Instance!");  // NOLINT(*-lambda-function-name)
    });

    // NOLINTBEGIN(*-multi-level-implicit-pointer-conversion)
    free(ppEnabledExtensionNames);
    if(ppEnabledLayerNames) { free(ppEnabledLayerNames); }
    // NOLINTEND(*-multi-level-implicit-pointer-conversion)
    return instance;
}
// clang-format off
// NOLINTEND(*-include-cleaner, *-init-variables, *-owning-memory, *-pro-type-cstyle-cast, *-no-malloc, *-narrowing-conversions,*-narrowing-conversions)
// clang-format on