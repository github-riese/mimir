#include "helpers.h"

#include <numeric>

using std::fabsl;

using std::vector;

using mimir::models::Probability;

namespace mimir {
namespace helpers {

float mean(const std::vector<models::Probability> &probabilities)
{
    return mean(probabilities.begin(), probabilities.end());
}

float deviation(const std::vector<models::Probability> &probabilities)
{
    return deviation(probabilities.begin(), probabilities.end());
}

std::vector<float> toVector(std::valarray<float> const&array)
{
    return {std::begin(array), std::end(array)};
}

std::valarray<float> toArray(std::vector<float> const &vector)
{
    std::valarray<float> t(vector.size());
    std::copy(std::begin(vector), std::end(vector), std::begin(t));
    return t;
}

uint64_t PackDouble::operator()(float value)
{
    if (value == 0.) {
        return 0;
    }
    unsigned bits = 64;
    unsigned expbits = 11;
    float fnorm;
    int shift;
    long long sign, exp, significand;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    // check sign and begin normalization
    if (value < 0) { sign = 1; fnorm = -value; }
    else { sign = 0; fnorm = value; }

    // get the normalized form of f and track the exponent
    shift = 0;
    while(fnorm >= 2.0) { fnorm /= 2.0; shift++; }
    while(fnorm < 1.0) { fnorm *= 2.0; shift--; }
    fnorm = fnorm - 1.0;

    // calculate the binary form (non-float) of the significand data
    significand = static_cast<long long>(fnorm * ((1LL<<significandbits) + 0.5));

    // get the biased exponent
    exp = shift + ((1<<(expbits-1)) - 1); // shift + bias

    // return the final answer
    return static_cast<uint64_t>((sign<<(bits-1)) | (exp<<(bits-expbits-1)) | significand);
}

float UnpackDouble::operator()(uint64_t packed)
{
    if (0 == packed) {
        return 0.;
    }
    unsigned bits = 64;
    unsigned expbits = 11;
    float result;
    long long shift;
    unsigned bias;
    unsigned significandbits = bits - expbits - 1; // -1 for sign bit

    // pull the significand
    result = (packed&((1LL<<significandbits)-1)); // mask
    result /= (1LL<<significandbits); // convert back to float
    result += 1.0; // add the one back on

    // deal with the exponent
    bias = (1<<(expbits-1)) - 1;
    shift = static_cast<long long>(((packed>>significandbits)&((1LL<<expbits)-1)) - bias);
    while(shift > 0) { result *= 2.0; shift--; }
    while(shift < 0) { result /= 2.0; shift++; }

    // sign it
    result *= (packed>>(bits-1))&1? -1.0: 1.0;
    return result;
}

} // namespace helpers
} // namespace mimir
