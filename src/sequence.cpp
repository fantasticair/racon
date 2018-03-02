/*!
 * @file sequence.cpp
 *
 * @brief Sequence class source file
 */

#include "sequence.hpp"

namespace racon {

std::unique_ptr<Sequence> createSequence(const std::string& name,
    const std::string& data) {

    return std::unique_ptr<Sequence>(new Sequence(name, data));
}

Sequence::Sequence(const char* name, uint32_t name_length, const char* data,
    uint32_t data_length)
        : name_(name, name_length), data_(data, data_length),
        reverse_complement_(), quality_(), reverse_quality_() {
}

Sequence::Sequence(const char* name, uint32_t name_length, const char* data,
    uint32_t data_length, const char* quality, uint32_t quality_length)
        : name_(name, name_length), data_(data, data_length),
        reverse_complement_(), quality_(quality, quality_length),
        reverse_quality_() {

    uint32_t quality_sum = 0;
    for (const auto& it: quality_) {
        quality_sum += it - '!';
    }

    if (quality_sum == 0) {
        std::string().swap(quality_);
    }
}

Sequence::Sequence(const std::string& name, const std::string& data)
    : name_(name), data_(data), reverse_complement_(), quality_(),
    reverse_quality_() {
}

void Sequence::create_reverse_complement() {

    if (!reverse_complement_.empty()) {
        return;
    }

    reverse_complement_.clear();
    reverse_complement_.reserve(data_.size());

    for (int32_t i = data_.size() - 1; i >= 0; --i) {
        switch (data_[i]) {
            case 'A':
                reverse_complement_ += 'T';
                break;
            case 'T':
                reverse_complement_ += 'A';
                break;
            case 'C':
                reverse_complement_ += 'G';
                break;
            case 'G':
                reverse_complement_ += 'C';
                break;
            default:
                reverse_complement_ += data_[i];
                break;
        }
    }

    reverse_quality_.clear();
    reverse_quality_.reserve(quality_.size());

    for (int32_t i = quality_.size() - 1; i >= 0; --i) {
        reverse_quality_ += quality_[i];
    }
}

void Sequence::transmute(bool has_name, bool has_data, bool has_reverse_data) {

    if (!has_name) {
        std::string().swap(name_);
    }

    if (has_reverse_data) {
        create_reverse_complement();
    }

    if (!has_data) {
        std::string().swap(data_);
        std::string().swap(quality_);
    }
}

}