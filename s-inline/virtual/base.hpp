#ifndef __BASE_HPP
#define __BASE_HPP

class Base {
 private:
  int data_;

 public:
  virtual void set() noexcept = 0;
};

#endif
