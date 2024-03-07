#ifndef __V_HPP
#define __V_HPP
#include "base.hpp"
class VDerived : public Base {
 public:
  void set() noexcept override;

 public:
  int d = 0;
};

#endif
