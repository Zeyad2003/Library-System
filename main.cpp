// Dr. Mostafa Projects
// Zeyad Nasef
// Structures project "Library System"
#include <bits/stdc++.h>
using namespace std;

//Book Structure
struct book{
    int id, quantity, totBorrowed;
    string name;
    book() : id(-1), quantity(0), totBorrowed(0), name (""){};

    //Read function
    void read()
    {
        cout << "Enter book id :";
        cin >> id;

        cout << "Enter book name :";
        cin >> name;

        cout << "Enter book quantity :";
        cin >> quantity;

        totBorrowed = 0;  // initialized for every new book
    }

    // Borrow function to check if user can borrow the book he wants
    bool borrow() {
        if (quantity - totBorrowed == 0) // Tells us how if there's available book to borrow
            return false;
        ++totBorrowed;
        return true;
    }

    // To return the book that user borrow
    void return_copy() {
        assert(totBorrowed > 0); // To check that there's book has been borrowed
        --totBorrowed;
    }

    // This function check that the book name we are looking for is existed
    bool has_prefix(string prefix) {
        if (name.size() < prefix.size())
            return false;
        // linear search for the prefix
        for (int i = 0; i < (int) prefix.size(); ++i) {
            if (prefix[i] != name[i])
                return false;
        }
        return true;
    }

    // Function to print book info
    void print() {
        cout << "\n******************************************\n"
             << "The book id = " << id << "\n the book name = " << name << "\n the total_quantity "
             << quantity << "\n the total_borrowed " << totBorrowed
             << "\n******************************************\n";
    }


};
// Two compare functions to use them when sort based on the information we have
bool cmp_book_by_name(book &a, book &b) {
    return a.name < b.name;
}

bool cmp_book_by_id(book &a, book &b) {
    return a.id < b.id;
}


//User Structure
struct user{
    int id; // user id
    string name; // username
    vector<int> borrowBooks; // total borrowed books

    user() : id (-1), name(""){}; //user constructor

    //Read function
    void read() {
        cout << "Enter user info id :";
        cin >> id;
        cout << "Enter user name :";
        cin.ignore(); getline(cin, name);
    }

    // To borrow book
    void borrow(int book_id) {
        borrowBooks.emplace_back(book_id);
    }

    //Function to see if there's a book with the giving id and return it to the library
    void return_copy(int book_id) {
        bool removed = false;
        sort(borrowBooks.begin(), borrowBooks.end());

        // using binary search to find the book id
        int idx = lower_bound(borrowBooks.begin(), borrowBooks.end(), book_id) - borrowBooks.begin();
        if(borrowBooks[idx] == book_id)
        {
            removed = true;
            auto it = borrowBooks.begin() + idx;
            borrowBooks.erase(it);
            cout << "\n!!Removed successfully!!\n";
        }
        if (!removed)
            cout << "\n*********************************************************\n"
                 << "!!User " << name << " never borrowed book with id " << book_id
                 << "!!\n*********************************************************\n";
    }

    // Function to check if the book with giving id is borrowed or not using binary search algorithm
    bool is_borrowed(int book_id) {
        sort(borrowBooks.begin(), borrowBooks.end());
        int idx = lower_bound(borrowBooks.begin(), borrowBooks.end(), book_id) - borrowBooks.begin();
        if(borrowBooks[idx] == book_id)
            return true;
        return false;
    }


    // Function to print what books id user borrowed
    void print() {
        // sort what he borrowed
        sort(borrowBooks.begin(), borrowBooks.end());
        cout << "\n******************************************\n";
        cout << "user " << name << " id " << id << " borrowed books ids :";
        for(auto a:borrowBooks)
            cout << a << " ";
        cout << "\n";
        cout << "******************************************\n";
    }
};

// The main structure to run the system
struct library_system {
    int total_books;
    book *books = new book[10];
    int total_users;
    user *users = new user[10];

    // Library constructor
    library_system() : total_books(0), total_users(0){};

    // Function to start the system
    void run() {
        // keep running the system until the user finish it by himself
        while (true) {
            int choice = menu();

            if (choice == 1)
                add_book();
            else if (choice == 2)
                search_books_by_prefix();
            else if (choice == 3)
                print_who_borrowed_book_by_name();
            else if (choice == 4)
                print_library_by_id();
            else if (choice == 5)
                print_library_by_name();
            else if (choice == 6)
                add_user();
            else if (choice == 7)
                user_borrow_book();
            else if (choice == 8)
                user_return_book();
            else if (choice == 9)
                print_users();
            else
                break;
        }
    }

