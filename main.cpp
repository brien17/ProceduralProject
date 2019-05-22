#include <iostream>

//prototypes
void showMenu();
void getInput();
void addItem();
void produceItems();

/** The main method for my program that runs the other methods.
 *
 * @return
 */
int main() {
    //greeting the user
    std::cout << "Production Line Tracker\n" << std::endl;
    getInput();
    return 0;
}

/**
 * This method shows the main menu for the user to make selections from.
 */
void showMenu(){
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
void getInput(){
    int userInput;
    bool gotten_input = false;
    //looping to get input from the user
    while (gotten_input == false){
        //display a menu for the user showing the options
        showMenu();
        //reading input from cin
        std::cin >> userInput;
        //using input to select menu options
        switch (userInput) {
            case 1:
                std::cout << "Produce Items Stub" << std::endl;
                produceItems();
                break;
            case 2:
                std::cout << "Add Employee Account Stub" << std::endl;
                break;
            case 3:
                std::cout << "Add New Product Stub" << std::endl;
                addItem();
                break;
            case 4:
                std::cout << "Display Production Statistics Stub" << std::endl;
                break;
            case 5:
                gotten_input = true;
                break;
            default:
                std::cout << "Not a valid selection" << std::endl;
        }
    }
}


void produceItems(){

}

/**
 * This method allows the user to add a new item that can be produced.
 */
void addItem(){

}