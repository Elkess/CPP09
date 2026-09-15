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
Sort a sequence of positive integers using a merge-insert style algorithm and compare two STL containers:
- `std::vector`
- `std::list`

The subject requires a merge-insert algorithm and also asks for explicit timing output for both containers.

### STL usage
This project uses two distinct containers to compare performance and behavior:

- `std::vector`: random-access container, better for contiguous memory access and lower overhead,
- `std::list`: linked-list container, good for frequent insertions and removals, but slower to access by index.

The algorithm is implemented using merge-sort style splitting and insertion into ordered ranges.

### Why these containers
The exercise explicitly asks to use at least two containers and to compare their sorting cost. Using `vector` and `list` makes the comparison meaningful because they have different internal structure and performance trade-offs.

### Parsing logic
The program reads all arguments as positive integers, validates that they are digits only, and stores them in a `std::vector<unsigned int>`.
A copy is made for the second container, and both are sorted independently.

The logic keeps the output simple and readable:
- first line: input sequence,
- second line: sorted sequence,
- then timing information for both containers.

## Overall notes

The implementations were kept intentionally explicit and readable so the parsing logic is easy to follow, and each exercise uses the STL container that best matches the problem constraints.
