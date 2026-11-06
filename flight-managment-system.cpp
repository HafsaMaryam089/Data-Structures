
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

// ======================================================
//                 FLIGHT DURATION
// ======================================================

float calculateDuration(string dep, string arr)
{
    int dh = 0, dm = 0, ah = 0, am = 0;
    char colon;

    stringstream(dep) >> dh >> colon >> dm;
    stringstream(arr) >> ah >> colon >> am;

    int depTotalMins = dh * 60 + dm;
    int arrTotalMins = ah * 60 + am;

    if (arrTotalMins < depTotalMins)
    {
        arrTotalMins += 24 * 60;
    }

    return (arrTotalMins - depTotalMins) / 60.0;
}

// ======================================================
//                 CALCULATE DELAY
// ======================================================

int calculateDelayMinutes(string schedArr, string actualArr)
{
    int sh = 0, sm = 0, ah = 0, am = 0;
    char colon;

    stringstream(schedArr) >> sh >> colon >> sm;
    stringstream(actualArr) >> ah >> colon >> am;

    int schedTotalMins = sh * 60 + sm;
    int actualTotalMins = ah * 60 + am;

    // Handle arrival after midnight
    if (actualTotalMins < schedTotalMins &&
        (schedTotalMins - actualTotalMins) > 12 * 60)
    {
        actualTotalMins += 24 * 60;
    }

    return actualTotalMins - schedTotalMins;
}

// ======================================================
//                    NODE STRUCTURE
// ======================================================

struct Node
{
    int flightID;
    string source;
    string destination;
    string airline;
    string dep_time;
    string arr_time;
    float flight_dur;
    int price;
    string status;

    Node* left;
    Node* right;
    Node* p;

    Node(int ID, string src, string dest, string aline,
        string dtime, string atime, int pri, string s)
    {
        flightID = ID;
        source = src;
        destination = dest;
        airline = aline;
        dep_time = dtime;
        arr_time = atime;

        flight_dur = calculateDuration(dtime, atime);

        price = pri;
        status = s;

        left = nullptr;
        right = nullptr;
        p = nullptr;
    }
};

class BST
{
public:

    Node* root;

    BST()
    {
        root = nullptr;
    }

  
    Node* search(int flightID)
    {
        Node* x = root;

        while (x != nullptr && x->flightID != flightID)
        {
            if (flightID < x->flightID)
                x = x->left;
            else
                x = x->right;
        }

        return x;
    }

   
    void insert(int ID, string src, string d, string aline,
        string dtime, string atime, int pri, string s)
    {
        if (search(ID) != nullptr)
        {
            cout << "Flight ID: " << ID
                << " already exists!" << endl;
            return;
        }

        Node* z = new Node(ID, src, d, aline,
            dtime, atime, pri, s);

        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr)
        {
            y = x;

            if (z->flightID < x->flightID)
                x = x->left;
            else
                x = x->right;
        }

        z->p = y;

        if (y == nullptr)
            root = z;

        else if (z->flightID < y->flightID)
            y->left = z;

