#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "../Headers/non_std_make_unique.hpp"
#include "../Headers/Functional.hpp"

template <typename T>
class SinglyLinkedList {
public:
    SinglyLinkedList() = default;

    virtual ~SinglyLinkedList() noexcept {
        clear();
    }

    SinglyLinkedList(const SinglyLinkedList&) = delete;
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;

    SinglyLinkedList(SinglyLinkedList&& source) noexcept {
        clear();

        size = source.size;
        std::swap(head, source.head);
    }

    SinglyLinkedList& operator=(SinglyLinkedList&& source) noexcept {
        if (this != &source) {
            clear();

            size = source.size;
            std::swap(head, source.head);
        }

        return *this;
    }

    Maybe<T> get() const noexcept {
        if (!size) {
            return Maybe<T>(); 
        }

        return return_<Maybe>(*head->data);
    }

    void add(T const& elem) noexcept {
        head = std::make_shared<Node>(elem, head);
        ++size;
    }

    void insertAt(size_t index, T const& elem) noexcept {
        if (size == 0 || index == 0) return add(elem);
        if (index >= size) index = size - 1;
        else --index;

        std::shared_ptr<Node> trav = head;
        for (size_t i = 0; i < index; ++i) {
            trav = trav->next;
        }

        trav->next = std::make_shared<Node>(elem, trav->next);
        ++size;
    }

    Maybe<size_t> indexOf(T const& elem) const noexcept {
        std::shared_ptr<Node> trav = head;
        for (size_t idx = 0; idx < size; ++idx, trav = trav->next) {
            if (*trav->data == elem) {
                return return_<Maybe>(idx);
            }
        }

        return Maybe<size_t>();
    }

    void removeAt(size_t index) noexcept {
        if (size <= 1 || index == 0) return remove();
        if (index >= size) index = size - 1;
        --index;

        std::shared_ptr<Node> trav = head;
        for (size_t i = 0; i < index; ++i) {
            trav = trav->next;
        }

        trav->next = trav->next->next;
        --size;
    }

    void remove() noexcept {
        if (!size) return;
        head = head->next;
        --size;
    }

    void clear() noexcept {
        while (head) {
            remove();
        }
    }

    size_t sizeOf() const noexcept {
        return size;
    }

    std::string toString() const noexcept {
        std::stringstream ss;
        ss << "[";

        std::shared_ptr<Node> trav = head;
        while (trav) {
            ss << *trav->data;
            if (trav->next) {
                ss << " -> ";
            }

            trav = trav->next;
        }

        ss << "]";
        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, SinglyLinkedList const& l) {
        return os << l.toString();
    }

protected:
    class Node {
    public:
        Node(T const& i_data, std::shared_ptr<Node>& i_next) : 
            data(non_std::make_unique<T>(i_data)), 
            next(i_next)
        {}

       Node(T const& i_data, std::nullptr_t i_next) :
            data(non_std::make_unique<T>(i_data)),
            next(i_next)
        {}

        std::unique_ptr<T> data;
        std::shared_ptr<Node> next;
    };

public:
    class forward_iter {
    public:
        forward_iter& operator=(const forward_iter& source) {
            if (std::addressof(master) != std::addressof(source.master)) {
                throw std::invalid_argument("Unable to copy forward_iter owned by another instance of SinglyLinkedList");
            }

            if (std::addressof(*this) != std::addressof(source)) {
                reset();
            }

            return *this;
        }

        Maybe<T> extract() const noexcept {
            auto ptr_locked = node_ptr.lock();
            if (ptr_locked) return return_<Maybe>(*ptr_locked->data);
            return Maybe<T>();
        }

        void assign(T const& elem) noexcept {
            auto ptr_locked = node_ptr.lock();
            if (ptr_locked) *ptr_locked->data = elem; 
        }

        void reset() noexcept {
            node_ptr = master.head;
            end_reached = !master.head;
        }

        forward_iter& step() noexcept {
            auto ptr_locked = node_ptr.lock();
            if (ptr_locked) {
                node_ptr = ptr_locked->next;
                if (ptr_locked->next) {
                    return *this;
                }
            }

            end_reached = true;
            return *this;
        }

        bool exhausted() const noexcept {
            return end_reached;
        }

        friend class SinglyLinkedList;

    private:
        forward_iter(SinglyLinkedList const& l) :
            master{l},
            node_ptr(l.head),
            end_reached{!l.head} 
        {}

        SinglyLinkedList const& master;
        std::weak_ptr<Node> node_ptr;
        bool end_reached;
    };

    friend class forward_iter;

    forward_iter iter() noexcept {
        return forward_iter(*this);
    }

private:
    size_t size = 0;
    std::shared_ptr<Node> head = nullptr;
};

/*int main(void) {
    SinglyLinkedList<int> lst;

    lst.insertAt(1, 9);
    lst.removeAt(1);
    lst.insertAt(1, 9);
    lst.insertAt(3, 10);
    lst.removeAt(1);
    lst.insertAt(3, 10);
    lst.insertAt(5, 15);
    lst.removeAt(1);

    lst.add(1);
    lst.add(2);
    lst.add(3);
    lst.add(4);
    lst.add(5);

    std::cout << lst << std::endl;

    auto it = lst.iter();
    it.assign(51);

    for (;!it.exhausted(); it.step()) {
        std::cout << it.extract().fromJust() << " ";
    }

    std::cout << std::endl << "Exhausted iterator yields: " << it.extract() << std::endl;
    std::cout << "First element: " << lst.get() << std::endl;
    std::cout << "Index of 10: " << lst.indexOf(10) << std::endl;
    std::cout << "Size of the list: " << lst.sizeOf() << std::endl;
    return 0;
}*/
