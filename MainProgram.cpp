/************************************************************
 CMP2003 - Data Structures
 LAB 4 - Array List  (arrayListType)
 ------------------------------------------------------------
 THIS IS THE ONLY FILE YOU EDIT AND SUBMIT.

 Build & run (no other files needed):
     g++ -Wall --std=c++17 MainProgram.cpp -o main
     ./main                      (Windows: main.exe)

 The program compiles and runs from the start: unfinished
 functions just print a "failed" message in main(). Add your
 own tests to main() as you like. On submission the official
 autograder tests each function separately and gives partial
 points per function.

 YOUR TASKS  (edit ONLY the 4 functions marked TODO below)
   Task 1 - removeAt(int location)      30 pts
   Task 2 - removeAll(const elemType&)  30 pts
   Task 3 - min() const                 20 pts
   Task 4 - max() const                 20 pts

 RULES
   - Edit only the 4 TODO functions (main() is yours to play
     with, the autograder does not use it).
   - Do not use std::vector or other STL containers in your
     solution: this lab is about manual array manipulation.
   - Do not rename the file.
************************************************************/
#include <iostream>
#include <cassert>
#include <stdexcept>
using namespace std;

template <class elemType>
class arrayListType {
public:
    bool isEmpty() const;
    bool isFull() const;
    int listSize() const;
    int maxListSize() const;
    void print() const;
    bool isItemAtEqual(int location, const elemType& item) const;
    void insertAt(int location, const elemType& insertItem);
    void insertEnd(const elemType& insertItem);
    void removeAt(int location);                       // Task 1
    void removeAll(const elemType& removeItem);        // Task 2
    void retrieveAt(int location, elemType& retItem) const;
    void replaceAt(int location, const elemType& repItem);
    void clearList();
    int seqSearch(const elemType& item) const;
    void insert(const elemType& insertItem);
    void remove(const elemType& removeItem);
    elemType min() const;                              // Task 3
    elemType max() const;                              // Task 4

    arrayListType(int size = 100);
    arrayListType(const arrayListType<elemType>& otherList);
    ~arrayListType();
    const arrayListType<elemType>& operator=(const arrayListType<elemType>& otherList);

protected:
    elemType* list;   // array holding the elements
    int length;       // number of elements currently stored
    int maxSize;      // capacity of the array
};

// ==========================================================
// PROVIDED IMPLEMENTATIONS (already working - do not change)
// ==========================================================
template <class elemType>
bool arrayListType<elemType>::isEmpty() const { return (length == 0); }

template <class elemType>
bool arrayListType<elemType>::isFull() const { return (length == maxSize); }

template <class elemType>
int arrayListType<elemType>::listSize() const { return length; }

template <class elemType>
int arrayListType<elemType>::maxListSize() const { return maxSize; }

template <class elemType>
void arrayListType<elemType>::print() const {
    for (int i = 0; i < length; i++)
        cout << list[i] << " ";
    cout << endl;
}

template <class elemType>
bool arrayListType<elemType>::isItemAtEqual(int location, const elemType& item) const {
    return (list[location] == item);
}

template <class elemType>
void arrayListType<elemType>::insertAt(int location, const elemType& insertItem) {
    if (location < 0 || location >= maxSize)
        cerr << "Position out of range\n";
    else if (length >= maxSize)
        cerr << "Cannot insert in a full list\n";
    else {
        for (int i = length; i > location; i--)
            list[i] = list[i - 1];
        list[location] = insertItem;
        length++;
    }
}

template <class elemType>
void arrayListType<elemType>::insertEnd(const elemType& insertItem) {
    if (length < maxSize)
        list[length++] = insertItem;
}

template <class elemType>
void arrayListType<elemType>::retrieveAt(int location, elemType& retItem) const {
    if (location >= 0 && location < length)
        retItem = list[location];
}

template <class elemType>
void arrayListType<elemType>::replaceAt(int location, const elemType& repItem) {
    if (location >= 0 && location < length)
        list[location] = repItem;
}

template <class elemType>
void arrayListType<elemType>::clearList() { length = 0; }

template <class elemType>
int arrayListType<elemType>::seqSearch(const elemType& item) const {
    for (int i = 0; i < length; i++)
        if (list[i] == item)
            return i;
    return -1;
}

template <class elemType>
void arrayListType<elemType>::insert(const elemType& insertItem) {
    if (!isFull())
        list[length++] = insertItem;
}

template <class elemType>
void arrayListType<elemType>::remove(const elemType& removeItem) {
    if (length == 0)
        cerr << "Cannot delete from an empty list." << endl;
    else {
        int loc = seqSearch(removeItem);
        if (loc != -1)
            removeAt(loc);
        else
            cout << "The item to be deleted is not in the list." << endl;
    }
}

