// clang-format off
// NOLINTBEGIN(*-include-cleaner, *-use-anonymous-namespace, *-easily-swappable-parameters, *-implicit-bool-conversion, *-init-variables)
// clang-format on
#include <vkl/vkl.hpp>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `vkl`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>

// NOLINTBEGIN(*-owning-memory)
// NOLINTNEXTLINE(bugprone-exception-escape)
int main() {
    INIT_LOG()
    LINFO("{} {}v", vkl::cmake::project_name, vkl::cmake::project_version);
    LINFO("{}", glfwGetVersionString());
    try {
        lve::App app{};

        app.run();
    } catch(const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
}

// NOLINTEND(*-owning-memory)
// // clang-format off
// NOLINTEND(*-include-cleaner, *-use-anonymous-namespace, *-easily-swappable-parameters, *-implicit-bool-conversion, *-init-variables)
// clang-format on