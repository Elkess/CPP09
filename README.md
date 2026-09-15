# CPP09 - STL exercises

This project contains the three exercises of the C++09 module, each focused on a different usage of the Standard Template Library (STL).

## Exercise 00: Bitcoin Exchange

### Goal
Read a file that contains lines in the form:

`date | value`

and convert the value using Bitcoin exchange rates from a CSV database.

### STL usage
This exercise uses `std::map`.

`std::map` is ideal here because:
- the database is keyed by date,
- dates must be read in sorted order,
- the requirement says that if the exact date is missing, we must use the nearest lower date.

The map automatically keeps keys ordered, and `lower_bound` / `previous` lookup gives the correct previous date efficiently.

### Parsing logic
The program reads the CSV file line by line. It ignores the header row (`date,exchange_rate`).
Each valid row is stored as:
- key: date string (`YYYY-MM-DD`)
- value: exchange rate as a double

For the input file, the parser does the following:
1. checks whether the line contains a pipe `|`,
2. verifies the format is `date | value`,
3. validates the date structure and calendar correctness,
4. verifies the numeric value is a valid double between `0` and `1000`,
5. finds the closest lower date in the map,
6. multiplies the input value by the rate and prints the result.

The error behavior follows the subject:
- bad input for a malformed line,
- negative value: `Error: not a positive number.`
- value above `1000`: `Error: too large a number.`
- invalid date: `Error: bad input => <date>`

## Exercise 01: Reverse Polish Notation

### Goal
Evaluate a Reverse Polish Notation expression passed as a single argument.

Example:

`./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"`

### STL usage
This exercise uses `std::stack`.

A stack is the natural container because RPN evaluation is fundamentally Last-In-First-Out:
- numbers are pushed onto the stack,
- operators pop two values, compute the result, and push the result back,
- the final stack must contain exactly one remaining value.

### Parsing logic
The expression is tokenized by whitespace:
- if the token is a number, it is pushed onto the stack,
- if the token is an operator (`+`, `-`, `*`, `/`), then two numbers are popped,
- the operation is executed in the correct order (`left` and `right`),
- the result is pushed back.

If the stack has not enough elements, the token sequence is invalid, or there are multiple values left at the end, the program prints:

`Error`

## Exercise 02: PmergeMe

### Goal
Sort a positive integer sequence using the Ford-Johnson merge-insertion algorithm, and compare the result using two STL containers:
- `std::vector`
- `std::list`

This exercise is not a plain merge sort. The subject specifically asks for the Ford-Johnson strategy described in "The Art of Computer Programming, Volume 3: Sorting and Searching" (merge insertion, pairs, Jacobsthal numbers, and binary insertion).

### Why this algorithm matters
Ford-Johnson is designed to reduce the number of comparisons when sorting large inputs. The main idea is:
1. create sorted pairs,
2. build a main ordered structure from the smaller elements of each pair,
3. insert the larger elements using carefully chosen gaps,
4. finish with a final insertion pass based on binary search.

This is why the subject explicitly mentions:
- the role of pairs,
- the Jacobsthal sequence,
- binary search.

### STL usage
This project uses two containers to compare their behavior and performance:

- `std::vector`: uses contiguous memory and supports efficient random access, which makes binary search and insertion easier to implement efficiently.
- `std::list`: stores nodes individually, which is useful for insertion-heavy algorithms, but it is slower for indexed access.

The point is not to choose the "best" container for the algorithm, but to show that the same algorithm can be implemented with two different STL structures and compared by time.

### The actual Ford-Johnson structure
The algorithm follows this structure:

1. Pairing step
   - the input is grouped into adjacent pairs,
   - each pair is sorted internally,
   - the smaller element of each pair becomes part of a smaller-element set,
   - the larger element becomes part of a larger-element set.

2. Main ordered set
   - the smallest element is placed first,
   - the remaining smaller elements are arranged in order,
   - the larger elements are then inserted at strategic positions.

3. Jacobsthal sequence
   - the insertion steps do not use simple 1, 2, 3, 4...
   - instead, they follow a Jacobsthal-like progression, which gives the best comparison pattern for merge-insertion sorting.
   - The reason is that the algorithm tries to minimize the number of comparisons by inserting at positions that are spread out in a controlled way.

4. Binary search insertion
   - once the current insertion position is chosen, the algorithm uses binary search to find the exact location inside the already-sorted structure.
   - This is the crucial difference from a naive insertion sort: we do not linearly scan the whole container; we jump to the correct range using the sorted order.

### Why the pair logic matters
Pairs are the foundation of Ford-Johnson. They reduce the problem to sorting smaller values and then inserting larger values in a structured order. This matters because the full algorithm is not just merge sort: it relies on reducing the data to a set of already-sorted substructures first.

### Why the Jacobsthal sequence matters
The Jacobsthal sequence is used because it produces the right spacing of insertion positions. In other words, the algorithm does not insert every element at every step; it inserts according to a computed sequence that keeps the total number of comparisons low. This is the key optimization that distinguishes Ford-Johnson from a basic insertion sort.

### Why binary search matters
After the algorithm selects the next insertion target, it must find where that value belongs among the already sorted elements. Binary search is ideal because it cuts the search space in half at each step, making the insertion efficient and consistent with the algorithm design.

### Parsing logic for PmergeMe
The program does the following:
1. reads all arguments,
2. validates that each argument is a positive integer,
3. stores the numbers in a `std::vector<unsigned int>`,
4. creates a second container copy using `std::list`,
5. sorts both independently,
6. prints the input sequence and the final sorted sequence,
7. prints the measured runtime for both containers.

### Output design
The subject requires a clear, explicit output:
- first line: initial unsorted sequence
- second line: sorted sequence
- third line: time for the first container
- final line: time for the second container

This is implemented so the comparison is easy to read and the performance difference between the two STL containers is visible immediately.

## Overall notes

The point of this module is not only to sort correctly, but to understand that STL containers are chosen for specific algorithmic behavior. In PmergeMe, the algorithmic structure is tied to the idea of merge-insertion, and the container choice affects how easily the algorithm can perform pairing, binary-search insertion, and measured timing comparisons.
