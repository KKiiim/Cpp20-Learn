#include <iostream>
#include <memory>
// shared_from_this demo

// Define a structure named Foo that inherits from std::enable_shared_from_this.
struct Foo : public std::enable_shared_from_this<Foo> {
  // Constructor
  Foo() { std::cout << "Foo::Foo\n"; }
  // Destructor
  ~Foo() { std::cout << "Foo::~Foo\n"; }
  // Member function that returns a shared_ptr to this object.
  std::shared_ptr<Foo> getFoo() { return shared_from_this(); }
};

int main() {
  // Create a Foo object using raw pointer.
  Foo *f = new Foo;  // Output: Foo::Foo

  // Declare a shared_ptr named pf1 without initializing it.
  std::shared_ptr<Foo> pf1;

  {  // Start of scope block
    // Create a shared_ptr named pf2 and take ownership of the Foo object
    // pointed to by f.
    std::shared_ptr<Foo> pf2(f);
    // Use the getFoo member function to create another shared_ptr (pf1) to the
    // same object.
    pf1 = pf2->getFoo();  // shares ownership of object with pf2
    // At this point, both pf1 and pf2 share ownership of the Foo object.
    std::cout << "Foo object owned count: " << pf1.use_count() << std::endl;
    std::cout << "Foo object owned count: " << pf2.use_count() << std::endl;
  }  // End of scope block, pf2 is destroyed, but the Foo object is not deleted
     // because pf1 is still owning it.

  // Output: pf2 is gone
  std::cout << "pf2 is gone\n";

  std::cout << "Foo object owned count: " << pf1.use_count() << std::endl;
}  // End of main, pf1 is destroyed, and the Foo object is deleted.

// Output: Foo::~Foo

/*

Here's a step-by-step breakdown:

1. The structure `Foo` is defined with a constructor and a destructor that print
messages when they are called. `Foo` inherits from
`std::enable_shared_from_this<Foo>`, which allows `Foo` instances to create
`std::shared_ptr` instances to `this`.

2. In the `main` function, a `Foo` object is created using a raw pointer. The
constructor message is printed.

3. A `std::shared_ptr<Foo>` named `pf1` is declared but not initialized.

4. A scope block is started to demonstrate the lifetime of another
`std::shared_ptr<Foo>` named `pf2`. Within this block, `pf2` is initialized to
take ownership of the `Foo` object pointed to by the raw pointer `f`.

5. The `getFoo` member function is called on the object that `pf2` owns. This
function uses `shared_from_this()` to return a new `std::shared_ptr<Foo>` that
shares ownership of the `Foo` instance with `pf2`. The returned
`std::shared_ptr` is assigned to `pf1`.
6. The scope block ends, causing `pf2` to be destroyed. However, the `Foo`
object is not deleted because `pf1` still owns it.
7. The message "pf2 is gone" is printed to indicate that `pf2` has been
destroyed.
8. The `main` function ends, and `pf1` goes out of scope. Since `pf1` was the
last `std::shared_ptr` owning the `Foo` object, the object is deleted, and the
destructor message is printed. It's important to note that in this example, the
raw pointer `f` is used to initialize `pf2`. This is not considered best
practice because it can lead to undefined behavior if another `std::shared_ptr`
is created independently with `f`. It's better to use `std::make_shared` to
create `pf1` directly, and then use `shared_from_this()` to create any
additional `std::shared_ptr` instances as needed.

*/