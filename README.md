# 🚀 Lab Assignment 4 — Array List

Welcome to **Lab 4**!
In this lab you will **extend the `arrayListType` class** to make it more efficient and add new features. You will implement **4 functions** in a single file, `MainProgram.cpp`, and the automated tests will tell you how you did ✅.

All the other files are part of the grading system.
⚠️ **Do not edit any other files** — your grade depends on this!

You will learn to:

* 🗑️ Remove elements efficiently from an **unordered** list
* 🔁 Handle **multiple occurrences** of the same element
* 🔽🔼 Find the **minimum** and **maximum** of a list
* 🛡️ Use `assert` to protect your functions from invalid use

---

## 📂 Project Structure

```text
.
├── README.md                   # This file
├── Makefile                    # Builds & runs your program / grade (don't touch)
├── autograder.cpp              # Automated tests (don't touch)
└── MainProgram.cpp             # ← ✍️ YOU WRITE THIS FILE
```

---

## 🧱 The Class

The class `arrayListType` and **all of its functions are already written for you** — except the four you must implement. The UML diagram:

```text
arrayListType<elemType>
-------------------------------------------------------
# list    : elemType*
# length  : int
# maxSize : int
-------------------------------------------------------
+ isEmpty() const : bool
+ isFull() const : bool
+ listSize() const : int
+ maxListSize() const : int
+ print() const : void
+ isItemAtEqual(int, const elemType&) const : bool
+ insertAt(int, const elemType&) : void
+ insertEnd(const elemType&) : void
+ removeAt(int) : void                       ← ✍️ Task 1
+ removeAll(const elemType&) : void          ← ✍️ Task 2
+ retrieveAt(int, elemType&) const : void
+ replaceAt(int, const elemType&) : void
+ clearList() : void
+ seqSearch(const elemType&) const : int
+ insert(const elemType&) : void
+ remove(const elemType&) : void
+ min() const : elemType                     ← ✍️ Task 3
+ max() const : elemType                     ← ✍️ Task 4
+ arrayListType(int = 100)
+ arrayListType(const arrayListType<elemType>&)
+ ~arrayListType()
+ operator=(const arrayListType<elemType>&) : const arrayListType<elemType>&
-------------------------------------------------------
```

---

## 🧑‍💻 Your Tasks

Open `MainProgram.cpp` and look for the four `// TODO` markers. Implement them **in order** — later tasks build on earlier ones.

### Task 1 — `removeAt(int location)` · 30 pts
Rewrite the removal so it does **not shift** elements. Since the list is **unordered**, simply copy the **last** element into the removed slot and decrease `length`.

* If `location` is not a valid index, print an error message to `cerr` and leave the list **unchanged**.

> 💡 Removing from the middle becomes O(1) instead of O(n).

### Task 2 — `removeAll(const elemType& removeItem)` · 30 pts
Remove **every** occurrence of `removeItem` from the list. Use your new `removeAt`.

> ⚠️ After `removeAt(i)`, a *different* element now sits at index `i`. Make sure it gets checked too!

### Task 3 — `min() const` · 20 pts
Return the **smallest** element of the list.

* Use `assert` to make sure the list is **not empty**.
* The list must **not** be modified.

### Task 4 — `max() const` · 20 pts
Return the **largest** element of the list.

* Use `assert` to make sure the list is **not empty**.
* The list must **not** be modified.

---

## ▶️ Run Your Program

`MainProgram.cpp` works **from the very first minute** — no setup, no extra files:

```bash
make run          # builds and runs your main()
make grade        # runs the official checks: every check + your score out of 100
make clean        # removes build files
```

(No `make`? Use `g++ -Wall --std=c++17 MainProgram.cpp -o main && ./main`.)

The `main()` in the file is an **example** that tries each of your functions. Unfinished functions simply print `... test failed`, so you can watch the output improve as you work. Feel free to add your own tests to `main()` — the autograder ignores it.

A fully working solution prints (element order may differ — the list is unordered):

```text
=== INITIAL LIST ===
5 7 3 7 9 7

[TEST] removeAt(1)
5 7 3 7 9

[TEST] removeAll(7)
5 9 3

[TEST] min()
Min element = 3

[TEST] max()
Max element = 9

=== ALL TESTS COMPLETED ===
```

---

## 🧪 How It Will Be Tested

* When you **push**, Classroom 50 builds your `MainProgram.cpp` together with the instructor's copy of `autograder.cpp` (your copy of it is ignored) and runs **20 small checks**, each one worth some points.
* The checks cover typical cases, edge cases, negative numbers, duplicates, invalid indexes, other data types (`double`, `string`) and the empty list.
* Every check is graded **separately**, so you earn **partial credit** — a bug in one place never costs you points elsewhere.
* A crash or an infinite loop only fails the check it happens in (each check has a time limit).
* Your score appears a minute later under **Releases** in your repository, with the name of every check that passed or failed.

You can see exactly what the autograder will give you by running `make grade` locally:

```text
Task 1 - removeAt()   (30 pts)
  [PASS]  5/5   list size decreases by one
  [FAIL]  0/10  last element is swapped into the empty slot
         -> expected: {10, 40, 30}  but got : {10, 30, 40}
...
  TOTAL: 55 / 100
```

### 💯 Grading breakdown

| Task | Function | Checks | Points |
|------|----------|:------:|:------:|
| 1 | `removeAt()` | 5 | 30 |
| 2 | `removeAll()` | 5 | 30 |
| 3 | `min()` | 5 | 20 |
| 4 | `max()` | 5 | 20 |
| | **Total** | **20** | **100** |

### 🔍 What the checks look for

| Function | Things to get right |
|----------|---------------------|
| `removeAt` | size decreases · right item removed · **last element swapped in** · removing the last/only element · invalid index (`-1`, `length`, …) |
| `removeAll` | all occurrences gone · **consecutive duplicates** · item not in list · every element equal · empty list |
| `min` / `max` | basic case, list unchanged · negative numbers · one element / first / last / duplicates · works for `double` and `string` · `assert` fires on an empty list |

---

## ⚠️ Rules & Grading

* ✅ **Only edit `MainProgram.cpp`** — and only the four `TODO` functions (plus `main()` for your own experiments).
* ❌ **Do not modify**: `Makefile`, `autograder.cpp`, `.github/`, `.classroom50.yaml`, or rename `MainProgram.cpp`. Grading uses the instructor's copies anyway.
* 🚫 **No `std::vector`** or other STL containers in your solution — this lab is about **manual array manipulation**.
* 🧹 Your code must compile with **no warnings** (`-Wall`) using **C++17**.
* 🔍 Your submission is tested automatically; partial credit is given per check.

---

<p align="center">
  <sub>
    🎓 <strong>Bahçeşehir University</strong> — CMP2003 Data Structures &amp; Algorithms (C++) · 2026–2027<br>
    Prepared by <strong>Ismail Taha Samed ÖZKAN</strong>
  </sub>
</p>
