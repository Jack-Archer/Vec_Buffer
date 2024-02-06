#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <exception>


template <typename Type>
class VecBuffer{
public:
    VecBuffer() = delete;

    VecBuffer(uint16_t buf_size) {
        buf_.resize(buf_size);
    }

    void clearBuf() {
        buf_.clear();
        tail_ = 0;
        cur_size_ = 0;
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
            int part = buf_.size() - tail_;
            if(part >= c.size()) {
                std::memcpy(&buf_[tail_], &c[0], c.size() * sizeof(Type));
            } else {
                 int part2 = c.size() - part;
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

    bool isCorrectFrame(int frame) {
        return (frame < cur_size_ && frame >=0);
    }

    const Type getFrame(int frame) {
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

    const std::vector<Type> getFrameRange(int start_pos, int end_pos) {
        if(!isCorrectFrame(start_pos) || !isCorrectFrame(end_pos) || start_pos > end_pos) {
            throw std::overflow_error("Range is out of borders or equal to each other(use <getFrame> function");
        }
         std::vector<Type> frame_vec{};
         auto  it1 {buf_.begin() + start_pos};
         auto  it2 {buf_.begin() + end_pos + 1};

         if(cur_size_ < buf_.size()) {
            std::copy(it1, it2 ,std::back_inserter(frame_vec));
         }
         if((tail_ + end_pos) >= cur_size_) {
             it1 = buf_.begin() + tail_ + start_pos;
             std::copy(it1, buf_.end(), std::back_inserter(frame_vec));
             end_pos -= cur_size_ - tail_;
             it2 = buf_.begin() + end_pos + 1;
             std::copy(buf_.begin(), it2, std::back_inserter(frame_vec));
         } else {
                 it1 = buf_.begin() + tail_ + start_pos;
                 it2 = buf_.begin() + tail_ + end_pos + 1;
                 std::copy(it1, it2 ,std::back_inserter(frame_vec));
                }
        return frame_vec;
    }

private:
    std::vector<Type> buf_{};
    uint16_t tail_ = 0;
    uint16_t cur_size_ = 0;
};
