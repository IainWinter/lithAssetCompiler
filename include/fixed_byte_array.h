#pragma once

#include <stdlib.h>
#include <string.h>

// Instead of using a vector, use this
// owns data
class FixedByteArray {
public:
    FixedByteArray() 
        : m_data (nullptr)
        , m_size (0ull)
    {}

    FixedByteArray(size_t size) {
        m_data = (char*)malloc(size);
        m_size = size;
    }

    FixedByteArray(const char* data, size_t size) {
        m_data = (char*)malloc(size);
        m_size = size;

        memcpy(m_data, data, size);
    }

    ~FixedByteArray() {
        free(m_data);
    }

    FixedByteArray(const FixedByteArray& other) {
        m_data = (char*)malloc(other.m_size);
        m_size = other.m_size;
        memcpy(m_data, other.m_data, m_size);
    }

    FixedByteArray(FixedByteArray&& other) {
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
    }

    FixedByteArray& operator=(const FixedByteArray& other) {
        m_data = (char*)malloc(other.m_size);
        m_size = other.m_size;
        memcpy(m_data, other.m_data, m_size);
        return *this;
    }

    FixedByteArray& operator=(FixedByteArray&& other) {
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
        return *this;
    }

    // iterator interface

    void resize(size_t size) {
        m_data = (char*)realloc(m_data, size);
        m_size = size;
    }

    size_t size() const { return m_size; }
    char* data() { return m_data; }
    const char* data() const { return m_data; }
    char* begin() { return m_data; }
    char* end() { return m_data + m_size; }
    const char* begin() const { return m_data; }
    const char* end() const { return m_data + m_size; }

private:
    char* m_data;
    size_t m_size;
};