template <class elemType>
arrayListType<elemType>::arrayListType(int size) {
    maxSize = (size > 0) ? size : 100;
    list = new elemType[maxSize];
    assert(list != nullptr);
    length = 0;
}

template <class elemType>
arrayListType<elemType>::~arrayListType() { delete[] list; }
template <class elemType>
arrayListType<elemType>::arrayListType(const arrayListType<elemType>& otherList) {
    maxSize = otherList.maxSize;
    length = otherList.length;
    list = new elemType[maxSize];
    assert(list != nullptr);
    for (int i = 0; i < length; i++)
        list[i] = otherList.list[i];
}

template <class elemType>
const arrayListType<elemType>& arrayListType<elemType>::operator=(const arrayListType<elemType>& otherList) {
    if (this != &otherList) {
        delete[] list;
        maxSize = otherList.maxSize;
        length = otherList.length;
        list = new elemType[maxSize];
        assert(list != nullptr);
        for (int i = 0; i < length; i++)
            list[i] = otherList.list[i];
    }
    return *this;
}

// ==========================================================
// YOUR WORK STARTS HERE
// Replace each "not implemented" line with your own code.
// ==========================================================

// ----------------------------------------------------------
// Task 1 - removeAt  (30 pts)
// Remove the item at index `location`. The list is UNORDERED,
// so do NOT shift elements: copy the LAST element into the
// removed slot, then decrease `length`.
// If `location` is not a valid index, print an error message
// to cerr and leave the list unchanged.
// ----------------------------------------------------------
// ----------------------------------------------------------
// Task 1 - removeAt  (30 pts)
// ----------------------------------------------------------
template <class elemType>
void arrayListType<elemType>::removeAt(int location) {
    if (location < 0 || location >= length) {
        cerr << "The location of the item to be removed is out of range." << endl;
        return;
    }
    list[location] = list[length - 1];
    length--;
}

// ----------------------------------------------------------
// Task 2 - removeAll  (30 pts)
// ----------------------------------------------------------
template <class elemType>
void arrayListType<elemType>::removeAll(const elemType& removeItem) {
    int i = 0;
    while (i < length) {
        if (list[i] == removeItem) {
            removeAt(i);
        } else {
            i++;
        }
    }
}

// ----------------------------------------------------------
// Task 3 - min  (20 pts)
// ----------------------------------------------------------
template <class elemType>
elemType arrayListType<elemType>::min() const {
    assert(length > 0);
    elemType smallest = list[0];
    for (int i = 1; i < length; i++) {
        if (list[i] < smallest) {
            smallest = list[i];
        }
    }
    return smallest;
}

// ----------------------------------------------------------
// Task 4 - max  (20 pts)
// ----------------------------------------------------------
template <class elemType>
elemType arrayListType<elemType>::max() const {
    assert(length > 0);
    elemType largest = list[0];
    for (int i = 1; i < length; i++) {
        if (list[i] > largest) {
            largest = list[i];
        }
    }
    return largest;
}
// ==========================================================
// YOUR WORK ENDS HERE
// ==========================================================

// ==========================================================
// EXAMPLE MAIN - try your functions here
// (the autograder compiles this file without this main)
// ==========================================================
#ifndef AUTOGRADING
int main() {
    arrayListType<int> list(20);
    list.insertEnd(5);
    list.insertEnd(7);
    list.insertEnd(3);
    list.insertEnd(7);
    list.insertEnd(9);
    list.insertEnd(7);

    cout << "\n=== INITIAL LIST ===\n";
    list.print();                       // 5 7 3 7 9 7

    // Task 1
    cout << "\n[TEST] removeAt(1)" << endl;
    try {
        list.removeAt(1);
        list.print();                   // 5 7 3 7 9   (any order is fine)
    } catch (...) {
        cout << "removeAt test failed" << endl;
    }

    // Task 2
    cout << "\n[TEST] removeAll(7)" << endl;
    try {
        list.removeAll(7);
        list.print();                   // 5 9 3       (any order is fine)
    } catch (...) {
        cout << "removeAll test failed" << endl;
    }

    // Task 3
    cout << "\n[TEST] min()" << endl;
    try {
        int minValue = list.min();
        cout << "Min element = " << minValue << endl;   // 3
    } catch (...) {
        cout << "min() test failed" << endl;
    }

    // Task 4
    cout << "\n[TEST] max()" << endl;
    try {
        int maxValue = list.max();
        cout << "Max element = " << maxValue << endl;   // 9
    } catch (...) {
        cout << "max() test failed" << endl;
    }

    cout << "\n=== ALL TESTS COMPLETED ===\n";
    return 0;
}
#endif
