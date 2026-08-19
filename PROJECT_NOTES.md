# Train Reservation System - Project Notes

## 1. Purpose of the project

This project is a Data Structures and Algorithms mini-project that simulates an Indian Railways-style ticket reservation system.

It focuses on demonstrating how linked lists and related algorithms can model real-world operations like:

- booking a ticket,
- assigning a passenger to confirmed / RAC / waiting status,
- cancelling a ticket,
- promoting passengers automatically,
- searching by PNR,
- sorting waiting passengers by age,
- undoing the last cancellation.

---

## 2. What the project contains

### C++ console application

Location: `cpp/`

This is the main implementation of the system. It contains:

- `main.cpp` — menu-driven user interface
- `Passenger.h` — node definition and status enum
- `ReservationSystem.h` — class declaration and data members
- `ReservationSystem.cpp` — all reservation logic
- `Makefile` — compile command support

### Browser demo

Location: `web_demo/index.html`

This version provides a visual interface that shows the same system using train coaches, live board updates, and interactive passenger cards.

It is useful for presentations, demos, and explaining how the data structure behaves visually.

---

## 3. Core idea behind the system

The system divides passengers into three separate linked lists:

1. Confirmed List
2. RAC List
3. Waiting List

This is important because bookings work on the same principle as a real booking system:

- Confirmed seats are assigned first.
- If the train is full, RAC is used.
- If both are full, the passenger goes to Waiting.

When a cancellation happens, the railway-style promotion chain is triggered:

- first RAC passenger is promoted to confirmed,
- first waiting passenger is promoted to RAC.

This creates a chain reaction that is easy to represent using linked-list pointer changes.

---

## 4. Data structures used

### Linked list

The main data structure used is a singly linked list.

Each passenger is stored as a node with a `next` pointer. This allows:

- booking at the tail,
- inserting at the end in O(1) with tail pointer,
- removing from the front or middle by relinking pointers,
- easy promotion between groups.

### Tail pointers

Each list keeps a head and tail pointer so the system can append new passengers faster.

This avoids traversing the whole list every time a new booking is added.

### Stack for undo

The `CancelRecord` structure stores the removed passenger data in a stack-like linked list.

This allows the system to undo the last cancellation.

This is a classic LIFO structure.

### Linear search

The `searchByPNR()` function searches across the three queues by traversing each list linearly.

This is done intentionally to illustrate algorithmic behavior and complexity.

---

## 5. Why the structure matters

The project shows that data structures are not just for storing data; they directly affect how the system behaves.

Examples:

- A linked list is suitable because tickets are dynamically added and removed.
- Separate lists make waiting and promotion realistic.
- A stack fits the undo feature.
- Merge sort is well suited to linked lists because it does not need random access.

---

## 6. Class and file responsibilities

### `Passenger.h`

Defines the passenger node:

- `pnr`
- `name`
- `age`
- `gender`
- `berthPref`
- `seatNumber`
- `status`
- `bookingOrder`
- `next`

This file is the heart of the project because every passenger is represented here.

### `ReservationSystem.h`

Contains the complete system class and state information:

- list heads and tails
- booking counts
- history stack top
- capacities
- PNR generator
- helper functions

### `ReservationSystem.cpp`

Contains the actual operational logic:

- `bookTicket()`
- `cancelTicket()`
- `searchByPNR()`
- `displayList()`
- `displayAll()`
- `sortWaitingListByAge()`
- `undoLastCancellation()`

This is the main logic file where the data structure operations happen.

### `main.cpp`

Provides a menu-based interface for experimenting with the system.

It lets the user:

- create bookings,
- cancel by PNR,
- search by PNR,
- look at lists,
- sort waiting list,
- undo cancellations.

---

## 7. Booking workflow

When a user books a ticket:

1. A new PNR is generated.
2. The system checks confirmed capacity.
3. If full, it checks RAC capacity.
4. If full, it adds the ticket to Waiting List.
5. The passenger is inserted at the end of the active list.

This makes the process behave like a queue.

---

## 8. Cancellation workflow

When a user cancels a ticket:

1. The system searches the Confirmed, RAC, and Waiting lists.
2. The matching passenger node is removed from its current list.
3. The system updates the list numbering.
4. If the cancelled passenger was confirmed, the first RAC passenger is promoted.
5. Then the first waiting passenger is promoted to RAC.
6. The deleted ticket is then pushed to the undo stack.

This is the most important demonstration of pointer manipulation in the project.

---

## 9. Sorting workflow

The waiting list is sorted by age using merge sort.

The algorithm works as follows:

1. Split the list into two halves.
2. Recursively sort each half.
3. Merge both sorted halves back together.

This is a classic linked-list merge sort and it demonstrates that sorting can be done without array conversion.

---

## 10. Undo workflow

When a cancellation happens, the cancelled passenger is saved in the history stack.

Later, the user can choose:

- Undo last cancellation

The function pops the stack and calls `bookTicket()` again using the saved passenger data.

This is a perfect example of stack behavior in a real system.

---

## 11. Browser demo structure

The `web_demo/index.html` file creates a visual train board with:

- confirmed coaches,
- RAC coaches,
- waiting coaches,
- dynamic count updates,
- clickable passenger cards,
- live status messages,
- quick-cancel actions,
- algorithm animations.

This version is designed to make linked-list operations understandable to an audience without code-level details.

---

## 12. Why this project is useful

This project is useful because it bridges theory and application.

It demonstrates how abstract DSA concepts are used in a realistic simulation:

- passenger records as nodes,
- lists as booking categories,
- queue-like booking order,
- stack-like undo behavior,
- sorting by linked-list merge sort,
- pointer manipulation during cancellation and promotion.

---

## 13. What is not included

This project is intentionally simple and is not a complete real-world railway system.

It does not include:

- database storage,
- user authentication,
- payment processing,
- train schedules,
- route management,
- real-time network access.

The emphasis is on algorithmic logic rather than production implementation.

---

## 14. Summary

The Train Reservation System is a practical DSA project that uses linked lists to simulate the behavior of railway ticket booking.

Its main strengths are:

- clear conceptual design,
- direct use of linked-list operations,
- realistic booking promotion logic,
- classroom-friendly lecture value,
- easy demonstration in both console and browser form.

This project is best understood as a visual and conceptual example of how real-world system rules can be implemented using standard data structures.