        else
            y->right = z;
    }

    // ==================================================
    //                    MINIMUM
    // ==================================================

    Node* minimum(Node* x)
    {
        while (x != nullptr && x->left != nullptr)
        {
            x = x->left;
        }

        return x;
    }

    // ==================================================
    //                    MAXIMUM
    // ==================================================

    Node* maximum(Node* x)
    {
        while (x != nullptr && x->right != nullptr)
        {
            x = x->right;
        }

        return x;
    }

    // ==================================================
    //                   TRANSPLANT
    // ==================================================

    void transplant(Node* u, Node* v)
    {
        if (u->p == nullptr)
            root = v;

        else if (u == u->p->left)
            u->p->left = v;

        else
            u->p->right = v;

        if (v != nullptr)
            v->p = u->p;
    }

    // ==================================================
    //                    DELETE
    // ==================================================

    void deletekey(int flightID)
    {
        Node* z = search(flightID);

        if (z == nullptr)
        {
            cout << "Flight ID " << flightID
                << " not found!" << endl;
            return;
        }

        // Case 1: No left child
        if (z->left == nullptr)
        {
            transplant(z, z->right);
        }

        // Case 2: No right child
        else if (z->right == nullptr)
        {
            transplant(z, z->left);
        }

        // Case 3: Two children
        else
        {
            Node* y = minimum(z->right);

            if (y->p != z)
            {
                transplant(y, y->right);

                y->right = z->right;
                y->right->p = y;
            }

            transplant(z, y);

            y->left = z->left;
            y->left->p = y;
        }

        delete z;

        cout << "Flight ID " << flightID
            << " deleted successfully." << endl;
    }

    // ==================================================
    //                 INORDER TRAVERSAL
    // ==================================================

    void inorder(Node* x)
    {
        if (x != nullptr)
        {
            inorder(x->left);

            cout << "\nFlight ID: " << x->flightID << endl;
            cout << "Route: " << x->source
                << " to " << x->destination << endl;
            cout << "Airline: " << x->airline << endl;

            cout << "Departure: " << x->dep_time
                << " | Arrival: " << x->arr_time << endl;

            cout << "Duration: " << fixed << setprecision(2)
                << x->flight_dur << " hours" << endl;

            cout << "Price: " << x->price << " PKR" << endl;
            cout << "Status: " << x->status << endl;

            cout << "------------------------------------------" << endl;

            inorder(x->right);
        }
    }

    // ==================================================
    //                 DISPLAY ALL FLIGHTS
    // ==================================================

    void display()
    {
        if (root == nullptr)
        {
            cout << "No flight records available." << endl;
            return;
        }

        cout << "\n========== ALL FLIGHTS ==========\n";

        inorder(root);

        cout << endl;
    }

    // ==================================================
    //                 SEARCH AND DISPLAY
    // ==================================================

    void searchAndDisplay(int flightID)
    {
        Node* x = search(flightID);

        if (x == nullptr)
        {
            cout << "Flight ID " << flightID
                << " not found in records." << endl;
            return;
        }

        cout << "\n========== FLIGHT FOUND ==========\n";

        cout << "Flight ID: " << x->flightID << endl;
        cout << "Route: " << x->source
            << " to " << x->destination << endl;
        cout << "Airline: " << x->airline << endl;
        cout << "Departure Time: " << x->dep_time << endl;
        cout << "Arrival Time: " << x->arr_time << endl;

        cout << "Duration: " << fixed << setprecision(2)
            << x->flight_dur << " hours" << endl;

        cout << "Price: " << x->price << " PKR" << endl;
        cout << "Status: " << x->status << endl;

        cout << "==================================\n";
    }

    // ==================================================
    //              CALCULATE ETA AND DELAY
    // ==================================================

    void calculateETAAndDelay(int flightID)
    {
        Node* x = search(flightID);

        if (x == nullptr)
        {
            cout << "Flight ID " << flightID
                << " not found." << endl;
            return;
        }

        string actualArr;

        cout << "Enter Actual Arrival Time (HH:MM): ";
        cin >> actualArr;

        int diffMins = calculateDelayMinutes(
            x->arr_time, actualArr);

        cout << "\n========== FLIGHT ETA & STATUS REPORT ==========\n";

        cout << "Flight ID: " << x->flightID << endl;

        cout << "Route: " << x->source
            << " to " << x->destination << endl;

        cout << "Scheduled Departure: " << x->dep_time << endl;
        cout << "Scheduled Arrival: " << x->arr_time << endl;
        cout << "Actual Arrival: " << actualArr << endl;

        cout << "Calculated Flight Duration: "
            << fixed << setprecision(2)
            << x->flight_dur << " hours" << endl;

        cout << "-----------------------------------------------\n";

        if (diffMins > 0)
        {
            int hours = diffMins / 60;
            int mins = diffMins % 60;

            cout << "Status: DELAYED by ";

            if (hours > 0)
                cout << hours << " hr(s) ";

            cout << mins << " min(s)." << endl;

            x->status = "Delayed";
        }

        else if (diffMins < 0)
        {
            int hours = abs(diffMins) / 60;
            int mins = abs(diffMins) % 60;

            cout << "Status: EARLY by ";

            if (hours > 0)
                cout << hours << " hr(s) ";

            cout << mins << " min(s)." << endl;

            x->status = "Early";
        }

        else
        {
            cout << "Status: ON-TIME" << endl;

            x->status = "On-Time";
        }

        cout << "===============================================\n";
    }

    // ==================================================
    //                 SPECIAL FEATURE
    //             FLIGHT STATUS DASHBOARD
    // ==================================================

    // CANCELLED FLIGHTS

    void displayCancelledFlights(Node* x, int& count)
    {
        if (x != nullptr)
        {
            displayCancelledFlights(x->left, count);

            if (x->status == "Cancelled" ||
                x->status == "cancelled")
            {
                cout << left
                    << setw(10) << x->flightID
                    << setw(27) << (x->source + " -> " + x->destination)
                    << setw(18) << x->airline
                    << setw(15) << "-"
                    << setw(15) << "CANCELLED"
                    << endl;

                count++;
            }

            displayCancelledFlights(x->right, count);
        }
    }

    // DELAYED FLIGHTS

    void displayDelayedFlights(Node* x, int& count)
    {
        if (x != nullptr)
        {
            displayDelayedFlights(x->left, count);

            if (x->status == "Delayed" ||
                x->status == "delayed")
            {
                cout << left
                    << setw(10) << x->flightID
                    << setw(27) << (x->source + " -> " + x->destination)
                    << setw(18) << x->airline
                    << setw(15) << x->dep_time
                    << setw(15) << "DELAYED"
                    << endl;

                count++;
            }

            displayDelayedFlights(x->right, count);
        }
    }

    // EARLY FLIGHTS

    void displayEarlyFlights(Node* x, int& count)
    {
        if (x != nullptr)
        {
            displayEarlyFlights(x->left, count);

            if (x->status == "Early" ||
                x->status == "early")
            {
                cout << left
                    << setw(10) << x->flightID
                    << setw(27) << (x->source + " -> " + x->destination)
                    << setw(18) << x->airline
                    << setw(15) << x->arr_time
                    << setw(15) << "EARLY"
                    << endl;

                count++;
            }

            displayEarlyFlights(x->right, count);
        }
    }

    // SCHEDULED FLIGHTS

    void displayScheduledFlights(Node* x, int& count)
    {
        if (x != nullptr)
        {
            displayScheduledFlights(x->left, count);

            if (x->status == "Scheduled" ||
                x->status == "scheduled")
            {
                cout << left
                    << setw(10) << x->flightID
                    << setw(27) << (x->source + " -> " + x->destination)
                    << setw(18) << x->airline
                    << setw(15) << x->dep_time
                    << setw(15) << "SCHEDULED"
                    << endl;

                count++;
            }

            displayScheduledFlights(x->right, count);
        }
    }

    // ==================================================
    //                 RUN STATUS DASHBOARD
    // ==================================================

    void runStatusDashboard()
    {
        if (root == nullptr)
        {
            cout << "No flights available to check status." << endl;
            return;
        }

        int cancelledCount = 0;
        int delayedCount = 0;
        int earlyCount = 0;
        int scheduledCount = 0;

        cout << "\n";
        cout << "==============================================================\n";
        cout << "                   FLIGHT STATUS DASHBOARD                    \n";
        cout << "==============================================================\n";

        // CANCELLED TABLE

        cout << "\nCANCELLED FLIGHTS\n";

        cout << "--------------------------------------------------------------------------\n";

        cout << left
            << setw(10) << "ID"
            << setw(27) << "ROUTE"
            << setw(18) << "AIRLINE"
            << setw(15) << "TIME"
            << setw(15) << "STATUS"
            << endl;

        cout << "--------------------------------------------------------------------------\n";

        displayCancelledFlights(root, cancelledCount);

        if (cancelledCount == 0)
            cout << "No cancelled flights.\n";

        // DELAYED TABLE

        cout << "\nDELAYED FLIGHTS\n";

        cout << "--------------------------------------------------------------------------\n";

        cout << left
            << setw(10) << "ID"
            << setw(27) << "ROUTE"
            << setw(18) << "AIRLINE"
            << setw(15) << "DEPARTURE"
            << setw(15) << "STATUS"
            << endl;

        cout << "--------------------------------------------------------------------------\n";

        displayDelayedFlights(root, delayedCount);

        if (delayedCount == 0)
            cout << "No delayed flights.\n";

        // EARLY TABLE

        cout << "\nEARLY FLIGHTS\n";

        cout << "--------------------------------------------------------------------------\n";

        cout << left
            << setw(10) << "ID"
            << setw(27) << "ROUTE"
            << setw(18) << "AIRLINE"
            << setw(15) << "ARRIVAL"
            << setw(15) << "STATUS"
            << endl;

        cout << "--------------------------------------------------------------------------\n";

        displayEarlyFlights(root, earlyCount);

        if (earlyCount == 0)
            cout << "No early flights.\n";

        // SCHEDULED TABLE

        cout << "\nSCHEDULED FLIGHTS\n";

        cout << "--------------------------------------------------------------------------\n";

        cout << left
            << setw(10) << "ID"
            << setw(27) << "ROUTE"
            << setw(18) << "AIRLINE"
            << setw(15) << "DEPARTURE"
            << setw(15) << "STATUS"
            << endl;

        cout << "--------------------------------------------------------------------------\n";

        displayScheduledFlights(root, scheduledCount);

        if (scheduledCount == 0)
            cout << "No scheduled flights.\n";

        // DASHBOARD SUMMARY

        cout << "\n";
        cout << "==============================================================\n";
        cout << "                    DASHBOARD SUMMARY                         \n";
        cout << "==============================================================\n";

        cout << left
            << setw(25) << "Scheduled Flights:"
            << scheduledCount << endl;

        cout << left
            << setw(25) << "Early Flights:"
            << earlyCount << endl;

        cout << left
            << setw(25) << "Delayed Flights:"
            << delayedCount << endl;

        cout << left
            << setw(25) << "Cancelled Flights:"
            << cancelledCount << endl;

        cout << "--------------------------------------------------------------\n";

        cout << left
            << setw(25) << "Total Flights:"
            << scheduledCount + earlyCount +
            delayedCount + cancelledCount << endl;

        cout << "==============================================================\n";
    }

    // ==================================================
    //                    FILE HANDLING
    // ==================================================

    // SAVE TO FILE

    void saveToFile(Node* x, ofstream& outFile)
    {
        if (x != nullptr)
        {
            saveToFile(x->left, outFile);

            outFile << x->flightID << ","
                << x->source << ","
                << x->destination << ","
                << x->airline << ","
                << x->dep_time << ","
                << x->arr_time << ","
                << x->flight_dur << ","
                << x->price << ","
                << x->status << endl;

            saveToFile(x->right, outFile);
        }
    }

    void saveAll(string filename)
    {
        if (root == nullptr)
        {
            cout << "\nThe flight tree is empty! "
                << filename << ".\n";
            return;
        }

        ofstream outFile(filename);

        if (!outFile)
        {
            cout << "Error opening file for writing." << endl;
            return;
        }

        saveToFile(root, outFile);

        outFile.close();

        cout << "Flights successfully saved to "
            << filename << endl;
    }

    // LOAD FROM FILE

    void loadFromFile(string filename)
    {
        ifstream inFile(filename);

        if (!inFile)
        {
            cout << "No saved record file found from "
                << filename << endl;
            return;
        }

        string line;
        int count = 0;

        while (getline(inFile, line))
        {
            if (line.empty())
                continue;

            stringstream ss(line);

            string idStr, src, dest, aline;
            string dtime, atime, durStr, priceStr, s;

            getline(ss, idStr, ',');
            getline(ss, src, ',');
            getline(ss, dest, ',');
            getline(ss, aline, ',');
            getline(ss, dtime, ',');
            getline(ss, atime, ',');
            getline(ss, durStr, ',');
            getline(ss, priceStr, ',');
            getline(ss, s);

            int id = stoi(idStr);
            int price = stoi(priceStr);

            if (search(id) == nullptr)
            {
                insert(id, src, dest, aline,
                    dtime, atime, price, s);

                count++;
            }
        }

        inFile.close();

        cout << "Successfully loaded " << count
            << " flight records from "
            << filename << endl;
    }

};

