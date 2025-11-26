#pragma once
#include <memory_resource>
#include <iterator>
#include <stdexcept>

template <typename T>
class Queue {
private:
    struct Node {
        T value;
        Node* next;
    };

    Node* head;
    Node* tail;
    std::pmr::polymorphic_allocator<Node> allocator;

public:
    Queue(std::pmr::memory_resource* res) 
        : head(nullptr), tail(nullptr), allocator(res) {}

    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& val) {
        Node* newNode = allocator.allocate(1);
        allocator.construct(newNode, Node{val, nullptr});

        if (tail) {
            tail->next = newNode;
            tail = newNode;
        } else {
            head = tail = newNode;
        }
    }

    void pop() {
        if (empty()) return;

        Node* temp = head;
        head = head->next;
        if (!head) {
            tail = nullptr;
        }

        allocator.destroy(temp);
        allocator.deallocate(temp, 1);
    }

    T& front() {
        if (empty()) throw std::runtime_error("Queue is empty");
        return head->value;
    }

    bool empty() const {
        return head == nullptr;
    }

    // --- ИТЕРАТОР ---
    struct Iterator {
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Node* current;

        Iterator(Node* node) : current(node) {}

        Iterator() : current(nullptr) {}

        reference operator*() const { return current->value; }
        pointer operator->() const { return &current->value; }

        Iterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator& other) const { return current == other.current; }
        bool operator!=(const Iterator& other) const { return current != other.current; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};