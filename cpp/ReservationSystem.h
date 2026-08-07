#ifndef RESERVATION_SYSTEM_H
#define RESERVATION_SYSTEM_H

#include "Passenger.h"
#include <string>

// ---------------------------------------------------------------
// A tiny STACK (implemented as a linked list) used to store
// cancellation history so we can support "Undo last cancellation".
// Stacks are LIFO - perfect fit for an undo feature.
// ---------------------------------------------------------------
struct CancelRecord {
    Passenger data;
    CancelRecord* next;
    CancelRecord(const Passenger& p) : data(p), next(nullptr) {}
};

class ReservationSystem {
private:
    // Three independent linked lists = three passenger queues
    Passenger* confirmedHead;
    Passenger* confirmedTail;
    Passenger* racHead;
    Passenger* racTail;
    Passenger* waitHead;
    Passenger* waitTail;

    CancelRecord* historyTop;   // stack top for undo

    int confirmedCount;
    int racCount;
    int waitCount;

    const int CONFIRMED_CAPACITY;   // e.g. 20 confirmed berths
    const int RAC_CAPACITY;         // e.g. 6 RAC slots (3 side-lower berths x 2)

    long long pnrCounter;   // used to auto-generate unique 10-digit PNRs
    int bookingOrderCounter;

    // internal helpers
    void appendNode(Passenger*& head, Passenger*& tail, Passenger* node);
    Passenger* detachHead(Passenger*& head, Passenger*& tail);
    Passenger* mergeSortByAge(Passenger* head);      // recursive merge sort
    Passenger* mergeTwoSortedByAge(Passenger* a, Passenger* b);
    Passenger* getMiddle(Passenger* head);
    void renumberList(Passenger* head);              // keeps seat/RAC/WL numbers sequential after removals

public:
    ReservationSystem(int confirmedCapacity = 20, int racCapacity = 6);
    ~ReservationSystem();

    long long generatePNR();

    // Core operations
    void bookTicket(const std::string& name, int age, char gender, const std::string& berthPref);
    bool cancelTicket(long long pnr);
    bool undoLastCancellation();

    // Search - O(n) linear search across the three lists
    Passenger* searchByPNR(long long pnr, Status* foundIn = nullptr);

    // Display
    void displayList(Status which) const;
    void displayAll() const;

    // DSA showcase: merge sort the waiting list by age
    void sortWaitingListByAge();
};

#endif // RESERVATION_SYSTEM_H
