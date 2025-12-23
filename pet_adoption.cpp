
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
using namespace std;

// --------------------- Base Class ---------------------
class Pet {
protected:
    string name;
    int age;
    string breed;
    bool isAdopted;

public:
    Pet(string n, int a, string b, bool adopted = false)
        : name(n), age(a), breed(b), isAdopted(adopted) {}

    virtual void displayInfo() {
        cout << "Name: " << name << ", Age: " << age
             << ", Breed: " << breed
             << (isAdopted ? " (Adopted)" : "") << endl;
    }

    void adopt() { isAdopted = true; }
    void returnPet() { isAdopted = false; }
    bool available() const { return !isAdopted; }

    string getName() const { return name; }
    bool getAdopted() const { return isAdopted; }

    virtual string getType() = 0;

    virtual void saveToFile(ofstream &fout) {
        fout << getType() << "," << name << "," << age << "," << breed << "," << isAdopted << "\n";
    }

    virtual ~Pet() {}
};

// --------------------- Derived Classes ---------------------
class Dog : public Pet {
public:
    Dog(string n, int a, string b, bool adopted = false)
        : Pet(n, a, b, adopted) {}

    void displayInfo() override {
        cout << "[Dog] ";
        Pet::displayInfo();
    }

    string getType() override { return "Dog"; }
};

class Cat : public Pet {
public:
    Cat(string n, int a, string b, bool adopted = false)
        : Pet(n, a, b, adopted) {}

    void displayInfo() override {
        cout << "[Cat] ";
        Pet::displayInfo();
    }

    string getType() override { return "Cat"; }
};

class Bird : public Pet {
public:
    Bird(string n, int a, string b, bool adopted = false)
        : Pet(n, a, b, adopted) {}

    void displayInfo() override {
        cout << "[Bird] ";
        Pet::displayInfo();
    }

    string getType() override { return "Bird"; }
};

// --------------------- Adopter Class ---------------------
class Adopter {
    string adopterName;
    vector<Pet*> adoptedPets;

public:
    Adopter(string n) : adopterName(n) {}

    void adoptPet(Pet* p) {
        if (p->available()) {
            p->adopt();
            adoptedPets.push_back(p);
            cout << adopterName << " adopted " << p->getName() << " successfully!\n";
        } else {
            cout << "This pet is already adopted!\n";
        }
    }

    void returnPet(Pet* p) {
        for (size_t i = 0; i < adoptedPets.size(); i++) {
            if (adoptedPets[i] == p) {
                p->returnPet();
                adoptedPets.erase(adoptedPets.begin() + i);
                cout << adopterName << " returned " << p->getName() << ".\n";
                return;
            }
        }
        cout << "You have not adopted this pet.\n";
    }

    void showAdoptedPets() {
        cout << "\n--- " << adopterName << "'s Adopted Pets ---\n";
        if (adoptedPets.empty()) {
            cout << "No pets adopted yet.\n";
            return;
        }
        for (auto p : adoptedPets) p->displayInfo();
    }
    
    // Helper function to check if adopter has a specific pet
    bool hasPet(const string& petName) {
        for (auto p : adoptedPets) {
            if (p->getName() == petName) {
                return true;
            }
        }
        return false;
    }
};

// --------------------- Adoption System ---------------------
class AdoptionSystem {
    vector<Pet*> pets;

public:
    void addPet(Pet* p) { pets.push_back(p); }

    void showAvailablePets() {
        cout << "\n--- Available Pets ---\n";
        int count = 0;
        for (size_t i = 0; i < pets.size(); i++) {
            if (pets[i]->available()) {
                cout << ++count << ". ";
                pets[i]->displayInfo();
            }
        }
        if (count == 0) cout << "No pets available right now.\n";
    }
    
    // New method to show all pets with indices for adoption
    void showAvailablePetsWithIndex() {
        cout << "\n--- Available Pets ---\n";
        int displayIndex = 1;
        vector<int> actualIndices;
        
        for (size_t i = 0; i < pets.size(); i++) {
            if (pets[i]->available()) {
                cout << displayIndex << ". ";
                pets[i]->displayInfo();
                actualIndices.push_back(i);
                displayIndex++;
            }
        }
        
        if (actualIndices.empty()) {
            cout << "No pets available right now.\n";
        }
    }
    
    // Get available pet by display index (1-based)
    Pet* getAvailablePetByIndex(int displayIndex) {
        int count = 0;
        for (size_t i = 0; i < pets.size(); i++) {
            if (pets[i]->available()) {
                count++;
                if (count == displayIndex) {
                    return pets[i];
                }
            }
        }
        return nullptr;
    }

    Pet* getPetByIndex(int index) {
        if (index >= 0 && index < (int)pets.size()) return pets[index];
        return nullptr;
    }
    
    // Get pet by name
    Pet* getPetByName(const string& name) {
        for (auto p : pets) {
            if (p->getName() == name) {
                return p;
            }
        }
        return nullptr;
    }

