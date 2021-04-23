#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "../Headers/NonSTD.hpp"
#include "../Headers/Functional.hpp"

template <typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList() = default;

    virtual ~DoublyLinkedList() noexcept {
        clear();
    }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    DoublyLinkedList(DoublyLinkedList&& source) noexcept {
        size = source.size;
        std::swap(head, source.head);
        std::swap(tail, source.tail);
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& source) noexcept {
        clear();
        size = source.size;
        std::swap(head, source.head);
        std::swap(tail, source.tail);
        return *this;
    }

    Maybe<T> getFirst() const noexcept {
        if (!size) {
            return Maybe<T>();
        }

        return return_<Maybe>(*head->data);
    }

    Maybe<T> getLast() const noexcept {
        if (!size) {
            return Maybe<T>();
        }

        return return_<Maybe>(*(tail.lock()->data));
    }

    void add(T const& elem) noexcept {
        addLast(elem);
    }

    void addFirst(T const& elem) noexcept {
        if (!size) {
            tail = head = std::make_shared<Node>(elem, nullptr, nullptr);
        } else {
            auto new_node = std::make_shared<Node>(elem, head, nullptr);
            head->prev = new_node;
            head = new_node;
        }

        ++size;
    }

    void addLast(T const& elem) noexcept {
        if (!size) {
            tail = head = std::make_shared<Node>(elem, nullptr, nullptr);
        } else {
            auto locked_tail = tail.lock();
            locked_tail->next = std::make_shared<Node>(elem, nullptr, locked_tail);
            tail = locked_tail->next;
        }

        ++size;
    }

    void insertAt(size_t index, T const& elem) noexcept {
        if (size == 0 || index == 0) return addFirst(elem);
        if (index >= size) return addLast(elem);

        std::shared_ptr<Node> trav;

        if (index <= size / 2) {
            trav = head;
            for (size_t i = 0; i < index; ++i) {
                trav = trav->next;
            }

        } else {
            trav = tail.lock();
            for (size_t i = size - 1; i > index; --i) {
                trav = trav->prev.lock();
            }
        }

        auto trav_prev_locked = trav->prev.lock();
        auto new_node = std::make_shared<Node>(elem, trav, trav_prev_locked);
        trav->prev = trav_prev_locked->next = new_node;
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
        if (size <= 1 || index == 0) return removeFirst();
        if (index >= size) index = size - 1;
        if (index == size - 1) return removeLast();

        std::shared_ptr<Node> trav;

        if (index <= size / 2) {
            trav = head;
            for (size_t i = 0; i < index; ++i) {
                trav = trav->next;
            }

        }
        else {
            trav = tail.lock();
            for (size_t i = size - 1; i > index; --i) {
                trav = trav->prev.lock();
            }
        }

        auto trav_prev_locked = trav->prev.lock();
        trav->next->prev = trav_prev_locked;
        trav_prev_locked->next = trav->next;
        --size;
    }

    void removeFirst() noexcept {
        if (!size) return;
        if (size == 1) {
            head.reset();
            tail.reset();
        }
        else {
            head = head->next;
            head->prev.reset();
        }

        --size;
    }

    void removeLast() noexcept {
        if (!size) return;
        if (size == 1) {
            tail.reset();
            head.reset();
        }
        else {
            auto locked_tail = tail.lock();
            auto locked_prev = locked_tail->prev.lock();
            tail = locked_prev;
            locked_prev->next.reset();
        }

        --size;
    }

    void clear() noexcept {
        while (head) {
            removeLast();
        }
    }

    size_t sizeOf() const noexcept {
        return size;
    }

    friend std::string to_string(DoublyLinkedList const& l) noexcept {
        std::stringstream ss;
        ss << "[";

        std::shared_ptr<Node> trav = l.head;
        while (trav) {
            ss << *trav->data;
            if (trav->next) {
                ss << " <-> ";
            }

            trav = trav->next;
        }

        ss << "]";
        return ss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, DoublyLinkedList const& l) noexcept {
        return os << non_std::to_string(l);
    }

    bool operator!=(DoublyLinkedList const& rhs) const noexcept {
        return !operator==(rhs);
    }

    bool operator==(DoublyLinkedList const& rhs) const noexcept {
        if (std::addressof(*this) == std::addressof(rhs)) {
            return true;
        }

        if (size != rhs.size) {
            return false;
        }

        auto this_it = fd_iter();
        auto rhs_it = rhs.fd_iter();
        for (;!this_it.exhausted(); this_it.step_forward(), rhs_it.step_forward()) {
            if (this_it.extract() != rhs_it.extract()) {
                return false;
            }
        }

        return true;
    }

