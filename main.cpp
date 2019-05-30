/**
 * @file main.cpp
 * @brief The whole media production project.
 * This file is production management software made for an imaginary company as an assignment for my COP 2001
 * course.
 * @author Cameron Brien
 * @bug potential index error at line 120
 */

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

//prototypes
void showMenu();

void getInput();

void addItem();

void produceItems();

void production_statistics();

void show_statistics_menu();

void get_production_from_serial();

void show_available_products_sorted();

std::string addLeadingZeros(int num);

/** The main method for my program that runs the other methods.
 *
 * @return
 */
int main() {
    //greeting the user
    std::cout << "Welcome to the Production Line Tracker" << std::endl;
    //getting input from the user
    getInput();
    return 0;
}

/**
 * This method shows the main menu for the user to make selections from.
 */
void showMenu() {
    // printing a menu for the user
    std::cout << "1. Produce Items" << std::endl;
    std::cout << "2. Add Employee Account" << std::endl;
    std::cout << "3. Add New Product" << std::endl;
    std::cout << "4. Display Production Statistics" << std::endl;
    std::cout << "5. Exit" << std::endl;
}

/**
 * This method gets input from the user and returns the appropriate response or calls the appropriate
 * method based on the input.
 */
void getInput() {
    //declaring variable for user input
    int userInput;
    //declaring boolean to track if the user is done with the program
    bool exit = false;
    //looping to get input from the user
    while (exit == false) {
        //prompting the user
        std::cout << "Please select an option" << std::endl;
        //display a menu for the user showing the options
        showMenu();
        //reading input from cin
        std::cin >> userInput;
        //using input to select menu options
        switch (userInput) {
            case 1:
                produceItems();
                break;
            case 2:
                std::cout << "Add Employee Account Stub" << std::endl;
                break;
            case 3:
                addItem();
                break;
            case 4:
                production_statistics();
                break;
            case 5:
                exit = true;
                break;
            default:
                std::cout << "Not a valid selection" << std::endl;
        }
    }
}

/**
 * A method to track items that are produced and allow for the production of more items.
 */
