#pragma once
// NOLINTBEGIN(*-include-cleaner)
#include "Window.hpp"
#include "Pipeline.hpp"
#include "headers.hpp"
#include "vulkanCheck.hpp"

//#include <vkl/VlukanLogInfoCallback.hpp>

/*template <typename T, auto N> auto tryConvertFormat(const vk::ArrayWrapper1D<T, N> &var) -> std::string {
    if constexpr(std::is_same_v<T, char>) {
        return var.data();
    } else {
        return FORMAT("[{}]", FMT_JOIN(var, ", "));
    }
}*/

// NOLINTBEGIN(*-lambda-function-name)
inline void print_extensions(const std::vector<VkExtensionProperties> &extensions) {
    const vnd::AutoTimer timer("print_extensions");
    std::ranges::for_each(extensions, [](const auto &extension) {
        LINFO("  \'{} (v. {})\'", extension.extensionName, extension.specVersion);
    });
}
inline std::string report_version_numberstr(uint32_t version) {
    return FORMAT("Variant: {}, Major: {}, Minor: {}, Patch: {}", VK_API_VERSION_VARIANT(version), VK_API_VERSION_MAJOR(version),
                  VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version));
}

inline std::string report_version_numberstrcomp(uint32_t version) {
    return FORMAT("v{}.{}.{}", VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version));
}

inline void print_layers(std::vector<VkLayerProperties> &layers) {
    std::ranges::for_each(layers, [](const auto &layer) {
        LINFO("Layer Name:       '{}'", layer.layerName);
        LINFO("Description:      {}", layer.description);
        LINFO("Spec Version:     {}", report_version_numberstrcomp(layer.specVersion));
        LINFO("Implementation Version: {}", layer.implementationVersion);
        LINFO("---------------------------------------------------");
    });
}

// NOLINTEND(*-lambda-function-name)

namespace lve {

    class App {
    public:
        App() {
            //LINFO("{}",Window::calculateRelativePathToSrcShaders(curentP,"simple_shader.vert.opt.rmp.spv").string());
            //LINFO("{}",Window::calculateRelativePathToSrcShaders(curentP,"simple_shader.frag.opt.rmp.spv").string());
        };
        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();

    private:
        lve::Window window{WWIDTH, WHEIGHT, WTITILE};
        lve::Pipeline pipeline{Window::calculateRelativePathToSrcShaders(curentP, "simple_shader.vert.opt.rmp.spv").string(),
                               Window::calculateRelativePathToSrcShaders(curentP, "simple_shader.frag.opt.rmp.spv").string()};
        //lve::Pipeline pipeline{"../../../shaders/simple_shader.vert.opt.rmp.spv", "../../../shaders/simple_shader.frag.opt.rmp.spv"};
    };
}  // namespace lve

// NOLINTEND(*-include-cleaner)