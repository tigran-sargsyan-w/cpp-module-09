# C++ Module 09 – STL 📚⚙️

✅ **Status**: Completed – all mandatory exercises  
🏫 **School**: 42 – C++ Modules (Module 09)  
🏅 **Score**: (score)/100

> *STL containers, parsing, stack-based evaluation, container choice, and merge-insert sorting with Ford–Johnson.*

---

## 📚 Table of Contents

* [Description](#-description)
* [Goals of the Module](#-goals-of-the-module)
* [Exercises Overview](#-exercises-overview)

  * [ex00 – Bitcoin Exchange](#ex00--bitcoin-exchange)
  * [ex01 – Reverse Polish Notation](#ex01--reverse-polish-notation)
  * [ex02 – PmergeMe](#ex02--pmergeme)
* [Why These Containers?](#-why-these-containers)
* [Ford–Johnson / Merge-Insert Sort – Quick Explanation](#-fordjohnson--merge-insert-sort--quick-explanation)
* [Requirements](#-requirements)
* [Build & Run](#-build--run)
* [Repository Layout](#-repository-layout)
* [Testing Tips](#-testing-tips)
* [Common Evaluation Questions](#-common-evaluation-questions)
* [42 Notes](#-42-notes)

---

## 📝 Description

This repository contains my solutions to **42’s C++ Module 09 (C++98)**.

Module 09 focuses on solving algorithmic problems with the **Standard Template Library (STL)**. Unlike the earlier modules, STL usage is not only allowed here, but required.

The module is built around three different kinds of problems:

* **historical data lookup** with validated input
* **expression evaluation** using stack logic
* **performance-oriented sorting** using a more advanced algorithm

A key rule of the module is that once a container is used in one exercise, it cannot be reused in the next one. In the last exercise, two different containers must be used and compared.

All exercises are written in **C++98** and compiled with strict flags.

---

## 🎯 Goals of the Module

Concepts covered in this module:

* Practical use of **STL containers**
* Choosing the right container depending on the problem
* Parsing and validating structured input
* File reading and error handling
* Associative lookup by key
* Stack-based computation
* Implementing a non-trivial sorting algorithm
* Comparing behavior and performance across containers
* Writing clean and defendable C++98 code

This module is not only about “using STL”, but about understanding **why a specific container fits a specific task**.

---

## 📦 Exercises Overview

### ex00 – Bitcoin Exchange

> Compute the value of a bitcoin amount on a given date.

**Goal:**
Create a program named **`btc`** that reads an input file where each line follows this format:

```text
date | value
```

The program uses a CSV database of historical bitcoin rates and outputs the corresponding result for each valid line.

**Main ideas:**

* parse a database file
* parse a user input file
* validate dates and values
* search for the exact date or the closest lower one
* print either the result or a clear error message

**Example usage:**

```bash
$ ./btc input.txt
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-03 => 1 = 0.3
2011-01-03 => 1.2 = 0.36
2011-01-09 => 1 = 0.32
Error: not a positive number.
Error: bad input => 2001-42-42
2012-01-11 => 1 = 7.1
Error: too large a number.
```

**Concepts practiced:**

* file parsing
* date validation
* numeric validation
* associative containers
* lower-bound style lookup logic

---

### ex01 – Reverse Polish Notation

> Evaluate a mathematical expression written in postfix form.

**Goal:**
Create a program named **`RPN`** that takes an inverted Polish expression as a single argument and computes the result.

The program must support:

```text
+ - / *
```

**Main ideas:**

* read the expression token by token
* push numbers into a container
* when an operator appears, pop the last two numbers
* apply the operation
* push the result back
* in the end, exactly one value must remain

**Example usage:**

```bash
$ ./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
42

$ ./RPN "7 7 * 7 -"
42

$ ./RPN "(1 + 1)"
Error
```

**Concepts practiced:**

* stack-based evaluation
* token parsing
* operator handling
* malformed expression detection
* using a container adapted to LIFO behavior

---

### ex02 – PmergeMe

> Sort a sequence of positive integers using merge-insert sort.

**Goal:**
Create a program named **`PmergeMe`** that sorts a sequence of positive integers using the **Ford–Johnson merge-insert sort algorithm**.

This exercise is the most algorithmic part of the module.

**Main ideas:**

* validate all input numbers
* group values into pairs
* separate smaller and bigger values inside each pair
* recursively sort the main chain
* insert pending elements in a carefully chosen order
* compare execution time for two different containers

**Example usage:**

```bash
$ ./PmergeMe 3 5 9 7 4
Before: 3 5 9 7 4
After: 3 4 5 7 9
Time to process a range of 5 elements with std::vector : ...
Time to process a range of 5 elements with std::deque : ...
```

**Concepts practiced:**

* advanced sorting logic
* recursive decomposition
* insertion strategy
* timing and benchmarking
* comparing containers under the same algorithm

---

## 🧠 Why These Containers?

One of the most important parts of this module is not only making the code work, but being able to justify the container choice during evaluation.

### ex00 – Associative container

For `BitcoinExchange`, an associative container such as **`std::map`** is a natural fit because:

* the data is stored as **key → value**
* the key is the date
* the value is the exchange rate
* the container keeps keys ordered
* ordered keys allow efficient lookup of the **closest lower date**

This is exactly the kind of problem where an ordered map is more useful than a simple sequence container.

### ex01 – Stack-oriented container

For `RPN`, the logic is fundamentally **Last-In, First-Out**:

* numbers are pushed as they appear
* operators consume the last two numbers
* the intermediate result is pushed back

So the most natural approach is a stack-like container, directly or indirectly.

### ex02 – Two sequence containers

For `PmergeMe`, the subject requires **two different containers**.
A common comparison is:

* **`std::vector`**
* **`std::deque`**

Why compare them?

* both support indexed access
* both are standard sequence containers
* their internal memory layout differs
* the same algorithm may behave differently depending on the container

This makes `ex02` both an algorithm exercise and a small performance study.

---

## 🔄 Ford–Johnson / Merge-Insert Sort – Quick Explanation

This is the trickiest part of the module, so here is a simple high-level summary.

The idea is not to sort everything in the most direct way.
Instead, the algorithm tries to reduce the number of comparisons by being smart about **pairing** and **insertion order**.

### Simplified intuition

1. Split the numbers into **pairs**.
2. Inside each pair, determine the **small** and **big** element.
3. Sort the sequence of the **big** elements.
4. Build a main sorted chain from those bigger elements.
5. Insert the smaller elements into the right positions.
6. Use a special insertion order to reduce comparisons.

### Why is it interesting?

Because the algorithm is designed to be comparison-efficient.
That is why the subject explicitly points to **Ford–Johnson** and not to a standard merge sort or insertion sort.

### What matters during evaluation

You usually do not need to reproduce a research-paper-level explanation.
But you should be able to explain:

* why you create pairs
* why each pair has a small and a big value
* why one chain is sorted first
* how the remaining values are inserted later
* why the algorithm is more subtle than a normal sort

A clean conceptual explanation is often more important than giving a mathematically perfect lecture.

---

## 🛠 Requirements

From the module rules:

* **Compiler**: `c++`
* **Flags**:

  * `-Wall -Wextra -Werror`
  * code must also compile with `-std=c++98`
* **No external libraries**
* **No C++11 or later**
* **No Boost**
* **No `printf` / `malloc` / `free` family**
* **No `using namespace ...`**
* **No `friend`** unless explicitly allowed

Additional Module 09 constraints:

* STL usage is mandatory
* once a container is used in one exercise, it cannot be reused later
* `ex02` must use **two different containers**
* function implementations in headers are forbidden except for templates
* headers must be self-sufficient and protected against double inclusion

---

## ▶️ Build & Run

Clone the repository and build each exercise separately.

```bash
git clone <this-repo-url>
cd cpp-module-09
```

### ex00 – Bitcoin Exchange

```bash
cd ex00
make
./btc input.txt
```

### ex01 – Reverse Polish Notation

```bash
cd ex01
make
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"
```

### ex02 – PmergeMe

```bash
cd ex02
make
./PmergeMe 3 5 9 7 4
```

### Large input example

```bash
./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "`
```

### macOS alternative

```bash
./PmergeMe `jot -r 3000 1 100000 | tr '\n' ' '`
```

---

## 📂 Repository Layout

```text
cpp-module-09/
├── ex00/
│   ├── Makefile
│   ├── main.cpp
│   ├── BitcoinExchange.hpp
│   ├── BitcoinExchange.cpp
│   └── data.csv
│
├── ex01/
│   ├── Makefile
│   ├── main.cpp
│   ├── RPN.hpp
│   └── RPN.cpp
│
└── ex02/
    ├── Makefile
    ├── main.cpp
    ├── PmergeMe.hpp
    └── PmergeMe.cpp
```

> Depending on the implementation, additional helper files may also be present.

---

## 🔍 Testing Tips

### ex00

Useful manual tests:

* invalid file path
* invalid header line
* invalid date format
* impossible dates
* negative values
* values larger than `1000`
* missing separator `|`
* date earlier than the first database entry
* date missing from database but still resolvable through the closest lower date

Examples:

```text
2012-13-01 | 2
2012-02-30 | 2
2001-42-42 | 1
2012-01-11 | -1
2012-01-11 | 2147483648
```

### ex01

Useful manual tests:

* valid simple expressions
* expression with too many operands
* expression with too many operators
* division by zero
* unsupported symbols
* empty input
* brackets, which should fail

Examples:

```bash
./RPN "1 2 +"
./RPN "8 0 /"
./RPN "1 2 3 +"
./RPN "+"
./RPN "(1 + 1)"
```

### ex02

Useful manual tests:

* already sorted input
* reverse sorted input
* one element only
* odd number of elements
* duplicates
* negative numbers
* non-numeric tokens
* large input with 3000 numbers

Examples:

```bash
./PmergeMe 1 2 3 4 5
./PmergeMe 5 4 3 2 1
./PmergeMe 42
./PmergeMe 3 5 9 7 4
./PmergeMe 1 2 2 3
./PmergeMe -1 2
```

---

## ❓ Common Evaluation Questions

Here are the kinds of questions that can come up during peer evaluation.

### General

**Why is STL allowed here but not in earlier modules?**
Because Modules 08 and 09 are the first modules where containers and algorithms from STL are explicitly allowed and expected.

**Why are you still using C++98?**
Because all 42 C++ modules in the Common Core are done under the C++98 constraint.

**Why avoid `using namespace std;`?**
Because the subject forbids it unless stated otherwise, and avoiding it also keeps names explicit.

### ex00

**Why use an ordered associative container?**
Because the problem is based on searching by date and, when necessary, finding the closest lower date.

**How do you handle a missing date?**
If the exact date is absent, the program uses the nearest lower date from the database.

**Why validate the date manually instead of trusting the string format?**
Because a string may look correct syntactically while still representing an invalid calendar date.

### ex01

**Why is a stack a good fit for RPN?**
Because the most recent operands are the first ones needed when an operator appears.

**What makes an expression invalid?**
Examples: too few operands, too many operands left at the end, invalid characters, or division by zero.

### ex02

**What is the idea behind Ford–Johnson?**
It sorts cleverly by grouping numbers, sorting a main chain first, and inserting the remaining elements in a smart order to reduce comparisons.

**Why compare two containers with the same algorithm?**
To observe how container properties influence performance even when the sorting logic is identical.

**Why not just use `std::sort`?**
Because the subject explicitly asks for merge-insert sort / Ford–Johnson.

---

## 🧾 42 Notes

* In the C++ modules, **Norminette is not enforced**.
* Even so, readable code is still extremely important during evaluation.
* The Makefile must still be clean and must not relink unnecessarily.
* Module 09 is one of the best places to show that you understand both **container choice** and **algorithmic reasoning**.
* For peer evaluation, being able to explain your data structures and edge cases is just as important as having correct output.

---

This module is a very good bridge between “basic C++ classes” and more practical algorithmic thinking with STL.

If you are another 42 student reading this repository, feel free to explore the code and compare approaches, but make sure you can explain every design choice yourself during evaluation. 🚀
