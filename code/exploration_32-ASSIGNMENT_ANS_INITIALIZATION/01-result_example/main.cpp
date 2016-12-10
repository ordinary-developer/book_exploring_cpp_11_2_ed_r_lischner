#include <cassert>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <sstream>

int gcd(int n, int m)
{
  n = std::abs(n);
  while (m != 0) {
    int tmp{n % m};
    n = m;
    m = tmp;
  }
  return n;
}

struct rational
{
  rational(int num, int den)
  : numerator{num}, denominator{den}
  {
    reduce();
  }
  rational(int num)
  : numerator{num}, denominator{1}
  {}

  void assign(int num, int den)
  {
    numerator = num;
    denominator = den;
    reduce();
  }


  void reduce()
  {
    assert(denominator != 0);
    if (denominator < 0)
    {
      denominator = -denominator;
      numerator = -numerator;
    }
    int div{gcd(numerator, denominator)};
    numerator = numerator / div;
    denominator = denominator / div;
  }

  int numerator; 
  int denominator; 
};

bool operator==(rational const& a, rational const& b)
{
  return a.numerator == b.numerator and a.denominator == b.denominator;
}

inline bool operator!=(rational const& a, rational const& b)
{
  return not (a == b);
}


bool operator<(rational const& a, rational const& b)
{
  return a.numerator * b.denominator < b.numerator * a.denominator;
}


inline bool operator<=(rational const& a, rational const& b)
{
  return not (b < a);
}

inline bool operator>(rational const& a, rational const& b)
{
  return b < a;
}


inline bool operator>=(rational const& a, rational const& b)
{
  return not (b > a);
}


rational operator+(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator + rhs.numerator * lhs.denominator,
                  lhs.denominator * rhs.denominator};
}

rational operator-(rational const& r)
{
  return rational{-r.numerator, r.denominator};
}


rational operator-(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator - rhs.numerator * lhs.denominator,
                  lhs.denominator * rhs.denominator};
}

rational operator*(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.numerator, lhs.denominator * rhs.denominator};
}

rational operator/(rational const& lhs, rational const& rhs)
{
  return rational{lhs.numerator * rhs.denominator, lhs.denominator * rhs.numerator};
}

std::istream& operator>>(std::istream& in, rational& rat)
{
  int n{0}, d{0};
  char sep{'\0'};
  if (not (in >> n >> sep))
    in.setstate(std::cin.failbit);
  else if (sep != '/')
  {
    in.unget();
    rat.assign(n, 1);
  }
  else if (in >> d)
    rat.assign(n, d);
  else
    in.setstate(std::cin.failbit);

  return in;
}

std::ostream& operator<<(std::ostream& out, rational const& rat)
{
  std::ostringstream tmp{};
  tmp << rat.numerator;
  if (rat.denominator != 1)
    tmp << '/' << rat.denominator;
  out << tmp.str();

  return out;
}

bool iofailure(std::istream& in)
{
  return in.fail() and not in.bad();
}

int main()
{
  rational r{0};

  while (std::cin)
  {
    if (std::cin >> r)
      std::cout << r << '\n';
    else if (iofailure(std::cin))
    {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<int>::max(), '\n');
    }
  }

  if (std::cin.bad()) 
    std::cerr << "Unrecoverable input failure\n";
}
