# 🚆 Train Reservation System

This project simulates a small Indian Railways-style ticket booking system using linked-list-based data structures. It is designed as a Data Structures and Algorithms mini-project: passengers are stored as nodes, cancellations trigger automatic promotion between lists, and ticket status can be searched quickly by PNR.

The repository contains two versions:

- `cpp/` — the main C++ console application
- `web_demo/` — a browser-based visual demo that demonstrates the same concepts in JavaScript

---

## Project purpose

The system models a train with three categories of passengers:

1. Confirmed passengers
2. RAC passengers
3. Waiting List passengers

The booking policy behaves like a railway system:

- If confirmed berths are available, a passenger is assigned a confirmed seat.
- If confirmed seats are full but RAC quota exists, the passenger is placed in RAC.
- If both confirmed and RAC are full, the passenger is placed on the Waiting List.
- When a confirmed ticket is cancelled:
  - the first RAC passenger is promoted to confirmed,
  - the first waiting passenger is promoted to RAC.

This is the heart of the project and is implemented using linked-list relinking, which makes the promotion logic easy to understand and excellent for DSA demonstrations.

---

## Key features

- Booking system with status-based allocation
- PNR-based ticket lookup
- Ticket cancellation with chain promotion
- Waiting list sorting by age using merge sort
- Undo of the last cancellation using a stack
- Console-based menu-driven interface
- Browser-based visual demo for presentation and viva

---

## Technologies used

- C++17
- GCC / g++ compiler
- Standard C++ data structures and linked-list logic
- HTML, CSS, JavaScript for the browser demo
- Basic procedural logic and OOP-style class design

---

## File structure

```text
train_reservation_system/
├── README.md
├── PROJECT_NOTES.md
├── cpp/
│   ├── main.cpp
│   ├── Passenger.h
│   ├── ReservationSystem.h
│   ├── ReservationSystem.cpp
│   └── Makefile
└── web_demo/
    └── index.html
```

---

## Core program design

### 1. Passenger node

The file `cpp/Passenger.h` defines the `Passenger` structure.

Each passenger is a node with fields like:

- `pnr` — unique booking number
- `name` — passenger name
- `age` — age
- `gender` — M/F/O
- `berthPref` — preferred berth
- `seatNumber` — assigned seat number or list order
- `status` — CONFIRMED, RAC, or WAITING
- `bookingOrder` — order of booking for FIFO behavior
- `next` — pointer to the next passenger node

This is the central linked-list node used throughout the project.

### 2. Reservation system class

The file `cpp/ReservationSystem.h` contains the `ReservationSystem` class and its private members:

- `confirmedHead`, `confirmedTail`
- `racHead`, `racTail`
- `waitHead`, `waitTail`
- `historyTop` — stack used for undo operations
- `confirmedCount`, `racCount`, `waitCount`
- `CONFIRMED_CAPACITY`, `RAC_CAPACITY`
- `pnrCounter` — generates unique PNR numbers

This design keeps all three categories in separate linked lists, which makes booking and cancellation behave much like a real railway reservation system.

### 3. Booking logic

The `bookTicket()` method in `ReservationSystem.cpp` decides where the passenger goes:

- If confirmed capacity is available, the passenger is added to the Confirmed list.
- Else if RAC capacity is available, they go to the RAC list.
- Else they are added to the Waiting List.

Each booking appends at the tail of the relevant list, giving queue-like insertion behavior.

### 4. Cancellation and chain promotion

The `cancelTicket()` method does the most important work.

It searches for the PNR across:

- Confirmed list
- RAC list
- Waiting List

When a passenger is removed:

- If a confirmed passenger cancels, the head of RAC is promoted to Confirmed.
- Then the head of Waiting List is promoted to RAC.
- If a RAC passenger cancels, the head of Waiting List is promoted to RAC.
- If a waiting passenger cancels, nothing else is promoted.

This is implemented by detaching nodes from one list and appending them into another, which is the cleanest way to model the railway promotion chain.

### 5. Search

`searchByPNR()` uses linear search across the three linked lists.

This is intentionally O(n) because the assignment focuses on demonstrating linked-list traversal and algorithmic thinking. A real production system would likely use a hash map or direct indexing for O(1) lookup.

### 6. Undo system

The code uses a custom stack called `CancelRecord`.

- Each cancellation creates a record with the removed passenger's data.
- The record is pushed onto `historyTop`.
- `undoLastCancellation()` pops the last record and re-books the same passenger.

This is a classic LIFO stack design.

### 7. Sorting logic

`sortWaitingListByAge()` calls `mergeSortByAge()` on the waiting list.

This is a linked-list merge sort using:

- slow/fast pointer split
- recursive sorting of halves
- merging of sorted halves

This is a perfect demonstration of linked-list sorting without converting the list into an array.

---

## Console application flow

The program in `cpp/main.cpp` creates a `ReservationSystem` and runs a menu loop.

Available menu options:

```text
1. Book a new ticket
2. Cancel a ticket (by PNR)
3. Search ticket status (by PNR)
4. Show Confirmed list
5. Show RAC list
6. Show Waiting list
7. Sort Waiting list by age (Merge Sort)
8. Undo last cancellation
9. Show everything
0. Exit
```

Each action calls the corresponding method in `ReservationSystem` and displays results to the terminal.

---

## How to compile and run the C++ project

From the repository root:

```bash
cd cpp
make
./reservation
```

If `make` is unavailable, compile manually:

```bash
g++ -std=c++17 main.cpp ReservationSystem.cpp -o reservation
./reservation
```

On Windows, the output executable may be `reservation.exe`.

---

## How to run the browser demo

Open the HTML file directly in a browser:

```bash
cd web_demo
start index.html
```

or open the file manually from the file browser.

The browser version is a visual representation of the same logic; it displays passengers as train coaches and allows booking, cancellation, search, sorting, and promotion to be visualized in a more interactive format.

---

## DSA concepts used

| Concept | Where used | Why it matters |
|---|---|---|
| Singly Linked List | Confirmed and Waiting list | Dynamic insertion and deletion |
| Tail pointers | Booking and appending | O(1) insertion at end |
| FIFO queue behavior | RAC and Waiting Logic | Simulates actual reservation order |
| Stack | Undo cancellation | LIFO behavior for undo |
| Linear search | PNR lookup | Simple O(n) search across lists |
| Merge sort | Waiting list sorting | Efficient linked-list sorting |
| Pointer relinking | Cancellation + promotion | Moves nodes between lists |

---

## Notes for understanding the project

This project is not meant to be a production-grade railway system with authentication or a database. It is intentionally built to demonstrate the practical use of linked lists and related algorithms in a system that behaves similarly to real booking rules.

The value of the project is in the logic:

- each passenger is a node,
- each category is a linked list,
- promotions are pointer operations,
- cancellation history is a stack,
- sorting is a linked-list algorithm exercise.

---

## Possible improvements

If you want to extend this project later, a few useful upgrades could be:

- add a database or file storage layer,
- support passenger seat preference matching more realistically,
- add a search by name or age,
- implement a more advanced system for berth preference management,
- add a GUI or web API version for full application behavior.

---

## Summary

This repository demonstrates how core DSA ideas can be used to model a real-world train reservation system. The main challenge is not just storing passenger data, but correctly handling the linked-list transitions when passengers move between Confirmed, RAC, and Waiting states.

That logic is the main educational value of the project.
