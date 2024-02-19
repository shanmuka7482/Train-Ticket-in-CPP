#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib> // For rand() and srand()
#include <ctime>  //For time

using namespace std;

class Ticket {
public:
    Ticket(int seatNumber, const string& passengerName)
        : seatNumber(seatNumber), passengerName(passengerName) {}

    int getSeatNumber() const {
        return seatNumber;
    }

    const string& getPassengerName() const {
        return passengerName;
    }

private:
    int seatNumber;
    string passengerName;
};

class Train {
public:
    Train(int totalSeats) : totalSeats(totalSeats) {
        for (int i = 1; i <= totalSeats; ++i) {
            availableSeats.push_back(i);
        }
    }

    void displayAvailableSeats() const {
        cout << "Available Seats: ";
        for (int seat : availableSeats) {
            cout << seat << " ";
        }
        cout << endl;
    }

    bool bookTicket(const string& passengerName) {
        if (availableSeats.empty()) {
            cout << "Sorry, the train is fully booked." << endl;
            return false;
        }

        // Generate a random seat number
        int randomIndex = rand() % availableSeats.size();
        int seatNumber = availableSeats[randomIndex];


        availableSeats.erase(availableSeats.begin() + randomIndex); // Remove the booked seat
        Ticket ticket(seatNumber, passengerName);
        bookedTickets.push_back(ticket);
        writeToFile(seatNumber, passengerName);
        cout << "Ticket booked successfully. Seat number: " << seatNumber << ", Passenger name: " << passengerName << endl;
        return true;
        }

    void writeToFile(int seatNumber, const string& passengerName) {
        ofstream outFile("booktickets.txt", ios::app); 
        outFile << "Passenger Name: " << passengerName << endl;
        outFile << "Seat Number: " << seatNumber << endl;
        outFile << "-------------------------------\n";
        outFile.close();
    }

    void removeFromFile(int seatNumber, const string& passengerName) {
        ifstream inFile("booktickets.txt");
        ofstream outFile("temp.txt");

        string line;
        while (getline(inFile, line)) {
            if (line.find("Seat Number: " + to_string(seatNumber)) == string::npos &&
                line.find("Passenger Name: " + passengerName) == string::npos) {
                outFile << line << endl;
            }
        }

        inFile.close();
        outFile.close();

        remove("booktickets.txt");
        rename("temp.txt", "booktickets.txt");
    }


    void cancelTicket(int seatNumber,const string& passengerName) {
        auto it = find_if(bookedTickets.begin(), bookedTickets.end(),
                           [seatNumber](const Ticket& ticket) {
                               return ticket.getSeatNumber() == seatNumber;
                           });

        if (it != bookedTickets.end()) {
            removeFromFile(seatNumber,passengerName);
            availableSeats.push_back(seatNumber);
            bookedTickets.erase(it);
            cout << "Ticket canceled successfully. Seat number: " << seatNumber << endl;
        } else {
            cout << "Ticket not found. Please enter a valid seat number." << endl;
        }
    }

private:
    int totalSeats;
    vector<int> availableSeats;
    vector<Ticket> bookedTickets;
};

int main() {
    Train train(50); // Assume the train has 50 seats

    int choice;
    do {
        cout << "\n1. Display Available Seats"
             << "\n2. Book a Ticket"
             << "\n3. Cancel a Ticket"
             << "\n4. Exit"
             << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                train.displayAvailableSeats();
                break;
            case 2: {
                string passengerName;
                cout << "Enter passenger name: ";
                cin.ignore();
                getline(cin, passengerName);
                transform(passengerName.begin(), passengerName.end(), passengerName.begin(), ::tolower);
                train.bookTicket(passengerName);
                break;
            }
            case 3: {
                int seatNumber;
                string passengerName;
                cout << "Enter seat number to cancel: ";
                cin >> seatNumber;
                cout << "Enter passenger name: ";
                cin.ignore();
                getline(cin, passengerName);
                transform(passengerName.begin(), passengerName.end(), passengerName.begin(), ::tolower);
                train.cancelTicket(seatNumber,passengerName);
                break;
            }
            case 4:
                cout << "Exiting the program." << endl;
                break;
            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 4);

    return 0;
}
