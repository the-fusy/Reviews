#include <iostream>

template<typename T>
struct Node {
    T value;
    Node<T>* next;

    Node(T val) : value(val), next(nullptr) {}
};

template<typename T>
struct List {
    Node<T>* root;
    Node<T>* last;
    int size;
    
    List() : root(nullptr), last(nullptr), size(0) {}

    List(List& list) : root(nullptr), last(nullptr), size(0) {
        Node<T>* now = list.root;
        for (int i = 0; i < list.size; ++i) {
            addElement(now->value);
            now = now->next;
        }
    }

    List(List list, int l, int r) : root(nullptr), last(nullptr), size(0) {
        Node<T>* now = list.root;
        for (int i = 0; i < list.size; ++i) {
            if (i >= r) break;
            if (i >= l) {
                addElement(now->value);
            }
            now = now->next;
        }
    }

    void addElement(int val) {
        Node<T>* node = new Node<T>(val);
        if (last == nullptr) {
            root = node;
            last = node;
        } else {
            last->next = node;
            last = node;
        }
        size++;
    }

    ~List() {
        Node<T>* now = root;
        while (now != nullptr) {
            now = now->next;
            delete root;
            root = now;
        }
    }
};

template<typename T>
List<T>& mergeSort(List<T>& v, int l, int r) {
    if (r - l == 1) return v;
    List<T> list1(v, l - l, (r + l) / 2 - l);
    List<T> list2(v, (r + l) / 2 - l, r - l);
    List<T> sortedList1 = mergeSort(list1, l, (r + l) / 2);
    List<T> sortedList2 = mergeSort(list2, (r + l) / 2, r);
    Node<T>* it1 = sortedList1.root;
    Node<T>* it2 = sortedList2.root;
    Node<T>* now = v.root;
    while (it1 != nullptr || it2 != nullptr) {
        if (it1 == nullptr) {
            now->value = it2->value;
            it2 = it2->next;
        } else if (it2 == nullptr) {
            now->value = it1->value;
            it1 = it1->next;
        } else if (it1->value <= it2->value) {
            now->value = it1->value;
            it1 = it1->next;
        } else {
            now->value = it2->value;
            it2 = it2->next;
        }
        now = now->next;
    }
    return v;
}

int main() {
    List<int> list;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int temp;
        std::cin >> temp;
        list.addElement(temp);
    }
    list = mergeSort(list, 0, n);
    Node<int>* now = list.root;
    for (int i = 0; i < n; ++i) {
        std::cout << now->value << ' ';
        now = now->next;
    }
}