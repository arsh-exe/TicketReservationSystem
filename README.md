# 🚆 Train Ticket Reservation System (Indian Railways Model)

A DSA-course mini-project that simulates the **Indian Railways ticket booking
engine** — Confirmed / RAC / Waiting List booking, cancellation with automatic
promotion, and PNR status enquiry — built almost entirely on **Linked Lists**,
plus a few other classic data structures & algorithms layered on top.

There are two parts to this project:

| Folder      | What it is                                                              |
|-------------|--------------------------------------------------------------------------|
| `cpp/`      | The actual DSA implementation in C++ (console app) — this is the "real" project you submit / explain in viva. |
| `web_demo/` | A single self-contained `index.html` that re-implements the same logic in JavaScript so you can **demo it live in a browser** — no server, no install, just double-click it. |

---

## 1. How Indian Railways booking actually works (the model we follow)

When you book a train ticket on IRCTC:

1. If a **confirmed berth** is free → you get a seat number immediately.
2. If confirmed berths are full but the train still has **RAC (Reservation
   Against Cancellation)** quota → you get an RAC number (2 passengers share
   a side-lower berth).
3. If RAC is also full → you go to the **Waiting List (WL)**, in the exact
   order you booked (FIFO).
4. When someone **cancels** a confirmed ticket:
   - The **first person in RAC** is promoted to that confirmed berth.
   - The **first person in the Waiting List** is promoted to fill the RAC
     slot that just opened up.
   - This is a chain reaction — exactly like relinking nodes in a linked list.

This project implements *exactly* this chain-reaction logic using three
separate linked lists.

---

## 2. Data Structures & Algorithms used

| Concept | Where it's used | Why |
|---|---|---|
| **Singly Linked List** | `Confirmed` and `Waiting` passenger lists | Core structure — each passenger is a node with a `next` pointer, dynamic size, O(1) insert/delete once positioned |
| **Doubly Linked List (DLL)** *(web demo)* | `RAC` passenger list | Each RAC node also carries a `prev` pointer — fitting, since real RAC berths are literally shared by two paired passengers. Once a PNR is located, deleting it is an O(1) pointer rewire using the node's own `prev`/`next`, no re-traversal needed |
| **Queue (via Linked List)** | RAC list & Waiting list behave as FIFO queues | New passengers are enqueued at the tail; promotions dequeue from the head |
| **Stack (via Linked List)** | `cancellationHistory` | Supports an **Undo Last Cancellation** feature — LIFO by nature |
| **Linear Search** — O(n) | `searchByPNR()` | PNR status lookup across all three lists |
| **Merge Sort on Linked List** — O(n log n) | `sortWaitingListByAge()` | Re-orders the waiting list (e.g., senior-citizen priority) without converting to an array — classic linked-list merge sort |
| **Bubble Sort on Linked List** — O(n²) *(web demo only)* | Sort RAC by name | Swaps node *data* in place rather than relinking pointers — a useful contrast to merge sort |
| **Linked List Reversal** — O(n), O(1) space *(web demo only)* | Reverse Waiting List | Classic iterative 3-pointer (`prev` / `curr` / `next`) in-place reversal |
| **Floyd's Slow/Fast Pointers** — O(n) *(web demo only)* | Find Middle of Confirmed list | Tortoise-and-hare technique to find the middle node in one pass, no length pre-count needed |
| **Chain promotion (list splicing)** | `cancelTicket()` | Detach/re-attach nodes between the three lists in O(1) after the search |
| **Auto-incrementing PNR generation** | `generatePNR()` | Simple hashing/ID generation, 10-digit PNR like real IRCTC |

Time/space complexity notes are written as comments directly above each
function in `ReservationSystem.cpp` — useful for your viva/record file.

---

## 3. Project structure

```
train_reservation_system/
├── README.md
├── cpp/
│   ├── Passenger.h            # Node structure (the linked list node)
│   ├── ReservationSystem.h    # Class declaration
│   ├── ReservationSystem.cpp  # All linked list / stack / queue / sort logic
│   ├── main.cpp                # Menu-driven console interface
│   └── Makefile
└── web_demo/
    └── index.html              # Browser demo (open directly, no build step)
```

---

## 4. How to compile & run the C++ project

```bash
cd train_reservation_system/cpp
make            # compiles using g++
./reservation    # run the program (or reservation.exe on Windows)
```

Or without `make`:

```bash
g++ -std=c++17 main.cpp ReservationSystem.cpp -o reservation
./reservation
```

You'll get a menu:

