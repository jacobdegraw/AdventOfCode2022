#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <cctype>
#include <sstream>
using namespace std;



//FILE CLASS
class File{
private:
    string name;
    long int size;

public:
    File(long int size_, string name_){
        size = size_;
        name = name_;
    }
    long int getSize() { return size; }
    string getName() const{ return name; }

    string toString() {
        stringstream out;
        out << name << " (Size: " << size << ")";
        return out.str();
    }

    bool operator<(const File& rhs)const{
        return name < rhs.getName();
    }
};



//DIRECTORY CLASS
class Directory {
private:
    string name;
    Directory* parent;
    set<Directory*> directories;
    set<File> files;

public:
    Directory(string name_, Directory* parent_){
        name = name_;
        parent = parent_;
    }
    //TODO Make a delete function and add it into the destructor cuz we're using pointers
    ~Directory(){ clear(); }

    //This recursively goes through and clears all of the directories in this
    void clear(){
        for (auto directory : directories){
            directory->clear();
        }

        delete this;
    }

    void addDirectory(string name){
        Directory* newDirectory = new Directory(name, this);
        directories.insert(newDirectory);
    }

    void addFile(long int size, string name){
        File newFile(size, name);
        files.insert(newFile);
    }

    Directory* cd(string name){
        for (auto dir : directories){
            if (dir->getName() == name){
                return dir;
            }
        }

        return nullptr;
    }

    Directory* toParent(){ return parent; }

    long int count(long int& total){
        long int sum = 0;
        for (auto file : files){
            sum += file.getSize();
        }
        for (auto dir : directories){
            sum += dir->count(total);
        }
        if (sum <= 100000){
            total += sum;
        }


        return sum;
    }

    set<Directory*> getDirectories() { return directories; }

    string getName() { return name; }

    string toString(string extraSpace = ""){
        stringstream out;
        out << name << endl;
        extraSpace.append(" ");
        for (auto file : files){
            out << extraSpace << file.toString() << endl;
        }

        for (auto dir : directories){
            out << extraSpace << dir->toString(extraSpace);
        }
        return out.str();
    }

};


//MAIN FUNCTION
int main(int argc, char** argv) {

    //Open the file so we can parse
    ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Unable to open " << argv[1] << " for input";
        return 2;
    }

    //Initialize the current directory

    Directory* home = new Directory("Home", nullptr);
    Directory* dir = home;

    //Parse the input, building the tree as we go
    string line;

    //This takes in one line at a time from the in fstream, puts it into the line string, and stops when we hit the end of the file
    while (getline(in, line)){
        //We're gonna use this stringstream to parse each line so we don't have to just use line.substr and cut the string
        // for every single thing because that will get real complicated real quick
        //We can get away with that because everything is seperated by spaces in the input
        stringstream input(line);
        //If the first character is a number, we know we are adding a file
        if (isdigit(line.at(0))){
            //Take in size and name, then add it to the current directory
            long int size;
            string name;

            input >> size >> name;
            dir->addFile(size, name);
        }
        //Else it is a command/making a directory
        else {
            string command;
            input >> command;
            //Make directory
            if (command == "dir"){
                string name;
                input >> name;

                dir->addDirectory(name);
            }
            //Else it's a command (cd or ls)
            else {
                input >> command;

                if (command == "ls"){
                    //cout << dir->toString();
                } else {
                    //Here we know it is cd, so we need to know where to go
                    string goTo;
                    input >> goTo;

                    //Go to the parent
                    if (goTo == ".."){
                        dir = dir->toParent();
                    }
                    //Go home
                    else if (goTo == "/"){
                        dir = home;
                    }
                    //Find the directory and go to it
                    else {
                        Directory* newDir = dir->cd(goTo);
                        if (newDir != nullptr) {
                            dir = newDir;
                        } else {
                            cout << goTo << " is not a valid directory" << endl;
                        }
                    }

                }
            }
        }
    }



    cout << home->toString() << endl;
    cout << dir->toString();

    long int total = 0;
    home->count(total);

    cout << "Total is: " << total << endl;



    return 0;
}
