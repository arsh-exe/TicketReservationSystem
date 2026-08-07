#include "ReservationSystem.h"
#include <iostream>
#include <iomanip>

ReservationSystem::ReservationSystem(int confirmedCapacity, int racCapacity)
    : confirmedHead(nullptr), confirmedTail(nullptr),
      racHead(nullptr), racTail(nullptr),
      waitHead(nullptr), waitTail(nullptr),
      historyTop(nullptr),
      confirmedCount(0), racCount(0), waitCount(0),
      CONFIRMED_CAPACITY(confirmedCapacity), RAC_CAPACITY(racCapacity),
      pnrCounter(2500000000LL), bookingOrderCounter(0) {}

ReservationSystem::~ReservationSystem() {
    // free all nodes in all three lists + the history stack
    Passenger* lists[3] = {confirmedHead, racHead, waitHead};
    for (Passenger* head : lists) {
        while (head) {
            Passenger* tmp = head;
            head = head->next;
            delete tmp;
        }
    }
    while (historyTop) {
        CancelRecord* tmp = historyTop;
        historyTop = historyTop->next;
        delete tmp;
    }
}

long long ReservationSystem::generatePNR() {
    return pnrCounter++;
}

// ---------------------------------------------------------------
// appendNode: O(1) insertion at the tail of a linked list.
// We keep a tail pointer for every list specifically so that
// booking (enqueue) never needs an O(n) traversal.
// ---------------------------------------------------------------
void ReservationSystem::appendNode(Passenger*& head, Passenger*& tail, Passenger* node) {
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
}

// ---------------------------------------------------------------
// detachHead: O(1) removal from the head of a linked list.
// Used when we promote the first RAC passenger to Confirmed, or
// the first Waiting passenger to RAC (FIFO / queue behaviour).
// ---------------------------------------------------------------
Passenger* ReservationSystem::detachHead(Passenger*& head, Passenger*& tail) {
    if (!head) return nullptr;
    Passenger* node = head;
    head = head->next;
    if (!head) tail = nullptr;
    node->next = nullptr;
    return node;
}

// ---------------------------------------------------------------
// renumberList: O(n) traversal that re-assigns seat/RAC/WL numbers
// 1..k in order. Needed after a node is removed from the middle of
// a list (e.g. a RAC passenger cancels), so the remaining passengers'
// displayed numbers stay contiguous - exactly like a real chart.
// ---------------------------------------------------------------
void ReservationSystem::renumberList(Passenger* head) {
    int pos = 1;
    for (Passenger* p = head; p; p = p->next) {
        p->seatNumber = pos++;
    }
}

// ---------------------------------------------------------------
// bookTicket: decides Confirmed vs RAC vs Waiting exactly like
// IRCTC does, then appends the new node (O(1) thanks to tail ptr).
// ---------------------------------------------------------------
void ReservationSystem::bookTicket(const std::string& name, int age, char gender, const std::string& berthPref) {
    long long pnr = generatePNR();
    int order = bookingOrderCounter++;

    if (confirmedCount < CONFIRMED_CAPACITY) {
        int seat = confirmedCount + 1;
        Passenger* node = new Passenger(pnr, name, age, gender, berthPref, seat, Status::CONFIRMED, order);
        appendNode(confirmedHead, confirmedTail, node);
        confirmedCount++;
        std::cout << "\n[BOOKED] PNR: " << pnr << " | Status: CONFIRMED | Seat No: " << seat << "\n";
    } else if (racCount < RAC_CAPACITY) {
        int racNo = racCount + 1;
        Passenger* node = new Passenger(pnr, name, age, gender, berthPref, racNo, Status::RAC, order);
        appendNode(racHead, racTail, node);
        racCount++;
        std::cout << "\n[BOOKED] PNR: " << pnr << " | Status: RAC " << racNo << "\n";
    } else {
        waitCount++;
        Passenger* node = new Passenger(pnr, name, age, gender, berthPref, waitCount, Status::WAITING, order);
        appendNode(waitHead, waitTail, node);
        std::cout << "\n[BOOKED] PNR: " << pnr << " | Status: WAITING LIST (WL" << waitCount << ")\n";
    }
}