protected:
    class Node {
    public:
        Node(T const& i_data, std::shared_ptr<Node>& i_next, std::shared_ptr<Node>& i_prev) : 
            data(non_std::make_unique<T>(i_data)), 
            next(i_next),
            prev(i_prev)
        {}

       Node(T const& i_data, std::nullptr_t i_next, std::shared_ptr<Node>& i_prev) :
            data(non_std::make_unique<T>(i_data)),
            next(i_next),
            prev(i_prev)
        {}

        Node(T const& i_data, std::shared_ptr<Node>& i_next, std::nullptr_t i_prev) :
            data(non_std::make_unique<T>(i_data)),
            next(i_next)
        {}

        Node(T const& i_data, std::nullptr_t i_next, std::nullptr_t i_prev) : 
            data(non_std::make_unique<T>(i_data)),
            next(i_next)
        {}

        std::unique_ptr<T> data;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
    };

public:
    class bidirect_iter {
    public:
        bidirect_iter& operator=(const bidirect_iter& source) {
            if (std::addressof(master) != std::addressof(source.master)) {
                throw std::invalid_argument("Unable to copy bidirect_iter owned by another instance of DoublyLinkedList");
            }

            if (std::addressof(*this) != std::addressof(source)) {
                forward_init = source.forward_init;
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
            auto ptr = forward_init ? master.head : master.tail.lock();
            node_ptr = ptr;
            end_reached = !ptr;
        }

        bidirect_iter& step_forward() noexcept {
            auto ptr_locked = node_ptr.lock();
            if (ptr_locked) {
                if (!ptr_locked->next) {
                    end_reached = true;
                    return *this;
                }

                node_ptr = ptr_locked->next;
                end_reached = false;
                return *this;
            }

            end_reached = true;
            return *this;
        }

        bidirect_iter& step_backward() noexcept {
            auto ptr_locked = node_ptr.lock();
            if (ptr_locked) {
                auto prev_locked = ptr_locked->prev.lock();
                if (!prev_locked) {
                    end_reached = true;
                    return *this;
                }

                node_ptr = prev_locked;
                end_reached = false;
                return *this;
            }

            end_reached = true;
            return *this;
        }

        bool exhausted() const noexcept {
            return end_reached;
        }

        friend class DoublyLinkedList;

    private:
        bidirect_iter(DoublyLinkedList const& l, bool fd = true) :
            master{l},
            forward_init{fd}
        {
            auto ptr = fd ? l.head : l.tail.lock();
            node_ptr = ptr;
            end_reached = !ptr;
        }

        DoublyLinkedList const& master;
        std::weak_ptr<Node> node_ptr;
        bool end_reached;
        bool forward_init;
    };

    friend class bidirect_iter;

    bidirect_iter fd_iter() const noexcept {
        return bidirect_iter(*this);
    }

    bidirect_iter bk_iter() const noexcept {
        return bidirect_iter(*this, false);
    }

private:
    size_t size = 0;
    std::shared_ptr<Node> head = nullptr;
    std::weak_ptr<Node> tail;
};

/*int main(void) {
    DoublyLinkedList<int> dl;

    dl.insertAt(1, 9);
    dl.removeAt(1);
    dl.insertAt(1, 9);
    dl.insertAt(3, 10);
    dl.removeAt(1);
    dl.insertAt(3, 10);
    dl.insertAt(5, 15);
    dl.removeAt(1);

    dl.add(1);
    dl.add(2);
    dl.add(3);
    dl.add(4);
    dl.add(5);

    std::cout << dl << std::endl;

    auto it = dl.fd_iter();
    it.assign(51);

    for (;!it.exhausted(); it.step_forward()) {
        std::cout << it.extract().fromJust() << " ";
    }

    std::cout << std::endl;

    it = dl.bk_iter();
    for (;!it.exhausted(); it.step_backward()) {
        std::cout << it.extract().fromJust() << " ";
    }

    std::cout << std::endl << "Exhausted iterator yields: " << it.extract() << std::endl;
    std::cout << "First element: " << dl.getFirst() << std::endl;
    std::cout << "Last element: " << dl.getLast() << std::endl;
    std::cout << "Index of 10: " << dl.indexOf(10) << std::endl;
    std::cout << "Size of the list: " << dl.sizeOf() << std::endl;
    return 0;
}*/
