#include "Sequence.hpp"

Sequence::Sequence(std::initializer_list<float> data) : i_(0), n_(data.size())
{
    assert(n_ > 0);
    seq_ = std::vector(data);
}

void Sequence::inc_ptr() {
    i_++;
    i_ %= n_;
}

float Sequence::operator()() {
    float ret = seq_[i_];
    inc_ptr();
    return ret;
}
