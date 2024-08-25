//
// Created by gbian on 25/08/2024.
//

#pragma once

#include "headers.hpp"

namespace lve {
    class Pipeline {
    public:
        Pipeline(const std::string& vertFilepath, const std::string& fragFilepath);

    private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
    };
}  // namespace lve