    void searchPetByName(string search) {
        cout << "\n--- Search Results ---\n";
        bool found = false;
        for (auto p : pets) {
            if (p->getName() == search) {
                p->displayInfo();
                found = true;
            }
        }
        if (!found) cout << "No pet found with name '" << search << "'.\n";
    }

    void searchPetByType(string type) {
        cout << "\n--- Search Results ---\n";
        bool found = false;
        for (auto p : pets) {
            if (p->getType() == type) {
                p->displayInfo();
                found = true;
            }
        }
        if (!found) cout << "No pets found of type '" << type << "'.\n";
    }

    void savePetsToFile(string filename = "pets_data.txt") {
        ofstream fout(filename);
        for (auto p : pets) p->saveToFile(fout);
        fout.close();
    }

    void loadPetsFromFile(string filename = "pets_data.txt") {
        ifstream fin(filename);
        if (!fin.is_open()) return;

        // Clear existing pets before loading
        for (auto p : pets) delete p;
        pets.clear();
        
        string line;

        while (getline(fin, line)) {
            if (line.empty()) continue;

            string type, name, breed;
            int age;
            bool adopted;

            size_t p1 = line.find(',');
            size_t p2 = line.find(',', p1 + 1);
            size_t p3 = line.find(',', p2 + 1);
            size_t p4 = line.find(',', p3 + 1);

            // Check if all commas were found
            if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos) {
                continue; // Skip malformed lines
            }

            type = line.substr(0, p1);
            name = line.substr(p1 + 1, p2 - p1 - 1);
            age = stoi(line.substr(p2 + 1, p3 - p2 - 1));
            breed = line.substr(p3 + 1, p4 - p3 - 1);
            adopted = stoi(line.substr(p4 + 1));

            if (type == "Dog") pets.push_back(new Dog(name, age, breed, adopted));
            else if (type == "Cat") pets.push_back(new Cat(name, age, breed, adopted));
            else if (type == "Bird") pets.push_back(new Bird(name, age, breed, adopted));
        }
    }

    ~AdoptionSystem() {
        for (auto p : pets) delete p;
    }
};

// --------------------- Main Function ---------------------
int main() {
    AdoptionSystem system;
    system.loadPetsFromFile();

    // If file was empty, add default pets
    ifstream fin("pets_data.txt");
    if (!fin.is_open() || fin.peek() == ifstream::traits_type::eof()) {
        system.addPet(new Dog("Brownie", 2, "Labrador"));
        system.addPet(new Cat("Tofu", 1, "Indian Street"));
        system.addPet(new Bird("Cheeku", 3, "Parrot"));
        system.addPet(new Dog("Maxy", 4, "German Shepherd"));
        system.addPet(new Cat("Tama", 2, "Persian"));
        system.addPet(new Cat("Troy", 3, "Siamese"));
        system.addPet(new Bird("Tweety", 1, "Canary"));
        system.addPet(new Bird("Sky", 2, "Budgerigar"));
        system.addPet(new Bird("Blue", 4, "Macaw"));
    }
    fin.close();

    string userName;
    cout << "Enter your name: ";
    getline(cin, userName);

    Adopter adopter(userName);

    int choice = 0;

    do {
        cout << "\n====== Navya's Pet Adoption System ======\n";
        cout << "1. Show Available Pets\n";
        cout << "2. Adopt a Pet\n";
        cout << "3. Return a Pet\n";
        cout << "4. Search Pet by Name\n";
        cout << "5. Search Pet by Type\n";
        cout << "6. Show My Adopted Pets\n";
        cout << "7. Save & Exit\n";
        cout << "Enter your choice: ";
        
        // Robust input handling
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between 1 and 7.\n";
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // remove leftover newline

        switch (choice) {
            case 1:
                system.showAvailablePets();
                break;

            case 2: {
                system.showAvailablePetsWithIndex();
                cout << "Enter pet number to adopt: ";
                int num;
                
                if (!(cin >> num)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid number.\n";
                    break;
                }
                
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                Pet* p = system.getAvailablePetByIndex(num);
                if (p) {
                    adopter.adoptPet(p);
                } else {
                    cout << "Invalid pet number.\n";
                }
                break;
            }

            case 3: {
                adopter.showAdoptedPets();
                cout << "Enter the name of the pet to return: ";
                string petName;
                getline(cin, petName);

                Pet* p = system.getPetByName(petName);
                if (p && adopter.hasPet(petName)) {
                    adopter.returnPet(p);
                } else {
                    cout << "No such pet found in your adopted pets.\n";
                }
                break;
            }

            case 4: {
                cout << "Enter name: ";
                string name;
                getline(cin, name);
                system.searchPetByName(name);
                break;
            }

            case 5: {
                cout << "Enter type (Dog/Cat/Bird): ";
                string type;
                getline(cin, type);
                system.searchPetByType(type);
                break;
            }

            case 6:
                adopter.showAdoptedPets();
                break;

            case 7:
                system.savePetsToFile();
                cout << "Saved. Goodbye!\n";
                break;

            default:
                cout << "Invalid option. Please enter a number between 1 and 7.\n";
        }

    } while (choice != 7);

    return 0;
}  