// ======================================================
//                    MAIN FUNCTION
// ======================================================

int main()
{
    BST tree;

    int choice;
    int flightID;

    string source;
    string destination;
    string airline;
    string dept_time;
    string arr_time;
    string status;

    int price;

    do
    {
        cout << "\n";
        cout << "================================================\n";
        cout << "              FLIGHT MANAGEMENT SYSTEM           \n";
        cout << "================================================\n";

        cout << "1. Add New Flight\n";
        cout << "2. Search Flight by ID\n";
        cout << "3. Delete Flight by ID\n";
        cout << "4. Display All Flights\n";
        cout << "5. Calculate Flight Duration and ETA\n";
        cout << "6. Flight Status Dashboard (Special Feature)\n";
        cout << "7. Save Flights to File\n";
        cout << "8. Load Flights from File\n";
        cout << "0. Exit\n";

        cout << "================================================\n";

        cout << "Enter your choice: ";
        cin >> choice;

        cout << endl;

        switch (choice)
        {
            // ==========================================
            //                 ADD FLIGHT
            // ==========================================

        case 1:
        {
            cout << "Enter flight ID: ";

            while (!(cin >> flightID))
            {
                cout << "Invalid input. Please enter numbers only: ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            if (tree.search(flightID) != nullptr)
            {
                cout << "Flight ID: " << flightID
                    << " already exists!" << endl;
            }

            else
            {
                cout << "Enter source city: ";
                cin >> source;

                cout << "Enter destination city: ";
                cin >> destination;

                cout << "Enter airline: ";
                cin >> airline;

                cout << "Enter departure time (HH:MM): ";
                cin >> dept_time;

                cout << "Enter arrival time (HH:MM): ";
                cin >> arr_time;

                cout << "Enter price (PKR): ";
                cin >> price;

                cout << "Enter status (Scheduled/Delayed/Cancelled): ";
                cin >> status;

                cout << endl;

                tree.insert(flightID, source, destination,
                    airline, dept_time, arr_time,
                    price, status);

                cout << "Flight added successfully!" << endl;
            }

            break;
        }

        // ==============================================
        //                 SEARCH FLIGHT
        // ==============================================

        case 2:
        {
            int searchID;

            cout << "Enter Flight ID to search: ";
            cin >> searchID;

            cout << endl;

            tree.searchAndDisplay(searchID);

            break;
        }

        // ==============================================
        //                 DELETE FLIGHT
        // ==============================================

        case 3:
        {
            int deleteID;

            cout << "Enter Flight ID to delete: ";
            cin >> deleteID;

            cout << endl;

            tree.deletekey(deleteID);

            break;
        }

        // ==============================================
        //               DISPLAY ALL FLIGHTS
        // ==============================================

        case 4:
        {
            tree.display();

            break;
        }

        // ==============================================
        //               CALCULATE ETA
        // ==============================================

        case 5:
        {
            int etaID;

            cout << "Enter Flight ID to calculate duration and ETA: ";
            cin >> etaID;

            cout << endl;

            tree.calculateETAAndDelay(etaID);

            break;
        }

        // ==============================================
        //              SPECIAL FEATURE
        //            FLIGHT STATUS DASHBOARD
        // ==============================================

        case 6:
        {
            tree.runStatusDashboard();

            break;
        }

        // ==============================================
        //                 SAVE TO FILE
        // ==============================================

        case 7:
        {
            tree.saveAll("flights.txt");

            break;
        }

        // ==============================================
        //                 LOAD FROM FILE
        // ==============================================

        case 8:
        {
            tree.loadFromFile("flights.txt");

            break;
        }

        // ==============================================
        //                     EXIT
        // ==============================================

        case 0:
        {
            cout << "Exiting program. Have a great day!" << endl;

            break;
        }

        // ==============================================
        //                INVALID CHOICE
        // ==============================================

        default:
        {
            cout << "Invalid choice! Please try again." << endl;

            break;
        }

        }

    } while (choice != 0);

    return 0;
}
