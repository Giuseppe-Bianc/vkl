//
// Created by gbian on 25/08/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "vkl/Pipeline.hpp"

#include <vkl/timer/Timer.hpp>

#define INDEPTH

namespace lve {
    Pipeline::Pipeline(const std::string &vertFilepath, const std::string &fragFilepath) {
        createGraphicsPipeline(vertFilepath, fragFilepath);
    }

    std::vector<char> Pipeline::readFile(const std::string &filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};  // NOLINT(*-signed-bitwise)
#ifdef INDEPTH
        const vnd::AutoTimer timer(FORMAT("reading shader {}", filepath));
#endif
        if(!file.is_open()) [[unlikely]] { throw std::runtime_error(FORMAT("failed to open file: {}", filepath)); }

        // Determine file size using std::ifstream::seekg and tellg
        const auto fileSize = C_ST(file.tellg());
        // Allocate buffer and read file contents
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), C_LL(fileSize));

        // Check for read errors
        if(!file) [[unlikely]] { throw std::runtime_error(FORMAT("failed to read file: {}", filepath)); }

        // Close the file (automatically done by ifstream destructor, but good practice to explicitly close)
        file.close();

        return buffer;
    }

    void Pipeline::createGraphicsPipeline(const std::string &vertFilepath, const std::string &fragFilepath) {
        const auto vertCode = readFile(vertFilepath);
        const auto fragCode = readFile(fragFilepath);

#ifdef INDEPTH
        LINFO("Vertex Shader Code Size: {}", vertCode.size());
        LINFO("Fragment Shader Code Size: {}", fragCode.size());
#endif
    }

}  // namespace lve
// NOLINTEND(*-include-cleaner)