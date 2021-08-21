
#include <iosfwd>

class StackOnList {
 public:
    StackOnList() = default;
    StackOnList(const StackOnList &arr);
    ~StackOnList();
    void push(const int val);
    void pop();
    int& top();
    bool empty() const { return head_ == nullptr; }
    StackOnList& operator=(const StackOnList &arr);
 private:
    struct Node {
        Node* next_{ nullptr };
        int data_{ 0 };
        Node(Node* next, const int &data) : next_(next), data_(data) { }
    };
    Node* head_{ nullptr };
};


