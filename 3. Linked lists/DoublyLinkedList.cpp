#include <iostream>
#include <memory>

template <class T>
class DoublyLinkedList {
public:
    DoublyLinkedList() = default;

    ~DoublyLinkedList() {
        while (head) {
           removeLast(); 
        }
    }

    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    DoublyLinkedList(DoublyLinkedList&& source) noexcept {
        while (head) {
            removeLast();
        }

        size = source.size;
        std::swap(head, source.head);
        std::swap(tail, source.tail);
    }

    DoublyLinkedList& operator=(DoublyLinkedList&& source) noexcept {
        if (this != &source) {
            while (head) {
                removeLast();
            }

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

        return head.data;
    }

    T& getLast() {
        if (!size) {
            throw std::out_of_range("List is empty");
        }

        return tail.data;
    }

    void add(T const& elem) {
        addLast(elem);
    }

    void addFirst(T const& elem) {
        if (!size) {
            head = tail = std::make_shared<Node>(elem, nullptr, nullptr);
        } else {
            head.prev = std::make_shared<Node>(elem, head, nullptr);
            head = head.prev;
        }

        ++size;
    }

    void addLast(T const& elem) {
        if (!size) {
            head = tail = std::make_shared<Node>(elem, nullptr, nullptr);
        } else {
            tail.next = std::make_shared<Node>(elem, nullptr, tail);
            tail = tail.next;
        }

        ++size;
    }
    
    void insertAt(size_t index, T const& val);

    void removeFirst() {
        if (!size) {
            throw std::out_of_range("List is empty");
        }

        head = head.next;
        head.prev = nullptr;
        --size;

        if (!size) {
            tail = nullptr;
        }
    }

    void removeLast() {
        if (!size) {
            throw std::out_of_range("List is empty");
        }

        auto tmp = tail.prev.lock();
        tail = tmp;
        tail.next = nullptr;
        --size;

        if (!size) {
            head = nullptr;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, DoublyLinkedList const& l) {
        os << "[";
        std::shared_ptr<Node> trav = l.head;
        while (trav) {
            os << trav;
            trav = trav.next;
        }
        os << "]";
        return os;
    }

    size_t sizeOf() noexcept {
        return size;
    }

private:
    class Node {
    public:
        Node(T const& i_data, std::shared_ptr<Node>& i_next, std::weak_ptr<Node>& i_prev) : 
            data(std::make_unique<T>(i_data)), 
            next(i_next), 
            prev(i_prev)
        {}

        friend std::ostream& operator<<(std::ostream& os, Node const& n) {
            os << n.data;
            if (n.next) {
                os << " <-> ";
            }

            return os;
        }

    private:
        std::unique_ptr<T> data;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
    };

    size_t size = 0;
    std::shared_ptr<Node> head = nullptr;
    std::weak_ptr<Node> tail = nullptr;
};
