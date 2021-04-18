#include "../3LinkedLists/SinglyLinkedList.hpp"

template <typename T>
class Stack : private SinglyLinkedList<T> {
public:
    Stack() : SinglyLinkedList<T>() {}
    // copy/move constructors

    Maybe<T> pop() noexcept {
        auto result = this->get();
        this->remove();
        return result;
    }

    void push(T const& elem) noexcept {
        this->add(elem);
    }

    bool contains(T const& elem) const noexcept {
        return this->indexOf(elem).isJust();        
    }

    using SinglyLinkedList<T>::clear;
    using SinglyLinkedList<T>::sizeOf;

    friend std::ostream& operator<<(std::ostream& os, Stack const& s) {
        return os << s.toString();
    }
};

int main(void) {
    Stack<int> s;
    s.push(5);

    std::cout << s << std::endl;
    std::cout << "Stack size: " << s.sizeOf() << std::endl;
    std::cout << "Stack top: " << s.pop() << std::endl;
    std::cout << "Stack size after pop: " << s.sizeOf() << std::endl;
    std::cout << "Exhausted stack top: " << s.pop() << std::endl;
}