```
====== INDIAN RAILWAYS - TICKET RESERVATION SYSTEM ======
1. Book a new ticket
2. Cancel a ticket (by PNR)
3. Search ticket status (by PNR)
4. Show Confirmed list
5. Show RAC list
6. Show Waiting list
7. Sort Waiting list by age (Merge Sort)
8. Undo last cancellation
9. Exit
```

---

## 5. How to run the web demo

No installation needed:

```bash
cd train_reservation_system/web_demo
# just open index.html in any browser, e.g.:
open index.html      # macOS
xdg-open index.html  # Linux
start index.html      # Windows
```

Or drag-and-drop `index.html` into any browser tab. It's fully self-contained
(HTML + CSS + JS in one file) and mirrors the exact same linked-list logic as
the C++ version — including live confirmed/RAC/waiting lists rendered as
connected "coaches" (visualizing the `next` pointer chain), PNR search,
cancellation with auto-promotion, and merge sort on the waiting list.

**Train Ticket Reservation System · Linked List / Queue / Stack / Sorting /
Reversal DSA demo · runs 100% locally in this browser tab, no server or
internet connection required (besides loading the two Google Fonts on first
open).**

Every passenger you see rendered on the tracks is a real linked-list node
with a `next` pointer, exactly like the C++ structs in `cpp/Passenger.h` —
the demo works for any train, any route, since the whole point is the data
structure underneath, not a specific journey.

This is what you'd project on a screen for your class demo/viva.

### What's interactive in the demo

- **Click any coach** to open a detail card for that passenger — name, age,
  berth, PNR, current status, and (to make the linked-list structure
  concrete) exactly what its own `next` pointer resolves to: another PNR, or
  `NULL` if it's the tail of that list.
- **Hover a coach** to see the same "next → ..." pointer info as a tooltip
  without opening anything.
- Each coach has a small **✕ quick-cancel** button in its corner for
  cancelling that one ticket in a single click (triggers the same chain
  promotion as cancelling from the sidebar).
- From the detail card you can also **copy the PNR** to your clipboard or
  jump straight to **"Track this PNR"**, which fills the search box and runs
  the lookup for you.
- When a cancellation causes a chain promotion, the promoted passenger's
  coach **pulses green** so you can visually follow RAC → Confirmed and
  Waiting → RAC hand-offs as they happen.
- **Algorithms animate step-by-step** instead of just snapping to the final
  result: Bubble Sort highlights each pair being compared and swapped in
  turn, Merge Sort plays out pass-by-pass (run size 1 → 2 → 4 → ...),
  Reverse Waiting List flips one pointer at a time and shows the "reversed
  so far" / "remaining" segments live, and Find Middle shows the slow and
  fast pointers racing through the Confirmed list until they meet. Buttons
  disable themselves mid-animation so runs can't overlap, and a status line
  under the buttons narrates what's happening.
- The departure board at the top has **live progress bars** per list and a
  one-line status ("6 confirmed · 2 on RAC · 1 waiting...") that updates
  after every action.
- **Keyboard shortcuts**: press Enter in the name/age fields to book, Enter
  in the PNR field to search, and Escape to close the detail card.
- Feedback for every action (booked, cancelled, promoted, sorted, reversed)
  appears as a small toast notification in the corner instead of a static
  log, so the interface stays uncluttered.

### Algorithms available as buttons in the demo

| Button | Algorithm | Complexity |
|---|---|---|
| Merge Sort WL by age | Iterative bottom-up merge sort (array-based, animated pass-by-pass in the demo; the pure recursive linked-list version — slow/fast split + merge — lives in `getMiddle`/`mergeSortByAge` and mirrors `cpp/ReservationSystem.cpp`) | O(n log n) |
| Bubble Sort RAC by name | Bubble sort that swaps node *data*, not links | O(n²) |
| Reverse Waiting List | Iterative 3-pointer linked-list reversal | O(n), O(1) space |
| Find Middle (Confirmed) | Floyd's slow/fast (tortoise-hare) pointer technique | O(n) |
| Undo last cancel | Stack pop + re-book | O(1) amortized |

---

## 6. Suggested talking points for your viva

- Why a **linked list** instead of an array for passenger records? → dynamic
  size (you don't know train occupancy in advance), O(1) insertion/removal
  once a node is located, no shifting needed on cancellation.
- Why **three separate linked lists** instead of one list with a status
  field? → mirrors real-world separation of concerns and makes promotion an
  O(1) "unlink from list A, link to list B" operation instead of an O(n)
  filter.
- Why **merge sort** for the waiting list and not quicksort? → merge sort is
  the natural choice for linked lists (no random access needed, stable,
  guaranteed O(n log n), unlike quicksort's O(n²) worst case and its need for
  random access for good pivots).
- Why a **stack** for cancellation history? → undo is inherently
  last-in-first-out.
