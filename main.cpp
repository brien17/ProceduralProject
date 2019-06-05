/**
 * @file main.cpp
 * @brief The whole media production project.
 * This file is production management software made for an imaginary company as an assignment for my COP 2001
 * course.
 * @author Cameron Brien
 * @bug
 */

#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

//prototypes
void show_menu();
void get_input();
void add_item();
void produce_items();
void production_statistics();
void show_statistics_menu();
void get_production_from_serial();
void show_available_products_sorted();
void add_employee_account();
std::string log_in();
std::string add_leading_zeros(int num);
std::string encrypt_password(std::string password);

/** The main method for my program that runs the other methods.
 *
 * @return
 */
int main() {
    //greeting the user
    std::cout << "Welcome to the Production Line Tracker" << std::endl;
    //getting input from the user
    get_input();
    return 0;
}

/**
 * This method shows the main menu for the user to make selections from.
 */
void show_menu() {
    // printing a menu for the user
    std::cout << "1. Produce Items" << std::endl;
    std::cout << "2. Add Employee Account" << std::endl;
    std::cout << "3. Add New Product" << std::endl;
    std::cout << "4. Display Production Statistics" << std::endl;
    std::cout << "5. Login" << std::endl;
    std::cout << "6. Exit" << std::endl;
}

/**
 * This method gets input from the user and returns the appropriate response or calls the appropriate
 * method based on the input.
 */
