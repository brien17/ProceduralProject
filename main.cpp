/**
 * @file ProceduralProject
 * @brief This file is production management software made for an imaginary company as an assignment for my COP 2001
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

//prototypes
void showMenu();

void getInput();

void addItem();

void produceItems();

std::string addLeadingZeros(int num);

/** The main method for my program that runs the other methods.
 *
 * @return
 */
int main() {
    //greeting the user
    std::cout << "Welcome to the Production Line Tracker\n" << std::endl;
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
                std::cout << "Display Production Statistics Stub" << std::endl;
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

    //creating vector to store item information
    //credit: jrohde
    //http://www.cplusplus.com/forum/beginner/17845/
    std::vector<std::string> items;

    //creating placeholder for file data
    std::string line;
    //reading file
    std::ifstream catalog("catalog.txt");

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

    //prompting user
    std::cout << "Select an available item" << std::endl;
    //displaying available items
    for (int i = 0; i < items.size(); i++) {
        std::cout << i + 1 << ". " << items[i] << std::endl;
    }

    //getting input from user
    int itemSelected;
    std::cin >> itemSelected;

    // potential index out of bounds here
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

    //opening file
    std::ofstream produced;
    produced.open("produced.txt", std::ios_base::app);

    //looping to output production number and serial number and write to file
    for (int i = 1; i <= numProduced; i++) {
        //creating production number
        int productionNumber = i;
        //outputting production number
        std::cout << "Production Number: " << productionNumber;
        //creating serial number
        std::string serialNumber = manufacturer.substr(0, 3) + itemTypeCode + addLeadingZeros(productionNumber);
        //outputting production number
        std::cout << " Serial Number: " << serialNumber << std::endl;
        //writing manufacturer, name, code, production number, and serial number file
        produced << "Manufacturer: " << manufacturer << ", Item name: " << itemName << ", Item Code: " <<
                 itemTypeCode << ", Production Number: " << productionNumber << ", Serial Number: " << serialNumber
                 << "\n";
    }

    //closing file
    produced.close();
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
        }
    }

    //creating file
    std::ofstream myFile;

    //opening file
    myFile.open("catalog.txt", std::ios_base::app);

    //writing to file
    myFile << manufacturer << "," << productName << "," << itemTypeCode << "\n";

    //closing file
    myFile.close();

    //providing feedback to user
    std::cout << "Item has been saved" << std::endl;


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

