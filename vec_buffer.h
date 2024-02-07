#pragma once

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <exception>


template <typename Type>
class VecBuffer{
public:
    VecBuffer() = delete;

    VecBuffer(uint32_t buf_size) {
        buf_.resize(buf_size);
    }

    void clearBuf() {
        buf_.clear();
        tail_ = 0;
        cur_size_ = 0;
    }

    uint32_t bufferSize() {
        return cur_size_;
    }

    void normalizeTail() {
        if(tail_ >= buf_.size()) {
        tail_ -= buf_.size();
        std::cout << "normal tail " << tail_ << std::endl;
        }
    }

    void insertArray(std::vector<Type>& c) {
        if(c.empty()) {
            return;
        }
        if(c.size() >= buf_.size()) {
            tail_ = 0;
            std::memcpy(&buf_[0], &c[c.size() - buf_.size()], buf_.size() * sizeof(Type));
            cur_size_ = buf_.size();
        } else {
            uint32_t part = buf_.size() - tail_;
            if(part >= c.size()) {
                std::memcpy(&buf_[tail_], &c[0], c.size() * sizeof(Type));
            } else {
                 uint32_t part2 = c.size() - part;
                 std::memcpy(&buf_[tail_], &c[0], part * sizeof(Type));
                 std::memcpy(&buf_[0], &c[part], part2 * sizeof(Type));
            }
            tail_ += c.size();
            normalizeTail();
            cur_size_ += c.size();
            if(cur_size_ > buf_.size()) {
                cur_size_ = buf_.size();
            }
        }
    }

    void insertData(Type data) {
        buf_[tail_] = data;
        ++tail_;
        if(cur_size_ < buf_.size()) {
            ++cur_size_;
        }
        normalizeTail();
    }

    bool isCorrectFrame(uint32_t frame) {
        return (frame < cur_size_ && frame >=0);
    }

    const Type getFrame(uint32_t frame) {
        if(!isCorrectFrame(frame)) {
            throw std::out_of_range("Out of range");
        }
        if(cur_size_ < buf_.size()) {
            return buf_[frame];
        }

        frame += tail_;

        if(frame >= cur_size_) {
            return buf_[frame - cur_size_];
        }
        return buf_[frame];
    }

    const std::vector<Type> getFrameRange(uint32_t start_pos, uint32_t end_pos) {
        if(!isCorrectFrame(start_pos) || !isCorrectFrame(end_pos) || start_pos > end_pos) {
            throw std::overflow_error("Range is out of borders or equal to each other(use <getFrame> function to get single frame");
        }

         auto size_range{end_pos - start_pos + 1};
         std::vector<Type> frame_vec(size_range);

         if(cur_size_ < buf_.size()) {
                std::memcpy(&frame_vec[0], &buf_[0], size_range * sizeof(Type));
         } else {
             if((tail_ + start_pos) + size_range <= buf_.size()) {
                std::memcpy(&frame_vec[0], &buf_[tail_ + start_pos],size_range * sizeof(Type));
             } else {
                auto part2 = (tail_ + start_pos + size_range) - buf_.size();
                auto part1 = size_range - part2;
                std::memcpy(&frame_vec[0], &buf_[tail_ + start_pos],part1 * sizeof(Type));
                std::memcpy(&frame_vec[part1], &buf_[0],part2 * sizeof(Type));
             }
         }

        return frame_vec;
    }

private:
    std::vector<Type> buf_{};
    uint32_t tail_ = 0;
    uint32_t cur_size_ = 0;
};