void produceItems() {
    //greeting user
    std::cout << "Welcome to the production tab" << std::endl;
    std::cout << "Here is a list of items that are available for production" << std::endl;
    //make it so that it quits or goes to add item tab when there are no items to be produced

    //creating vector to store item information
    //credit: jrohde
    //http://www.cplusplus.com/forum/beginner/17845/
    std::vector<std::string> items;

    //ensuring good input
    unsigned int itemSelected = 0;
    bool goodInput = false;
    //looping until good input is recieved
    while (goodInput == false) {
        //making sure the vector is clear
        items.clear();
        //creating placeholder for file data
        std::string line;
        //reading file
        std::ifstream catalog("catalog.csv");

        //checking if file is open
        if (catalog.is_open()) {
            //declaring line counter
            int lineCounter = 0;
            //while there are more lines, takes the line and stores it as variable line
            while (getline(catalog, line)) {
                //storing line in array
                items.push_back(line);
                //incrementing counter
                lineCounter++;
            }
        }

        //closing file
        catalog.close();

        //entering letters makes it crash
        //prompting user
        std::cout << "Select an available item or add an item to be produced" << std::endl;
        //displaying available items
        for (unsigned int i = 0; i < items.size(); i++) {
            std::cout << i + 1 << ". " << items[i] << std::endl;
        }
        //displaying the option to add new items
        std::cout << items.size() + 1 << ". " << "Add new item" << std::endl;

        //getting input from user
        std::cin >> itemSelected;
        //checking if the user selection is in the array, is add new item, or is bad input
        if (itemSelected <= items.size() && itemSelected > 0) {
            goodInput = true;
        } else if (itemSelected == items.size() + 1) {
            addItem();
        } else {
            std::cout << "Input not understood" << std::endl;
        }
    }

    //selecting item based on user input
    std::string itemToProduce = items[itemSelected - 1]; // subtracting 1 because zero index

    //creating variables to hold information about the item
    std::string manufacturer;
    std::string itemName;
    std::string itemTypeCode;

    //assigning product data to variables
    std::stringstream stream(itemToProduce);
    getline(stream, manufacturer, ',');
    getline(stream, itemName, ',');
    getline(stream, itemTypeCode, ',');

    //prompting user for number of items produced and assigning to variable
    std::cout << "Enter the number of items that were produced" << std::endl;
    int numProduced;
    std::cin >> numProduced;

    //reading file
    std::ifstream production_number_in;
    production_number_in.open("production_number.txt");

    //creating a string to store the data temporarily
    std::string production_number_string;

    //getting data as a string
    getline(production_number_in, production_number_string);

    //creating a variable to hold production number
    int production_number = 1;

    //closing file
    production_number_in.close();

    //creating a string stream of the production number
    std::stringstream production_number_string_stream(production_number_string);

    //storing that stream in the variable production number
    production_number_string_stream >> production_number;

    //reading file
    std::ifstream producedIn;
    producedIn.open("produced.csv");

    //creating a string to hold the first part of a serial number to match with others that have already been produced
    std::string searchString = itemTypeCode;

    //creating placeholder for file data
    std::string line;

    //creating a variable to hold the serial number of the last of the created products
    std::string found = "not found";

    //creating a variable to hold number of the current line
    unsigned int currentLine = 0;

    //looping to find the last occurance of the serial number being searched for
    while (getline(producedIn, line)) {
        currentLine++;
        //checking if the line contains the searched for serial number
        if (line.find(searchString, 0) != std::string::npos) {
            //setting the line matching the looked for serial number to found
            found = line;
        }
    }

    int startingNumber;
    int endingNumber;

    if (found == "not found") {

        startingNumber = 0;

        endingNumber = numProduced;
    } else {
        //creating a string stream with just the last 5 digits of the serial number
        std::stringstream startingNumberStream(found.substr(found.length() - 5));

        //storing that stream in the variable starting number
        startingNumberStream >> startingNumber;

        //adding one to starting number
        startingNumber++;
        //
        endingNumber = startingNumber + numProduced;
    }

    //closing stream
    producedIn.close();

    //opening file
    std::ofstream producedOut;
    producedOut.open("produced.csv", std::ios_base::app);

    //opening file
    std::ofstream production_number_out;
    production_number_out.open("production_number.txt");

    //looping to output production number and serial number and write to file
    for (int i = startingNumber; i < endingNumber; i++) {
        //outputting production number
        std::cout << "Production Number: " << production_number;
        //creating serial number
        std::string serialNumber = manufacturer.substr(0, 3) + itemTypeCode + addLeadingZeros(i);
        //outputting production number
        std::cout << " Serial Number: " << serialNumber << std::endl;
        //writing manufacturer, name, code, production number, and serial number file
        producedOut << manufacturer << "," << itemName << "," << itemTypeCode << "," << production_number
                    << "," << serialNumber << "\n";
        //iterating production number
        ++production_number;
    }

    //writing to file
    production_number_out << production_number;

    //closing file
    producedOut.close();
    production_number_out.close();
}

/**
 * This method allows the user to add a new item that can be produced.
 */
