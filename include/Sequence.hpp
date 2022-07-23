#ifndef Sequence_hpp
#define Sequence_hpp

#include <vector>
#include <initializer_list>

class Sequence {
public:
    Sequence(std::initializer_list<float> data);
//    float next();
    float operator()();
private:
    void inc_ptr();
    size_t i_;
    size_t n_;
    std::vector<float> seq_;
};

#endif /* Sequence_hpp */
