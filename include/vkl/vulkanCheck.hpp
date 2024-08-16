//
// Created by gbian on 17/07/2024.
//

#pragma once
// NOLINTBEGIN(*-avoid-do-while)
#include "headers.hpp"

// vulkan headers
#define VULKAN_HPP_NO_EXCEPTIONS
#include <vulkan/vulkan.hpp>

#define VK_CHECK(f, throwable)                                                                                                             \
    do {                                                                                                                                   \
        const vk::Result res = (f);                                                                                                        \
        if(res != vk::Result::eSuccess) [[unlikely]] {                                                                                     \
            const auto loc = std::source_location::current();                                                                              \
            const std::string detailed_message = FORMAT("VkResult is \"{}\" from {} in {} at line {} in function {}", vk::to_string(res),  \
                                                        #f, loc.file_name(), loc.line(), loc.function_name());                             \
            LCRITICAL(detailed_message);                                                                                                   \
            throw std::runtime_error(FORMAT("{}: {}", throwable, detailed_message));                                                       \
        }                                                                                                                                  \
    } while(0)

#define VK_CHECK_SYNC_OBJECTS(f, f2, f3, throwable)                                                                                        \
    do {                                                                                                                                   \
        const vk::Result res = (f);                                                                                                        \
        const vk::Result res2 = (f2);                                                                                                      \
        const vk::Result res3 = (f3);                                                                                                      \
        if(res != vk::Result::eSuccess || res2 != vk::Result::eSuccess || res3 != vk::Result::eSuccess) [[unlikely]] {                     \
            const auto loc = std::source_location::current();                                                                              \
            const std::string detailed_message = FORMAT(                                                                                   \
                "VkResult is \"{}\",\"{}\",\"{}\" from {}, {}, {} in {} at line {} in function {}", vk::to_string(res),                    \
                vk::to_string(res2), vk::to_string(res3), #f, #f2, #f3, loc.file_name(), loc.line(), loc.function_name());                 \
            LCRITICAL(detailed_message);                                                                                                   \
            throw std::runtime_error(FORMAT("{}: {}", throwable, detailed_message));                                                       \
        }                                                                                                                                  \
    } while(0)

#define VK_CHECK_SWAPCHAIN(f, trowable)                                                                                                    \
    do {                                                                                                                                   \
        const vk::Result res = (f);                                                                                                        \
        if(res != vk::Result::eSuccess && res != vk::Result::eSuboptimalKHR) [[unlikely]] {                                                \
            constexpr auto loc = std::source_location::current();                                                                          \
            LCRITICAL("Fatal : VkResult is \"{0}\" from{1} in {2} at line {3}", #f, vk::to_string(res), loc.file_name(), loc.line());      \
            throw std::runtime_error(trowable);                                                                                            \
        }                                                                                                                                  \
    } while(0)
// NOLINTEND(*-avoid-do-while)