#include <iostream>

using namespace std;

class Dog
{
public:
        Dog();
        ~Dog();

        void run(void);
        void jump(void);

private:
        /* Private member only can be called by Class inter functoin */
        void fly(void);
};

Dog::Dog()
{
        cout << "Dog Class: constructor function.\n";
}

Dog::~Dog()
{
        cout << "Dog Class: destructor function.\n";
}

void Dog::run()
{
        cout << "Dog Class: run function.\n";
}

void Dog::jump()
{
        cout << "Dog Class: jump function.\n";
        fly();
}

void Dog::fly()
{
        cout << "Dog Class: fly function.\n";
}

int main()
{
        Dog dog;

        dog.run();
        dog.jump();

        return 0;
}
