#ifndef RATIONAL_HPP_
#define RATIONAL_HPP_

#include <cassert>

int gcd(int n, int m) {
    if (n < 0)
        n = -n;
    while (m != 0) {
        int tmp{ n % m };
        n = m;
        m = tmp;
    }

    return n;
}

#include <stdexcept>
#include <string>

class rational
{
    public:
        class zero_denominator : public std::logic_error {
            public:
                zero_denominator(std::string const& what_arg)
                    : logic_error{ what_arg } { }
        };

        rational();
        rational(int num);
        rational(int num, int den);
        void assign(int num, int den);
        int numerator() const;
        int denominator() const;
        rational& operator=(int num);
    private:
        void reduce();
        int numerator_;
        int denominator_;
};

rational::rational()
: rational{0}
{}

rational::rational(int num)
: numerator_{num}, denominator_{1}
{}

rational::rational(int num, int den)
: numerator_{ den < 0 ? -num : num },
  denominator_{ 0 == den ? throw zero_denominator("0 denominator")
                         : (den < 0 ? -den : den) }
{
    reduce();
}

void rational::assign(int num, int den) {
   numerator_ = num;
   denominator_ = den;
   reduce();
}

void rational::reduce() {
    if (0 == denominator_)
        throw zero_denominator{ "denominator is zero" };
    if (denominator_ < 0) {
        denominator_ = -denominator_;
        numerator_ = -numerator_;
    }
    int div{gcd(numerator_, denominator_)};
    numerator_ = numerator_ / div;
    denominator_ = denominator_ / div;
}

int rational::numerator() const {
    return numerator_;
}

int rational::denominator() const {
    return denominator_;
}

rational& rational::operator=(int num) {
    numerator_ = num;
    denominator_ = 1;
    return *this;
}

#endif