void addItem() {
    // prompting the user for the manufacturer
    std::cout << "Enter the manufacturer" << std::endl;
    std::string manufacturer;
    std::cin >> manufacturer;

    //prompting the user for a product name
    std::cout << "Enter the product name" << std::endl;
    std::string productName;
    std::cin >> productName;

    //creating a variable to hold the users input
    int itemTypeChoice;
    //creating a variable to hold the item code
    std::string itemTypeCode;
    //creating a boolean for the loop
    bool gotten_input = false;
    //looping to ensure the user enters good input
    while (gotten_input == false) {
        //printing menu
        std::cout << "Enter the item type\n";
        std::cout << "1. Audio\n" <<
                  "2. Visual\n" <<
                  "3. AudioMobile\n" <<
                  "4. VisualMobile\n";
        //reading user choice
        std::cin >> itemTypeChoice;
        //switch to select correct item code from user choice
        switch (itemTypeChoice) {
            case 1:
                itemTypeCode = "MM";
                gotten_input = true;
                break;
            case 2:
                itemTypeCode = "VI";
                gotten_input = true;
                break;
            case 3:
                itemTypeCode = "AM";
                gotten_input = true;
                break;
            case 4:
                itemTypeCode = "VM";
                gotten_input = true;
                break;
            default:
                std::cout << "Input not understood" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    //creating file
    std::ofstream myFile;

    //opening file
    myFile.open("catalog.csv", std::ios_base::app);

    //writing to file
    myFile << manufacturer << "," << productName << "," << itemTypeCode << "\n";

    //closing file
    myFile.close();

    //providing feedback to user
    std::cout << "Item has been saved" << std::endl;


}

/**
 * This method gets input from the user and allows them to run other production statistics methods
 */
void production_statistics() {
    //declaring variable for user input
    int userInput;
    //declaring boolean to track if the user is done with the program
    bool exit = false;
    //looping to get input from the user
    while (exit == false) {
        //prompting the user
        std::cout << "Welcome to the production statistics tab" << std::endl;
        std::cout << "Please select an option to continue" << std::endl;
        //display a menu for the user showing the options
        show_statistics_menu();
        //reading input from cin
        std::cin >> userInput;
        //using input to select menu options
        switch (userInput) {
            case 1:
                get_production_from_serial();
                break;
            case 2:
                show_available_products_sorted();
                break;
            case 3:
                exit = true;
                break;
            default:
                std::cout << "Not a valid selection" << std::endl;
        }
    }
}

/**
 * This method shows the statistics sub-menu for the user to make selections from.
 */
void show_statistics_menu() {
    std::cout << "1. Get Production Number From Serial Number" << std::endl;
    std::cout << "2. Show Available Products" << std::endl;
    std::cout << "3. Exit" << std::endl;
}

/**
 * This method prompts the user for a serial number and then prints the production number for that serial number to the
 * console.
 */
void get_production_from_serial() {
    //creating a variable to hold the user input
    std::string serial_number = "Serial number not found";

    //prompting the user
    std::cout << "Please enter a serial number" << std::endl;

    //assigning their input to the variable
    std::cin >> serial_number;

    //reading file
    std::ifstream produced_in;
    produced_in.open("produced.csv");

    //creating placeholder for file data
    std::string line;

    //creating a variable to hold number of the current line
    unsigned int production_number = 0;

    //looping to find the serial number in the produced file
    while (getline(produced_in, line)) {
        //iterating the counter variable
        ++production_number;
        //checking if the line contains the searched for serial number
        if (line.find(serial_number, 0) != std::string::npos) {
            //breaking out of the loop if it matches
            break;
        }
    }
    std::cout << "The production number is, " << production_number << std::endl;

    produced_in.close();
}

/**
 * This method prints a sorted list of all of the products that are able to printed to the console.
 */
void show_available_products_sorted() {
    //printing to user
    std::cout << "Available items" << std::endl;

    //reading file
    std::ifstream catalog_in;
    catalog_in.open("catalog.csv");

    //creating a vector to hold the product names
    std::vector<std::string> product_line_names;

    //creating a temporary variable to hold the line
    std::string line;

    //looping over the catalog and adding the name to the vector
    while (getline(catalog_in, line)) {
        //creating a stream for the line
        std::stringstream stream(line);
        //getting the manufacturer
        getline(stream, line, ',');
        //getting the product name
        getline(stream, line, ',');
        product_line_names.push_back(line);
        //getting the item type code
        getline(stream, line, ',');
    }
    //sorting
    sort(product_line_names.begin(), product_line_names.end());
    //printing items
    for (std::string s : product_line_names)
        std::cout << s << "\n";
}

/**
 * A method to add leading zeros to an int and return a string that contains the int with
 * leading zeros up to five digits.
 * @param num The number to be converted to a string with leading zeros
 * @return the string made from the int
 */
std::string addLeadingZeros(int num) {
    //creating a string from the numbers
    std::string numString = std::to_string(num);
    //getting the length of the string
    int len = numString.length();
    //creating a string to hold the zeros
    std::string zerosString;
    //adding the right number of zeros to that string
    for (int i = len; i < 5; i++) {
        zerosString.append("0");
    }
    //combining the strings
    std::string finalString = zerosString + numString;
    //returning the result
    return finalString;
}