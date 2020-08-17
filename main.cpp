#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
using namespace std;

struct BookData  // this is used to store a book data
{
    int bookID;
    string bookTitle;
    string author;
    int userId;
};

vector<string> split(string str, string token) {// this function is used to break a string into a vector using a delimiter
    vector<string>result;// creating a vector to store a string
    while (str.size())
    {
        int index = str.find(token);
        if (index != string::npos)
        {
            result.push_back(str.substr(0, index));
            str = str.substr(index + token.size());
            if (str.size() == 0)result.push_back(str); // adding individual string to vector
        }
        else
        {
            result.push_back(str); // adding individual string to vector
            str = "";
        }
    }
    return result;// returning the string vector
}

int convertStringtoInt(string data)
{
    stringstream intValue(data);  //store the string into stringstream
    int x = 0;
    intValue >> x;// converting string value to int
    return x; // returning the int

}


bool checkId(string id) // this function is used to check for user id
{
    if (id.length() != 4) 
    {
        cout << "The user id is invalid as its size is greater than 4" << endl;
    }
    string int_num = "^(0|[1-9][0-9]*)$";
    regex pattern(int_num, regex_constants::ECMAScript);
    if (regex_match(id, pattern))
    {
        int nid = convertStringtoInt(id);
        if (nid >= 1000 && nid <= 9999)
        {
            return true;
        }
        else
        {
            cout << "Id should be between 1000 and 9999" << endl;
            return false;

        }
    }
    else 
    {
        cout << "The user id is invalid as it contains characters" << endl;
        return false;
    }
    
}


void loadDataToVector(vector<BookData> &book)  // this function is used to read the data from the database
{
    fstream newfile;
    newfile.open("database.txt", ios::in); //open a file to perform read operation using file object
    if (newfile.is_open()) {   //checking whether the file is open
        string tp;
        while (getline(newfile, tp)) { //read data from file object and put it into string.
            vector<string> bookData = split(tp,",");
            BookData obj;
            obj.bookID = convertStringtoInt(bookData.at(0));
            obj.bookTitle = bookData.at(1);
            obj.author = bookData.at(2);
            obj.userId = convertStringtoInt(bookData.at(3));
            book.push_back(obj);
        }
        newfile.close(); //close the file object.
    }
    else 
    {
        cout << "Database file not found" << endl;
    }
}

bool search(vector<BookData> &book) // this function is used to search a book
{
    string title;
    cout << "Enter the book title you want to find" << endl;
    cin.ignore();
    getline(cin, title);
    transform(title.begin(),title.end(),title.begin(),::tolower);
    int found = 0;
    for (int i = 0; i < book.size();i++)
    {
        string temp = book.at(i).bookTitle;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (temp.compare(title) == 0)
        {
            cout << "Book Information" << endl;
            cout << "Book Id: "<<book.at(i).bookID << endl;
            cout << "Book Author: " << book.at(i).author << endl;
            cout << "Book Title: " << book.at(i).bookTitle << endl;
            cout << "User Id: " << book.at(i).userId << endl;
            cout << endl;
            return true;
        }
    }
    return false;
}

int checkBookId(int id,vector<BookData> &book)//this function is used to check if the book id is valid
{
    for (int i = 0; i < book.size();i++)
    {
        if (id == book.at(i).bookID) 
        {
            return i;
        }
    }
    return -1;
}


void checkOut(vector<BookData> &book) // this is used for checkOut
{
    string id;
    cout << "Enter the User Id: " ;
    cin >> id;
    cout << endl;
    bool check;
    check = checkId(id);
    if (!check) 
    {
        return;
    }
    int bookId;
    cout << "Enter the book id you want to borrow: ";
    cin >> bookId;
    cout << endl;
    int checkBook = checkBookId(bookId,book);
    if (checkBook == -1)
    {
        cout << "Invalid Book Id" << endl;
        cout << endl;
        return;
    }
    
    if (book.at(checkBook).userId == 0)
    {
        book.at(checkBook).userId = convertStringtoInt(id);
        cout << "Book Is issued to you" << endl;
        cout << endl;
    }
    else 
    {
        cout << "The book is already borrowed by another user" << endl;
        cout << endl;
    }

}

void returnBook(vector<BookData> &book)// this function is used to return book
{
    int bookId;
    cout << "Enter the book id you want to return: ";
    cin >> bookId;
    cout << endl;
    int checkBook = checkBookId(bookId, book);
    if (checkBook == -1)
    {
        cout << "Invalid Book Id" << endl;
        cout << endl;
        return;
    }

    if (book.at(checkBook).userId == 0)
    {
        cout << "Book is already free" << endl;
        cout << endl;
    }
    else
    {
        book.at(checkBook).userId = 0;
        cout << "The book is returned" << endl;
        cout << endl;
    }

}

void writeDataToFile(vector<BookData> &book)// this function is used to update the database
{
    ofstream myfile;
    string temp;
    myfile.open("database.txt");
    for (int i = 0; i < book.size();i++)
    {
        temp = to_string(book.at(i).bookID) + "," + book.at(i).bookTitle+","+ book.at(i).author + "," +to_string(book.at(i).userId);
        myfile<< temp<< "\n";
    }
    
    myfile.close();
}


int main()
{
    int choice;
    vector<BookData> book;// creating a vector of to store information of different book
    loadDataToVector(book);// storing information of books from the database to vector book
    
    
    while (true)
    {
        cout << "1.Search a book" << endl;
        cout << "2.Check Out"<<endl;
        cout << "3.Return Book" << endl;
        cout << "4.Exit"<<endl;
        cout << "\nEnter Your Choice : ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            bool find = search(book);
            if (!find) 
            {
                cout << "No book exists with this title" << endl;
            }
            break;
        }
        case 2:
        {
            checkOut(book);
            break;
        }
        case 3: 
        {
            returnBook(book);
            break;
        }
        case 4: 
        {
            writeDataToFile(book);
            cout << "Exiting the program......" << endl;
            exit(0);
            break;
        }
        default: 
        {
            cout << "\nWrong Choice\n";
        }
            
        }
        
    }
    return 0;

}

    

