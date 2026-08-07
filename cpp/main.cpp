#include "ReservationSystem.h"
#include <iostream>
#include <limits>

void pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

int main() {
    // Small train for demo purposes: 5 confirmed berths, 2 RAC slots
    // (change these numbers to simulate a bigger train)
    ReservationSystem system(/*confirmedCapacity=*/5, /*racCapacity=*/2);

    std::cout << "=====================================================\n";
    std::cout << "   INDIAN RAILWAYS - TRAIN TICKET RESERVATION SYSTEM \n";
    std::cout << "   (Linked List based | DSA Mini Project)            \n";
    std::cout << "=====================================================\n";

    while (true) {
        std::cout << "\n------------------ MAIN MENU ------------------\n";
        std::cout << "1. Book a new ticket\n";
        std::cout << "2. Cancel a ticket (by PNR)\n";
        std::cout << "3. Search ticket status (by PNR)\n";
        std::cout << "4. Show Confirmed list\n";
        std::cout << "5. Show RAC list\n";
        std::cout << "6. Show Waiting list\n";
        std::cout << "7. Sort Waiting list by age (Merge Sort)\n";
        std::cout << "8. Undo last cancellation\n";
        std::cout << "9. Show everything\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter choice: ";

        int choice;
        if (!(std::cin >> choice)) break;

        if (choice == 0) {
            std::cout << "\nThank you for using Indian Railways Reservation System. Have a safe journey!\n";
            break;
        }

        switch (choice) {
            case 1: {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::string name, berth;
                int age; char gender;
                std::cout << "Passenger name: "; std::getline(std::cin, name);
                std::cout << "Age: "; std::cin >> age;
                std::cout << "Gender (M/F/O): "; std::cin >> gender;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Berth preference (Lower/Middle/Upper/Side-Lower/Side-Upper): ";
                std::getline(std::cin, berth);
                system.bookTicket(name, age, gender, berth);
                break;
            }
            case 2: {
                long long pnr;
                std::cout << "Enter PNR to cancel: "; std::cin >> pnr;
                system.cancelTicket(pnr);
                break;
            }
            case 3: {
                long long pnr;
                std::cout << "Enter PNR to search: "; std::cin >> pnr;
                Status s;
                Passenger* p = system.searchByPNR(pnr, &s);
                if (p) {
                    std::cout << "\nPNR " << p->pnr << " | " << p->name << " | Age " << p->age
                              << " | Status: " << statusToString(s) << " | No: " << p->seatNumber << "\n";
                } else {
                    std::cout << "\nNo record found for PNR " << pnr << "\n";
                }
                break;
            }
            case 4: system.displayList(Status::CONFIRMED); break;
            case 5: system.displayList(Status::RAC); break;
            case 6: system.displayList(Status::WAITING); break;
            case 7: system.sortWaitingListByAge(); break;
            case 8: system.undoLastCancellation(); break;
            case 9: system.displayAll(); break;
            default: std::cout << "Invalid choice, try again.\n";
        }
    }
    return 0;
}
