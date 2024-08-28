// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-avoid-magic-numbers,*-magic-numbers, *-uppercase-literal-suffix,*-uppercase-literal-suffix,  *-pro-type-union-access)
// clang-format on
#include "vkl/app.hpp"
#include <vkl/FPSCounter.hpp>

namespace lve {

    App::App() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    App::~App() { vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr); }

    void App::run() {
        FPSCounter fpsCounter{lveWindow.getGLFWWindow(), WTITILE};
        while(!lveWindow.shouldClose()) {
            fpsCounter.frameInTitle();
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(lveDevice.device());
    }

    void App::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        VK_CHECK(vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout), "failed to create pipeline layout!");
    }

    void App::createPipeline() {
        auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(lveSwapChain.width(), lveSwapChain.height());
        pipelineConfig.renderPass = lveSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        lvePipeline = MAKE_UNIQUE(
            Pipeline, lveDevice, Window::calculateRelativePathToSrcShaders(curentP, "simple_shader.vert.opt.rmp.spv").string(),
            Window::calculateRelativePathToSrcShaders(curentP, "simple_shader.frag.opt.rmp.spv").string(), pipelineConfig);
    }

    void App::createCommandBuffers() {
        commandBuffers.resize(lveSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = lveDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        VK_CHECK(vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()), "failed to allocate command buffers!");

        for(std::size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = lveSwapChain.getRenderPass();
            renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(C_I(i));

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = VkClearColorValue{.float32{0.1f, 0.1f, 0.1f, 1.0f}};
            clearValues[1].depthStencil = {.depth = 1.0f, .stencil = 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            lvePipeline->bind(commandBuffers[i]);
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) { throw std::runtime_error("failed to record command buffer!"); }
        }
    }
    void App::drawFrame() {
        uint32_t imageIndex;  // NOLINT(*-init-variables)
        auto result = lveSwapChain.acquireNextImage(&imageIndex);
        if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) { throw std::runtime_error("failed to acquire swap chain image!"); }

        result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if(result != VK_SUCCESS) { throw std::runtime_error("failed to present swap chain image!"); }
    }

}  // namespace lve

// clang-format off
// NOLINTEND(*-include-cleaner, *-avoid-magic-numbers,*-magic-numbers, *-uppercase-literal-suffix,*-uppercase-literal-suffix,  *-pro-type-union-access)
// clang-format on