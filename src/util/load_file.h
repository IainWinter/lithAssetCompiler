#pragma once

#include <vector>
#include <string>
#include <fstream>

inline FixedByteArray loadFile(const std::string& filepath, std::string& err) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        err = "Failed to load filepath";
        return {};
    }
    
    std::streampos size;
    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    FixedByteArray bytes = FixedByteArray(size);
    file.read(bytes.data(), size);

    err = "";
    return bytes;
}