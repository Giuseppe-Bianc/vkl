//
// Created by gbian on 16/08/2024.
//

#pragma once

#include "../vulkanCheck.hpp"

/**
* @brief Create a Vulkan instance.
*
* @param applicationName the name of the application.
* @param deletionQueue Queue onto which to push the instance's destructor.
*
* @return the instance created.
*/
vk::Instance make_instance(const char* applicationName, std::deque<std::function<void()>>& deletionQueue);