void get_input() {
    //declaring variable for user input
    int user_input;
    //declaring a variable to hold user name
    std::string current_user;
    //declaring boolean to track if the user is done with the program
    bool exit = false;
    //looping to get input from the user
    while (!exit) {
        //prompting the user
        std::cout << "Please select an option" << std::endl;
        //display a menu for the user showing the options
        show_menu();
        //reading input from cin
        std::cin >> user_input;
        //using input to select menu options
        switch (user_input) {
            case 1:
                produce_items();
                break;
            case 2:
                add_employee_account();
                break;
            case 3:
                add_item();
                break;
            case 4:
                production_statistics();
                break;
            case 5:
                current_user = log_in();
                break;
            case 6:
                exit = true;
                break;
            default:
                std::cout << "Not a valid selection\n" << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }
}

/**
 * A method to track items that are produced and allow for the production of more items.
 */
void produce_items() {
    //greeting user
    std::cout << "Welcome to the production tab" << std::endl;
    std::cout << "Here is a list of items that are available for production" << std::endl;

    //creating vector to store item information
    //credit: jrohde
    //http://www.cplusplus.com/forum/beginner/17845/
    std::vector<std::string> items;

    //ensuring good input
    unsigned int item_selected = 0;
    bool good_input = false;
    //looping until good input is received
    while (!good_input) {
        //making sure the vector is clear
        items.clear();

        //creating placeholder for file data
        std::string line;

        //reading file
        std::ifstream catalog("catalog.csv");

        //checking if file is open
        if (catalog.is_open()) {
            //while there are more lines, takes the line and stores it as variable line
            while (getline(catalog, line)) {
                //storing line in array
                items.push_back(line);
            }
        }

        //closing file
        catalog.close();

        //prompting user
        std::cout << "Select an available item or add an item to be produced" << std::endl;
        //displaying available items
        for (unsigned int i = 0; i < items.size(); i++) {
            std::cout << i + 1 << ". " << items[i] << std::endl;
        }
        //displaying the option to add new items
        std::cout << items.size() + 1 << ". " << "Add new item" << std::endl;

        //checking if the user inputs the right type of data
        if (!(std::cin >> item_selected)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        //checking if the user selection is in the array, is add new item, or is bad input
        if (item_selected <= items.size() && item_selected > 0) {
            good_input = true;
        } else if (item_selected == items.size() + 1) {
            add_item();
        } else {
            std::cout << "Input not understood\n" << std::endl;
        }
    }

    //selecting item based on user input
    std::string item_to_produce = items[item_selected - 1]; // subtracting 1 because zero index

    //creating variables to hold information about the item
    std::string manufacturer;
    std::string item_name;
    std::string item_type_code;

    //assigning product data to variables
    std::stringstream stream(item_to_produce);
    getline(stream, manufacturer, ',');
    getline(stream, item_name, ',');
    getline(stream, item_type_code, ',');

    //prompting user for number of items produced and assigning to variable
    std::cout << "Enter the number of items that were produced" << std::endl;
    int num_produced;
    std::cin >> num_produced;

    //reading file
    std::ifstream produced_in;
    produced_in.open("produced.csv");

    //creating a string to hold the first part of a serial number to match with others that have already been produced
    std::string search_string = item_type_code;

    //creating placeholder for file data
    std::string line;

    //creating a variable to hold the serial number of the last of the created products
    std::string found = "not found";

    //creating a variable to hold number of the current line
    unsigned int production_number = 1;

    //looping to find the last occurrence of the serial number being searched for
    while (getline(produced_in, line)) {
        production_number++;
        //checking if the line contains the searched for serial number
        if (line.find(search_string, 0) != std::string::npos) {
            //setting the line matching the looked for serial number to found
            found = line;
        }
    }

    int starting_number;
    int ending_number;

    if (found == "not found") {

        starting_number = 0;

        ending_number = num_produced;
    } else {
        //creating a string stream with just the last 5 digits of the serial number
        std::stringstream startingNumberStream(found.substr(found.length() - 5));

        //storing that stream in the variable starting number
        startingNumberStream >> starting_number;

        //adding one to starting number
        starting_number++;
        //
        ending_number = starting_number + num_produced;
    }

    //closing stream
    produced_in.close();

    //opening file
    std::ofstream produced_out;
    produced_out.open("produced.csv", std::ios_base::app);

    //looping to output production number and serial number and write to file
    for (int i = starting_number; i < ending_number; i++) {
        //outputting production number
        std::cout << "Production Number: " << production_number;
        //creating serial number
        std::string serialNumber = manufacturer.substr(0, 3) + item_type_code + add_leading_zeros(i);
        //outputting production number
        std::cout << " Serial Number: " << serialNumber << std::endl;
        //writing manufacturer, name, code, production number, and serial number file
        produced_out << manufacturer << "," << item_name << "," << item_type_code << "," << production_number
                     << "," << serialNumber << "\n";
        //iterating production number
        ++production_number;
    }

}

/**
 * This method allows the user to add a new item that can be produced.
 */
void add_item() {
    std::cin.ignore();
    // prompting the user for the manufacturer
    std::cout << "Enter the manufacturer" << std::endl;
    std::string manufacturer;
    std::getline(std::cin, manufacturer);

    //prompting the user for a product name
    std::cout << "Enter the product name" << std::endl;
    std::string product_name;
    std::getline(std::cin, product_name);

    //creating a variable to hold the users input
    int item_type_choice;
    //creating a variable to hold the item code
    std::string item_type_code;
    //creating a boolean for the loop
    bool gotten_input = false;
    //looping to ensure the user enters good input
    while (!gotten_input) {
        //printing menu
        std::cout << "Enter the item type\n";
        std::cout << "1. Audio\n" <<
                  "2. Visual\n" <<
                  "3. AudioMobile\n" <<
                  "4. VisualMobile\n";
        //reading user choice
        std::cin >> item_type_choice;
        //switch to select correct item code from user choice
        switch (item_type_choice) {
            case 1:
                item_type_code = "MM";
                gotten_input = true;
                break;
            case 2:
                item_type_code = "VI";
                gotten_input = true;
                break;
            case 3:
                item_type_code = "AM";
                gotten_input = true;
                break;
            case 4:
                item_type_code = "VM";
                gotten_input = true;
                break;
            default:
                std::cout << "Input not understood\n" << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }

    //creating file
    std::ofstream catalog_out;

    //opening file
    catalog_out.open("catalog.csv", std::ios_base::app);

    //writing to file
    catalog_out << manufacturer << "," << product_name << "," << item_type_code << "\n";

    //closing file
    catalog_out.close();

    //providing feedback to user
    std::cout << "Item has been saved" << std::endl;
}

/**
 * This method gets input from the user and allows them to run other production statistics methods
 */
void production_statistics() {
    //declaring variable for user input
    int user_input;
    //declaring boolean to track if the user is done with the program
    bool exit = false;
    //looping to get input from the user
    while (!exit) {
        //prompting the user
        std::cout << "Welcome to the production statistics tab" << std::endl;
        std::cout << "Please select an option to continue" << std::endl;
        //display a menu for the user showing the options
        show_statistics_menu();
        //reading input from cin
        std::cin >> user_input;
        //using input to select menu options
        switch (user_input) {
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
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }
}

/**
 * This method shows the statistics sub-menu for the user to make selections from.
 */
void show_statistics_menu() {
    std::cout << "1. Get Production Number From Serial Number" << std::endl;
    std::cout << "2. Show Available Products" << std::endl;
    std::cout << "3. Return to Main Menu" << std::endl;
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
    std::cout << "Available items:" << std::endl;

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
    }
    //sorting
    sort(product_line_names.begin(), product_line_names.end());
    //printing items
    for (std::string &s : product_line_names)
        std::cout << s << "\n";
}

/**
 * This method allows the user to add a new user name and password, then it encrypts the password and writes the user
 * name and password to a file.
 */
void add_employee_account() {
    //clearing buffer
    std::cin.ignore();
    //giving instructions to the user
    std::cout << "Please enter you first name and last name" << std::endl;
    //creating first and last name
    std::string first_name;
    std::string last_name;
    //getting input
    std::cin >> first_name;
    std::cin >> last_name;
    //making first and last name lowercase
    std::transform(first_name.begin(), first_name.end(), first_name.begin(), ::tolower);
    std::transform(last_name.begin(), last_name.end(), last_name.begin(), ::tolower);
    //creating user name
    std::string user_name = first_name.at(0) + last_name;
    //prompting user
    std::cout << "Please enter a password (must contain a number, upper and lower case letters, and a symbol"
              << std::endl;
    bool good_password = false;
    //creating a string for password
    std::string password;
    while (!good_password) {
        std::cin.ignore();
        //getting line from input
        std::cin >> password;
        //creating test variables
        bool lower = false;
        bool upper = false;
        bool number = false;
        bool symbol = false;
        bool valid = false;
        //looping to test each character of the password to see if it is legal
        for (int i = 0; i < password.size(); i++){
            if (islower(password[i])){
                lower = true;
            } else if (isupper(password[i])){
                upper = true;
            }else if (isdigit(password[i])){
                number = true;
            } else {
                symbol = true;
            }
        }
        //if there are no issues with the product
        if (lower && upper && number && !symbol) {
            valid = true;
        }
        //if the password is valid or not
        if (valid) {
            good_password = true;
        } else {
            std::cout << "Password not accepted" << std::endl;
        }
    }
    //encrypting password
    std::string encrypted_password = encrypt_password(password);

    //creating file
    std::ofstream employee_accounts_out;

    //opening file
    employee_accounts_out.open("employee_accounts.txt", std::ios_base::app);

    //writing to file
    employee_accounts_out << user_name << "," << encrypted_password << std::endl;

    //closing file
    employee_accounts_out.close();

    //providing feedback to user
    std::cout << "User name and password have been saved" << std::endl;
}

/**
 * This method prompts the user for a username and password and if it is correct allows them to log in and returns their
 * user name as a string.
 * @return The user name of the user
 */
std::string log_in(){
    //declaring a variable to hold user name
    std::string user_name;
    //declaring a variable to hold user name
    std::string password;
    //creating a variable to hold the string being searched for
    std::string found = "not found";
    //creating file
    std::ifstream employee_accounts_in;
    //opening file
    employee_accounts_in.open("employee_accounts.txt");
    //creating a variable to hold a line
    std::string line;
    //checking if file is open
    if (employee_accounts_in.is_open()) {
        //looping until a valid login
        while (found == "not found") {
            //resetting back to the beginning of the file
            employee_accounts_in.clear();
            employee_accounts_in.seekg(0, std::ios::beg);
            //prompting user for user name
            std::cout << "Enter your user name" << std::endl;
            //storing input as user name
            std::cin >> user_name;
            //prompting user for password
            std::cout << "Enter your password" << std::endl;
            //storing input as user name
            std::cin >> password;
            //encrypting password
            std::string encrypted_password = encrypt_password(password);
            //creating a string to hold the information being searched for
            std::string search_string = user_name.append("," + encrypted_password);
            //looping to search for the password and user name
            while (getline(employee_accounts_in, line)) {
                //checking if the line contains the searched for serial number
                if (line.find(search_string, 0) != std::string::npos) {
                    //setting the line matching the looked for serial number to found
                    found = line;
                    std:: cout << "Login successful" << std::endl;
                }
            }
            std::cout << "user name or password is incorrect" << std::endl;
        }
        //closing file
        employee_accounts_in.close();
        //returning user name
        return user_name;
    }else{
        return "";
    }
}

/**
 * A method to add leading zeros to an int and return a string that contains the int with
 * leading zeros up to five digits.
 * @param num The number to be converted to a string with leading zeros
 * @return the string made from the int
 */
std::string add_leading_zeros(int num) {
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

/**
 * This method encrypts the users password, passed as a string and returns the encrypted password as a string.
 * @param password The users password
 * @return The encrypted password
 */
std::string encrypt_password(std::string password){
    int encrypt_number = 3;

    if (password.length() == 1){
        return password;
    } else {
        // get Ascii code of first letter by casting char to int
        int code = (int)password[0];
        // shift the Ascii code by adding the encrypt number
        code += encrypt_number;
        // convert the new Ascii code to a character by casting int to char
        char code_char = (char)code;
        // return the decrypted char + a recursive call to decrypt the next char
        return (code_char + encrypt_password(password.substr(1, password.length() - 1)));
    }
}