// ---------------------------------------------------------------
// cancelTicket: O(n) to locate the PNR (linear search across the
// three lists), then O(1) relinking to splice the node out.
// If a Confirmed ticket is cancelled, we trigger the classic
// Indian Railways CHAIN PROMOTION:
//     RAC head -> becomes Confirmed
//     Waiting head -> becomes RAC
// which is just two O(1) detach + O(1) append operations.
// ---------------------------------------------------------------
bool ReservationSystem::cancelTicket(long long pnr) {
    Passenger* prev = nullptr;
    Passenger* curr = confirmedHead;

    // search Confirmed list
    while (curr && curr->pnr != pnr) { prev = curr; curr = curr->next; }
    if (curr) {
        // unlink `curr` from confirmed list
        if (prev) prev->next = curr->next; else confirmedHead = curr->next;
        if (curr == confirmedTail) confirmedTail = prev;
        curr->next = nullptr;
        confirmedCount--;

        // push to undo stack BEFORE we mutate/re-seat anyone
        CancelRecord* rec = new CancelRecord(*curr);
        rec->next = historyTop;
        historyTop = rec;

        std::cout << "\n[CANCELLED] PNR " << pnr << " (" << curr->name << ") - was CONFIRMED, seat freed.\n";
        delete curr;
        renumberList(confirmedHead);

        // --- Chain promotion #1: RAC head -> Confirmed ---
        if (racHead) {
            Passenger* promoted = detachHead(racHead, racTail);
            racCount--;
            promoted->status = Status::CONFIRMED;
            appendNode(confirmedHead, confirmedTail, promoted);
            confirmedCount++;
            renumberList(confirmedHead);
            renumberList(racHead);
            std::cout << "[PROMOTED] PNR " << promoted->pnr << " (" << promoted->name
                      << ") : RAC -> CONFIRMED, seat " << promoted->seatNumber << "\n";

            // --- Chain promotion #2: Waiting head -> RAC ---
            if (waitHead) {
                Passenger* promotedWL = detachHead(waitHead, waitTail);
                waitCount--;
                promotedWL->status = Status::RAC;
                appendNode(racHead, racTail, promotedWL);
                racCount++;
                renumberList(racHead);
                renumberList(waitHead);
                std::cout << "[PROMOTED] PNR " << promotedWL->pnr << " (" << promotedWL->name
                          << ") : WAITING -> RAC " << promotedWL->seatNumber << "\n";
            }
        }
        return true;
    }

    // search RAC list
    prev = nullptr; curr = racHead;
    while (curr && curr->pnr != pnr) { prev = curr; curr = curr->next; }
    if (curr) {
        if (prev) prev->next = curr->next; else racHead = curr->next;
        if (curr == racTail) racTail = prev;
        curr->next = nullptr;
        racCount--;

        CancelRecord* rec = new CancelRecord(*curr);
        rec->next = historyTop;
        historyTop = rec;

        std::cout << "\n[CANCELLED] PNR " << pnr << " (" << curr->name << ") - was RAC.\n";
        delete curr;
        renumberList(racHead);

        if (waitHead) {
            Passenger* promotedWL = detachHead(waitHead, waitTail);
            waitCount--;
            promotedWL->status = Status::RAC;
            appendNode(racHead, racTail, promotedWL);
            racCount++;
            renumberList(racHead);
            renumberList(waitHead);
            std::cout << "[PROMOTED] PNR " << promotedWL->pnr << " (" << promotedWL->name
                      << ") : WAITING -> RAC " << promotedWL->seatNumber << "\n";
        }
        return true;
    }

    // search Waiting list
    prev = nullptr; curr = waitHead;
    while (curr && curr->pnr != pnr) { prev = curr; curr = curr->next; }
    if (curr) {
        if (prev) prev->next = curr->next; else waitHead = curr->next;
        if (curr == waitTail) waitTail = prev;
        curr->next = nullptr;
        waitCount--;

        CancelRecord* rec = new CancelRecord(*curr);
        rec->next = historyTop;
        historyTop = rec;

        std::cout << "\n[CANCELLED] PNR " << pnr << " (" << curr->name << ") - was WAITING LIST.\n";
        delete curr;
        renumberList(waitHead);
        return true;
    }

    std::cout << "\n[ERROR] No booking found with PNR " << pnr << "\n";
    return false;
}

