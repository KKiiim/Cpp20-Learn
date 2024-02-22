#include <iostream>
#include <memory>

class MyClass : public std::enable_shared_from_this<MyClass> {
public:
   MyClass() {
       std::cout << "MyClass constructor called." << std::endl;
   }

   ~MyClass() {
       std::cout << "MyClass destructor called." << std::endl;
   }

   std::shared_ptr<MyClass> getSharedFromThis() {
       return shared_from_this();
   }
};

int main() {
   // Create an instance of MyClass using std::make_shared.
   std::shared_ptr<MyClass> myClassInstance = std::make_shared<MyClass>();

   // Get another shared_ptr to the same instance from inside the class.
   std::shared_ptr<MyClass> anotherSharedInstance = myClassInstance->getSharedFromThis();

   // Both shared_ptrs now point to the same MyClass instance.
   std::cout << "Use count of myClassInstance: " << myClassInstance.use_count() << std::endl;
   std::cout << "Use count of anotherSharedInstance: " << anotherSharedInstance.use_count() << std::endl;

   return 0;
}