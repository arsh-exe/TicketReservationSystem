#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

// Status a passenger can be in - mirrors real Indian Railways PNR status
enum class Status {
    CONFIRMED,
    RAC,
    WAITING,
    CANCELLED
};

inline std::string statusToString(Status s) {
    switch (s) {
        case Status::CONFIRMED: return "CONFIRMED";
        case Status::RAC:       return "RAC";
        case Status::WAITING:   return "WAITING LIST";
        case Status::CANCELLED: return "CANCELLED";
    }
    return "UNKNOWN";
}

// ---------------------------------------------------------------
// This is the LINKED LIST NODE at the heart of the whole project.
// Every passenger booked on the train (confirmed, RAC, or waiting)
// is one of these nodes, linked to the next passenger via `next`.
// ---------------------------------------------------------------
struct Passenger {
    long long pnr;          // 10-digit unique PNR number (like real IRCTC)
    std::string name;
    int age;
    char gender;            // 'M' / 'F' / 'O'
    std::string berthPref;  // Lower / Middle / Upper / Side-Lower / Side-Upper
    int seatNumber;         // Assigned seat/berth number, -1 if waiting
    Status status;
    int bookingOrder;       // Order in which the ticket was booked (for FIFO/merge sort tie-break)

    Passenger* next;        // <-- the pointer that makes this a linked list

    Passenger(long long pnr_, std::string name_, int age_, char gender_,
              std::string berthPref_, int seatNumber_, Status status_, int bookingOrder_)
        : pnr(pnr_), name(std::move(name_)), age(age_), gender(gender_),
          berthPref(std::move(berthPref_)), seatNumber(seatNumber_),
          status(status_), bookingOrder(bookingOrder_), next(nullptr) {}
};

#endif // PASSENGER_H