// ---------------------------------------------------------------
// undoLastCancellation: pops the STACK (LIFO) of cancelled tickets
// and re-books that exact passenger, respecting current availability.
// ---------------------------------------------------------------
bool ReservationSystem::undoLastCancellation() {
    if (!historyTop) {
        std::cout << "\n[UNDO] Nothing to undo.\n";
        return false;
    }
    CancelRecord* rec = historyTop;
    historyTop = historyTop->next;

    std::cout << "\n[UNDO] Restoring PNR " << rec->data.pnr << " (" << rec->data.name << ")...\n";
    bookTicket(rec->data.name, rec->data.age, rec->data.gender, rec->data.berthPref);

    delete rec;
    return true;
}

// ---------------------------------------------------------------
// searchByPNR: LINEAR SEARCH, O(n), across all three linked lists.
// (A real system would hash PNR -> node for O(1); we keep it O(n)
// on purpose here since traversal is the point of the DSA exercise.)
// ---------------------------------------------------------------
Passenger* ReservationSystem::searchByPNR(long long pnr, Status* foundIn) {
    for (Passenger* p = confirmedHead; p; p = p->next) {
        if (p->pnr == pnr) { if (foundIn) *foundIn = Status::CONFIRMED; return p; }
    }
    for (Passenger* p = racHead; p; p = p->next) {
        if (p->pnr == pnr) { if (foundIn) *foundIn = Status::RAC; return p; }
    }
    for (Passenger* p = waitHead; p; p = p->next) {
        if (p->pnr == pnr) { if (foundIn) *foundIn = Status::WAITING; return p; }
    }
    return nullptr;
}

void ReservationSystem::displayList(Status which) const {
    Passenger* head = nullptr;
    std::string label;
    switch (which) {
        case Status::CONFIRMED: head = confirmedHead; label = "CONFIRMED"; break;
        case Status::RAC:       head = racHead;       label = "RAC";       break;
        case Status::WAITING:   head = waitHead;       label = "WAITING LIST"; break;
        default: return;
    }
    std::cout << "\n--- " << label << " LIST ---\n";
    if (!head) { std::cout << "  (empty)\n"; return; }
    std::cout << std::left << std::setw(14) << "PNR" << std::setw(20) << "Name"
              << std::setw(6) << "Age" << std::setw(4) << "Sex" << std::setw(14) << "Berth Pref"
              << "Seat/No\n";
    for (Passenger* p = head; p; p = p->next) {
        std::cout << std::left << std::setw(14) << p->pnr << std::setw(20) << p->name
                  << std::setw(6) << p->age << std::setw(4) << p->gender
                  << std::setw(14) << p->berthPref << p->seatNumber << "\n";
    }
}

void ReservationSystem::displayAll() const {
    displayList(Status::CONFIRMED);
    displayList(Status::RAC);
    displayList(Status::WAITING);
}

// =================================================================
//                MERGE SORT ON A LINKED LIST (by age)
// This is the classic DSA algorithm: split the list into halves
// using the slow/fast pointer technique, recursively sort each
// half, then merge two sorted linked lists. O(n log n), and unlike
// quicksort it does NOT need random access, so it's the natural
// sorting algorithm for linked lists.
// =================================================================
Passenger* ReservationSystem::getMiddle(Passenger* head) {
    if (!head) return head;
    Passenger* slow = head;
    Passenger* fast = head->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow; // slow lands on the node just before the second half
}

Passenger* ReservationSystem::mergeTwoSortedByAge(Passenger* a, Passenger* b) {
    if (!a) return b;
    if (!b) return a;
    if (a->age <= b->age) {
        a->next = mergeTwoSortedByAge(a->next, b);
        return a;
    } else {
        b->next = mergeTwoSortedByAge(a, b->next);
        return b;
    }
}

Passenger* ReservationSystem::mergeSortByAge(Passenger* head) {
    if (!head || !head->next) return head; // base case: 0 or 1 node

    Passenger* mid = getMiddle(head);
    Passenger* secondHalf = mid->next;
    mid->next = nullptr; // split into two halves

    Passenger* left = mergeSortByAge(head);
    Passenger* right = mergeSortByAge(secondHalf);

    return mergeTwoSortedByAge(left, right);
}

void ReservationSystem::sortWaitingListByAge() {
    waitHead = mergeSortByAge(waitHead);
    // re-find tail and re-number WL positions after sort
    waitTail = waitHead;
    int pos = 1;
    while (waitTail) {
        waitTail->seatNumber = pos++;
        if (!waitTail->next) break;
        waitTail = waitTail->next;
    }
    std::cout << "\n[SORTED] Waiting list re-ordered by age (youngest to oldest) using Merge Sort.\n";
}
