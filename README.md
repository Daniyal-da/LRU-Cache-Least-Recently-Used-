# High-Performance LRU Cache Implementation in C++

A robust implementation of the **Least Recently Used (LRU) Cache** algorithm using C++. This project demonstrates efficient memory management, polymorphism, and data structure design to achieve **O(1)** time complexity.

## 🔗 Problem Source
This project is a solution for the **Abstract Classes - Polymorphism** challenge on **HackerRank**.
- **Challenge Link:** [HackerRank - Abstract Classes - Polymorphism](https://www.hackerrank.com/challenges/abstract-classes-polymorphism/problem?isFullScreen=true)
- **Difficulty:** Hard
- **Topic:** C++, Abstract Classes, Polymorphism, LRU Cache

## 📂 Project Structure (Comparison)

This repository contains two versions of the implementation to demonstrate different coding approaches:

### 1. `LRUCache_Modular.cpp` (Recommended)
- **Style:** Engineering / Production-Ready.
- **Description:** Uses helper methods (`addToHead`, `removeNode`, `removeTail`) to break down logic.
- **Why read this?** It shows clean code principles, readability, and maintainability.

### 2. `LRUCache_Monolithic.cpp`
- **Style:** Competitive Programming / HackerRank.
- **Description:** Implements logic inline within `get` and `set` functions for brevity.
- **Why read this?** It demonstrates raw pointer manipulation logic in a compact form.

## 🚀 Features
- **O(1) Time Complexity:** Instant access and updates regardless of cache size.
- **Generic Design:** Can be easily adapted for different data types.
- **Memory Efficient:** Implements automatic eviction of the least recently used items when capacity is reached.
- **No Memory Leaks:** Proper handling of dynamic memory allocation/deallocation using pointers.

## 🛠️ Data Structures Used
To achieve $O(1)$ performance, this implementation combines:
1.  **Doubly Linked List:** Maintains the order of elements based on usage (Head = MRU, Tail = LRU).
2.  **Hash Map (`std::map`):** Stores key-node pairs for instant lookup.

## 📝 Example Usage
```cpp
int main() {
    LRUCache lru(2); // Capacity = 2
    
    lru.set(1, 10); 
    lru.set(2, 20); 
    
    cout << lru.get(1) << endl; // Returns 10 (Node 1 moves to Head)
    
    lru.set(3, 30); // Capacity full! Evicts key 2 (Tail)
    
    cout << lru.get(2) << endl; // Returns -1 (Not found)
    return 0;
}
