//
// Created by gbian on 16/08/2024.
//

// NOLINTBEGIN(*-include-cleaner)
#include "vkl/renderer/instance.hpp"
#include <GLFW/glfw3.h>

vk::Instance make_instance(const char *applicationName, std::deque<std::function<void()>> &deletionQueue) {
    LINFO("Making an instance...");

    uint32_t version = vk::enumerateInstanceVersion().value;

    LINFO("System can support vulkan Variant: {}, Major: {}, Minor: {}, Patch: {}", vk::apiVersionVariant(version),
          vk::apiVersionMajor(version), vk::apiVersionMinor(version), vk::apiVersionPatch(version));

    // set the patch to 0 for best compatibility/stability)
    version &= ~(0xFFFU); // NOLINT(*-avoid-magic-numbers,*-magic-numbers)

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
     * Everything with Vulkan is "opt-in", so we need to query which extensions glfw needs
     * in order to interface with vulkan.
     */
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    LINFO("extensions to be requested:");
    for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
        LINFO("\t\"{}\"", glfwExtensions[i]); // NOLINT(*-pro-bounds-pointer-arithmetic)
    }

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
    vk::InstanceCreateInfo const createInfo = vk::InstanceCreateInfo({}, &appInfo, 0, nullptr, glfwExtensionCount, glfwExtensions);
    vk::ResultValue<vk::Instance> const instanceAttempt = vk::createInstance(createInfo);
    if(instanceAttempt.result != vk::Result::eSuccess) {
        LINFO("Failed to create Instance!");
        return nullptr;
    }

    vk::Instance instance = instanceAttempt.value;
    VkInstance handle = instance;

    deletionQueue.emplace_back([handle]() {
        vkDestroyInstance(handle, nullptr);
        LINFO("Deleted Instance!"); // NOLINT(*-lambda-function-name)
    });

    return instance;
}

// NOLINTEND(*-include-cleaner)