    // Function to view the choices and return what choice user choose
    static int menu() { /* made static to share the same memory */
        int choice = -1;
        while (choice == -1) {
                cout << "!!Library Menu!!\n";
                cout << "1) add_book\n";
                cout << "2) search_books_by_prefix\n";
                cout << "3) print_who_borrowed_book_by_name\n";
                cout << "4) print_library_by_id\n";
                cout << "5) print_library_by_name\n";
                cout << "6) add_user\n";
                cout << "7) user_borrow_book\n";
                cout << "8) user_return_book\n";
                cout << "9) print_users\n";
                cout << "10) Exit\n";


            cout << "\nEnter your menu choice [1 - 10] :";
            cin >> choice;

            if (!(1 <= choice && choice <= 10)) {
                cout << "Invalid choice. Try again\n";
                choice = -1;	// make loop keep working
            }
        }
        return choice;
    }

    // Function to add book to the system and read its info
    void add_book() {
        books[total_books++].read();
    }

    // The prefix function to search about the book with first letters user enter
    void search_books_by_prefix() {
        cout << "Enter book name prefix :";
        string prefix;
        cin.ignore();
        getline(cin, prefix);

        int cnt = 0;
        bool first = true;
        for (int i = 0; i < total_books; ++i) {
            if (books[i].has_prefix(prefix)){
                if(first){
                    cout << "\n************************\n";
                    first = false;
                }
                cout << books[i].name << "\n";
                ++cnt;
            }
        }
        if(cnt) cout << "************************\n";
        if(!cnt)
           cout << "\n**************************\n"
                << "No books with such prefix"
                << "\n**************************\n";
    }

    // Function To add user and read its info
    void add_user() {
        users[total_users++].read();
    }

    // Function to search about the book with his name
    int find_book_idx_by_name(const string& name) const {
        for (int i = 0; i < total_books; ++i) {
            if (name == books[i].name)
                return i;
        }
        return -1;
    }

    // Function to search about the book with his name
    int find_user_idx_by_name(string name) {
        for (int i = 0; i < total_users; ++i) {
            if (name == users[i].name)
                return i;
        }
        return -1;
    }

    // Function to check if the book is and user available or not with 3 times of trying
    bool read_user_name_and_book_name(int &user_idx, int &book_idx, int trials = 3) {
        string user_name;
        string book_name;

        while (trials--) {
            cout << "Enter user name :";
            cin.ignore();
            getline(cin, user_name);

            cout << "Enter book name :";
            cin >> book_name;

            user_idx = find_user_idx_by_name(user_name);
            if (user_idx == -1) {
                cout << "\n!!Invalid user name. Try again!!\n";
                continue;
            }

            book_idx = find_book_idx_by_name(book_name);
            if (book_idx == -1) {
                cout << "\n!!Invalid book name. Try again!!\n";
                continue;
            }
            return true;
        }
        cout << "\n!!You did several trials! Try later.!!\n";
        return false;
    }

    void user_borrow_book() {
        int user_idx, book_idx;

        if (!read_user_name_and_book_name(user_idx, book_idx))
            return;

        int book_id = books[book_idx].id;

        if (!books[book_idx].borrow())
            cout << "\n\n!!No more copies available right now!!\n\n";
        else
            users[user_idx].borrow(book_id);
    }

    // function to return book to the library
    void user_return_book() {
        int user_idx, book_idx;

        if (!read_user_name_and_book_name(user_idx, book_idx))
            return;

        int book_id = books[book_idx].id;
        books[book_idx].return_copy();
        users[user_idx].return_copy(book_id);
    }

    // Function to print all books in the system "sorted"
    void print_library_by_id() {
        sort(books, books + total_books, cmp_book_by_id);

        cout << "\n";
        for (int i = 0; i < total_books; ++i)
            books[i].print();
    }

    // Function to pring all users in the system "sorted"
    void print_library_by_name() {
        sort(books, books + total_books, cmp_book_by_name);

        cout << "\n";
        for (int i = 0; i < total_books; ++i)
            books[i].print();
    }

    // Function to print all users in the system
    void print_users() {
        cout << "\n";
        if(total_users == 0)
            cout << "There's no user right now!!\n";
        else {
            for (int i = 0; i < total_users; ++i)
                users[i].print();
        }
    }


    void print_who_borrowed_book_by_name() {
        string book_name;
        cout << "Enter book name :";
        cin.ignore();
        getline(cin, book_name);

        int book_idx = find_book_idx_by_name(book_name);

        if (book_idx == -1) {
            cout << "\n!!Invalid book name.!!\n";
            return;
        }
        int book_id = books[book_idx].id;

        if (books[book_idx].totBorrowed == 0) {
            cout << "\n!!No borrowed copies.!!\n\n";
            return;
        }

        for (int i = 0; i < total_users; ++i) {
            if (users[i].is_borrowed(book_id))
                cout << users[i].name << "\n";
        }
    }
};


// main function
int main()
{
    library_system library; // create object

    library.run();
}