#pragma once
#include <memory_resource>
#include <vector>
#include <algorithm>
#include <iostream>

struct Chunk {
    void* ptr;
    size_t size;
};

class MemoryRes : public std::pmr::memory_resource {
private:
    char* buffer;
    size_t buffer_size;
    std::vector<Chunk> used_blocks;

public:
    MemoryRes(size_t size){
        buffer_size = size;
        buffer = new char[buffer_size];
    }

    ~MemoryRes(){
        delete[] buffer;
    }

protected:
    void* do_allocate(size_t bytes, size_t alignment) override {
        std::sort(used_blocks.begin(), used_blocks.end(),
            [](const Chunk& a, const Chunk& b){
                return a.ptr < b.ptr;
            });
        char* cur_ptr = buffer;

        for (const auto& block : used_blocks){
            char* block_start = (char*)block.ptr;
            size_t free_space = block_start - cur_ptr;

            if (free_space >= bytes){
                used_blocks.push_back({cur_ptr, bytes});
                return cur_ptr;
            }

            cur_ptr = block_start + block.size;
        }

        char* buffer_end = buffer + buffer_size;
        if ((buffer_end - cur_ptr) >= bytes){
            used_blocks.push_back({cur_ptr, bytes});
            return cur_ptr;
        }

        throw std::bad_alloc();
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override {
        for (size_t i = 0; i < used_blocks.size(); ++i){
            if (used_blocks[i].ptr == p){
                used_blocks.erase(used_blocks.begin() + i);
                return;
            }
        }
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        return this == &other;
    }
};