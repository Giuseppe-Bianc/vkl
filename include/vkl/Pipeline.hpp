//
// Created by gbian on 25/08/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "Device.hpp"
#include "headers.hpp"
#include "vulkanCheck.hpp"

namespace lve {

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline {
    public:
        Pipeline(Device &device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);
        ~Pipeline();

        Pipeline(const Pipeline &) = delete;
        Pipeline& operator=(const Pipeline &) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile(const std::string &filepath);

        void createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo);

        void createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule);

        Device &lveDevice;
        VkPipeline graphicsPipeline{};
        VkShaderModule vertShaderModule{};
        VkShaderModule fragShaderModule{};
    };
}  // namespace lve
   // NOLINTEND(*-include-cleaner)