#include <iostream>
#include "non_std_make_unique.hpp"
#include "Functional.hpp"

template <typename T>
class DoublyLinkedList {
public:
    DoublyLinkedList() = default;

    virtual ~DoublyLinkedList() {
        clear();
    }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    DoublyLinkedList(DoublyLinkedList&& source) noexcept {
        clear();

        size = source.size;
        std::swap(head, source.head);
        std::swap(tail, source.tail);
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& source) noexcept {
        if (this != &source) {
            clear();

            size = source.size;
            std::swap(head, source.head);
            std::swap(tail, source.tail);
        }

        return *this;
    }

    T& getFirst() {
        if (!size) {
            throw std::out_of_range("List is empty");
        }

        return *head->data;
    }

    T& getLast() {
        if (!size) {
            throw std::out_of_range("List is empty");
        }

        return *(tail.lock()->data);
    }

    void add(T const& elem) {
        addLast(elem);
    }

    void addFirst(T const& elem) {
        if (!size) {
            tail = head = std::make_shared<Node>(elem, nullptr, nullptr);
        } else {
            auto new_node = std::make_shared<Node>(elem, head, nullptr);
            head->prev = new_node;
            head = new_node;
        }

        ++size;
    }

    void addLast(T const& elem) {
        if (!size) {
            tail = head = std::make_shared<Node>(elem, nullptr, nullptr);
        } else {
            auto locked_tail = tail.lock();
            locked_tail->next = std::make_shared<Node>(elem, nullptr, locked_tail);
            tail = locked_tail->next;
        }

        ++size;
    }

    void insertAt(size_t index, T const& elem) {
        if (index == 0) return addFirst(elem);
        if (index >= size) throw std::out_of_range("Out of range");
        if (index == size - 1) return addLast(elem);

        std::shared_ptr<Node> trav;

        if (index < size / 2) {
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

    int indexOf(T const& elem) {
        std::shared_ptr<Node> trav = head;
        for (size_t idx = 0; idx < size; ++idx, trav = trav->next) {
            if (*trav->data == elem) {
                return idx;
            }
        }

        return -1;
    }

    void removeAt(size_t index) {
        if (index == 0) return removeFirst();
        if (index >= size) throw std::out_of_range("Out of range");
        if (index == size - 1) return removeLast();

        std::shared_ptr<Node> trav;

        if (index < size / 2) {
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

    void removeFirst() {
        if (!size) throw std::out_of_range("List is empty");
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

    void removeLast() {
        if (!size) throw std::out_of_range("List is empty");
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

    void clear() {
        while (head) {
            removeLast();
        }
    }

    size_t sizeOf() noexcept {
        return size;
    }

    friend std::ostream& operator<<(std::ostream& os, DoublyLinkedList const& l) {
        os << "[";

        std::shared_ptr<Node> trav = l.head;
        while (trav) {
            os << *trav->data;
            if (trav->next) {
                os << " <-> ";
            }

            trav = trav->next;
        }

        os << "]";
        return os;
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
        bidirect_iter(std::shared_ptr<Node> ptr) {
            if (ptr.get()) {
                node_ptr = ptr;
                return;
            }

            throw std::invalid_argument("Cant iterate over nullptr");
        }

        T& extract() {
            return *(node_ptr.lock()->data);
        }

        bidirect_iter& step_forward() {
            auto ptr_locked = node_ptr.lock();
            if (ptr_locked->next.get()) {
                node_ptr = ptr_locked->next;
                end_reached = false;
                return *this;
            }

            end_reached = true;
            return *this;
        }

        bidirect_iter& step_backward() {
            auto prev_locked = node_ptr.lock()->prev.lock();
            if (prev_locked.get()) {
                node_ptr = prev_locked;
                end_reached = false;
                return *this;
            }

            end_reached = true;
            return *this;
        }

        bool exhausted() {
            return end_reached;
        }

    private:
        std::weak_ptr<Node> node_ptr;
        bool end_reached = false;
    };

    Maybe<bidirect_iter> fd_iter() {
        if (head.get()) {
            return return_<Maybe>(bidirect_iter(head));
        }

        return Maybe<bidirect_iter>();
    }

    Maybe<bidirect_iter> bk_iter() {
        auto tail_locked = tail.lock();
        if (tail_locked.get()) {
            return return_<Maybe>(bidirect_iter(tail_locked));
        }

        return Maybe<bidirect_iter>();
    }

private:
    size_t size = 0;
    std::shared_ptr<Node> head = nullptr;
    std::weak_ptr<Node> tail;
};

int main(void) {
    DoublyLinkedList<int> dl;

    dl.add(1);
    dl.add(2);
    dl.add(3);
    dl.add(4);
    dl.add(5);

    std::cout << dl << std::endl;

    dl.insertAt(1, 9);
    dl.insertAt(3, 10);
    dl.insertAt(0, 50);
    dl.getFirst() = 51;

    function<bool(decltype(dl)::bidirect_iter)> isIterExhausted = 
        [](decltype(dl)::bidirect_iter it) { return !it.exhausted(); };

    for (
        auto maybe_it = dl.fd_iter();
        (isIterExhausted % maybe_it).on(
            [](bool exh) { return exh; },
            []() { return false; }
        );
        maybe_it.fromJust().step_forward()
    ) {
        std::cout << maybe_it.fromJust().extract() << " ";
    }

    dl.removeFirst();
    dl.removeLast();
    dl.removeAt(dl.indexOf(10));

    std::cout << std::endl << dl << std::endl;
    std::cout << "First element: " << dl.getFirst() << std::endl;
    std::cout << "Last element: " << dl.getLast() << std::endl;
    std::cout << "Index of 10: " << dl.indexOf(10) << std::endl;
    std::cout << "Size of the list: " << dl.sizeOf() << std::endl;
    return 0;
}
