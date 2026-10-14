#include <iostream>
#include <string>
#include <limits>
using namespace std;

// ---------- Node ----------
struct Node {
    int studentID;
    string studentName;
    int bookID;
    string bookTitle;
    int priority;          // 1 = highest priority ... 5 = lowest
    Node* next;

    Node(int sid, string sname, int bid, string title, int pr)
        : studentID(sid), studentName(sname), bookID(bid),
          bookTitle(title), priority(pr), next(NULL) {}
};

// ---------- Priority Queue (sorted linked list) ----------
class PriorityQueue {
private:
    Node* head;            // head always holds the highest-priority request
    int count;

public:
    PriorityQueue() : head(NULL), count(0) {}

    ~PriorityQueue() {     // frees every remaining node
        while (head != NULL) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    bool IsEmpty() const    { return head == NULL; }
    bool IsNotEmpty() const { return head != NULL; }
    int  Size() const       { return count; }

    // Insert in priority order. Equal priority -> first come, first served.
    void Add(int sid, string sname, int bid, string title, int pr) {
        Node* newNode = new Node(sid, sname, bid, title, pr);

        if (IsEmpty() || pr < head->priority) {          // goes at the front
            newNode->next = head;
            head = newNode;
        } else {
            Node* cur = head;
            while (cur->next != NULL && cur->next->priority <= pr)
                cur = cur->next;                          // skip equal/higher priority
            newNode->next = cur->next;
            cur->next = newNode;
        }
        count++;
    }

    // Remove and show the highest-priority request.
    void Remove() {
        if (IsEmpty()) {
            cout << "\nNo requests to process. Queue is empty.\n";
            return;
        }
        Node* temp = head;
        head = head->next;
        cout << "\nProcessing request:\n";
        cout << "  Student : " << temp->studentID << " - " << temp->studentName << "\n";
        cout << "  Book    : " << temp->bookID << " - " << temp->bookTitle << "\n";
        cout << "  Priority: " << temp->priority << "\n";
        delete temp;
        count--;
    }

    void Display() const {
        if (IsEmpty()) {
            cout << "\nNo pending requests.\n";
            return;
        }
        cout << "\nPending requests (highest -> lowest priority):\n";
        int n = 1;
        for (Node* cur = head; cur != NULL; cur = cur->next, n++) {
            cout << n << ". [P" << cur->priority << "] "
                 << cur->studentID << " " << cur->studentName << " | "
                 << cur->bookID << " " << cur->bookTitle << "\n";
        }
    }
};

// ---------- Input helpers ----------
int readInt(const string& prompt, int lo = numeric_limits<int>::min(),
            int hi = numeric_limits<int>::max()) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= lo && value <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

// ---------- Menu ----------
void showMenu() {
    cout << "\n===== Library Book Request System =====\n"
         << "1. Add Book Request\n"
         << "2. Process Highest Priority Request\n"
         << "3. Display All Requests\n"
         << "4. Check Queue Status\n"
         << "5. Exit\n";
}

int main() {
    PriorityQueue pq;
    int choice;

    do {
        showMenu();
        choice = readInt("Enter your choice (1-5): ", 1, 5);

        switch (choice) {
        case 1: {
            int sid = readInt("Student ID: ");
            string sname = readLine("Student Name: ");
            int bid = readInt("Book ID: ");
            string title = readLine("Book Title: ");
            int pr = readInt("Priority (1 = highest, 5 = lowest): ", 1, 5);
            pq.Add(sid, sname, bid, title, pr);
            cout << "Request added.\n";
            break;
        }
        case 2:
            pq.Remove();
            break;
        case 3:
            pq.Display();
            break;
        case 4:
            if (pq.IsNotEmpty())
                cout << "\nQueue is NOT empty. Pending requests: " << pq.Size() << "\n";
            else
                cout << "\nQueue is EMPTY.\n";
            break;
        case 5:
            cout << "\nExiting... releasing memory.\n";
            break;
        }
    } while (choice != 5);

    return 0;   // ~PriorityQueue() deletes any remaining nodes
}
