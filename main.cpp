/**
 * @file main.cpp
 * @brief This is a production tracker for a fictional company to keep track of production line and products produced.
 * This file is production management software made for an imaginary company as an assignment for my COP 2001
 * course. The purpose of this project is to allow for this imaginary company to be able to track production of
 * products, the catalog of products, and the employee records.
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

//creating structs to hold data
struct Item {
    std::string manufacturer;
    std::string item_name;
    std::string item_type_code;
};
struct Product {
    std::string manufacturer;
    std::string item_name;
    std::string item_type_code;
    std::string production_number;
    std::string serial_number;
};
struct Stats {
    int MM;
    int VI;
    int VM;
    int AM;
    int total_produced;
};

//prototypes
void get_input();

void add_item(std::vector<Item> &);

void remove_item(std::vector<Item> &, const std::string &);

void produce_items(std::vector<Item> &, Stats &);

void production_statistics(const Stats &stats, const std::vector<Item> &);

void get_production_from_serial(const std::vector<Product> &);

void show_available_products_sorted(const std::vector<Item> &);

void add_employee_account();

std::string log_in();

std::string add_leading_zeros(int num);

std::string encrypt_password(std::string password);

std::vector<Item> load_catalog();

std::vector<Product> load_production_log();

Stats load_stats();

void print_file(const std::string &);

void print_produced_by_manufacturer(const std::vector<Product> &);

void print_produced_by_item_name(const std::vector<Product> &);

/**
 * @brief The main method for my program that runs the other methods.
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
 * @brief This method gets input from the user and returns the appropriate response or calls the appropriate
 * method based on the input.
 */
void get_input() {
    //initializing the catalog
    std::vector<Item> catalog = load_catalog();
    //initializing the stats
    Stats stats = load_stats();
    //declaring variable for user input
    int user_input;
    //declaring a variable to hold user name
    std::string current_user;
    //declaring boolean to track if the user is done with the program
    bool exit = false;
    //looping to get input from the user
    while (!exit) {
        //prompting the user and displaying a menu showing options
        std::cout << '\n'; //leaving a blank line for readability for the user
        std::cout << "Please select an option" << std::endl;
        std::cout << "1. Produce Items" << std::endl;
        std::cout << "2. Add Employee Account" << std::endl;
        std::cout << "3. Add New Product" << std::endl;
        std::cout << "4. Remove a Product" << std::endl;
        std::cout << "5. Display Production Statistics" << std::endl;
        std::cout << "6. Login" << std::endl;
        std::cout << "7. Exit" << std::endl;
        //reading input from cin
        std::cin >> user_input;
        //using input to select menu options
        switch (user_input) {
            case 1:
                produce_items(catalog, stats);
                break;
            case 2:
                add_employee_account();
                break;
            case 3:
                add_item(catalog);
                break;
            case 4:
                remove_item(catalog, current_user);
                break;
            case 5:
                production_statistics(stats, catalog);
                break;
            case 6:
                current_user = log_in();
                break;
            case 7:
                exit = true;
                break;
            default:
                std::cout << "Not a valid selection" << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }
    //printing exit message
    std::cout << "Goodbye " << current_user << std::endl;
}

/**
 * @brief This method allows the user to select a product and specify the number of items produced and then writes that
 * information along with the serial numbers to a file.
 * @param catalog The catalog of available products
 * @param stats The production number and numbers produced for each item type
 */
void produce_items(std::vector<Item> &catalog, Stats &stats) {
    //greeting user
    std::cout << "Welcome to the production tab" << std::endl;
    std::cout << "Here is a list of items that are available for production" << std::endl;

    //ensuring good input
    unsigned int user_input = 0;
    bool good_input = false;
    //looping until good input is received
    while (!good_input) {
        //prompting user
        std::cout << "Select an available item or add an item to be produced" << std::endl;
        //displaying available items
        for (unsigned int i = 0; i < catalog.size(); i++) {
            std::cout << i + 1 << ". " << catalog[i].manufacturer << ", " << catalog[i].item_name << ", "
                      << catalog[i].item_type_code << std::endl;
        }
        //displaying the option to add new items
        std::cout << catalog.size() + 1 << ". " << "Add new item" << std::endl;

        //checking if the user inputs the right type of data
        if (!(std::cin >> user_input)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }

        //checking if the user selection is in the array, is add new item, or is bad input
        if (user_input <= catalog.size() && user_input > 0) {
            good_input = true;
        } else if (user_input == catalog.size() + 1) {
            add_item(catalog);
        } else {
            std::cout << "Input not understood" << std::endl;
        }
    }

    //selecting item based on user input
    Item item_to_produce = catalog[user_input - 1]; // subtracting 1 because zero index

    //prompting user for number of items produced and assigning to variable
    std::cout << "Enter the number of items that were produced" << std::endl;
    int num_produced;
    std::cin >> num_produced;

    //setting starting and ending numbers
    int starting_number = stats.total_produced + 1;
    int ending_number = starting_number + num_produced;

    //getting the number produced for that category to us in serial number and updating the stats
    int num_produced_this_category;
    if (item_to_produce.item_type_code == "MM") {
        num_produced_this_category = stats.MM;
        stats.MM += num_produced;
    } else if (item_to_produce.item_type_code == "VI") {
        num_produced_this_category = stats.VI;
        stats.VI += num_produced;
    } else if (item_to_produce.item_type_code == "AM") {
        num_produced_this_category = stats.AM;
        stats.AM += num_produced;
    } else if (item_to_produce.item_type_code == "VM") {
        num_produced_this_category = stats.VM;
        stats.VM += num_produced;
    } else {
        num_produced_this_category = 0;
        std::cout << "Error: serial number not valid" << std::endl; //should not get here unless something goes wrong
    }

    //opening files
    std::ofstream produced_out;
    produced_out.open("ProductionLog.csv", std::ios_base::app);
    std::ofstream stats_out;
    stats_out.open("stats.txt");

    //looping to output production number and serial number and write to production file
    for (int i = starting_number; i < ending_number; i++) {
        //incrementing total
        stats.total_produced++;
        //outputting production number
        //std::cout << "Production Number: " << stats.total_produced;
        //creating serial number
        std::string serial_number =
                item_to_produce.manufacturer.substr(0, 3) + item_to_produce.item_type_code +
                add_leading_zeros(num_produced_this_category);
        //outputting production number
        //std::cout << " Serial Number: " << serial_number << std::endl;
        //writing manufacturer, name, code, production number, and serial number file
        produced_out << item_to_produce.manufacturer << "," << item_to_produce.item_name << ","
                     << item_to_produce.item_type_code << "," << stats.total_produced
                     << "," << serial_number << "\n";
        //incrementing variables
        num_produced_this_category++;
    }
    //writing to stats file
    stats_out << stats.MM << ", " << stats.VI << ", " << stats.AM << ", " << stats.VM << ", " << stats.total_produced
              << std::endl;
    //giving feedback to user
    std::cout << "Saved to file" << std::endl;
    //closing out files
    produced_out.close();
    stats_out.close();
}

/**
 * @brief This method allows the user to add a new item that can be produced.
 * @param catalog The catalog of available products
 */
void add_item(std::vector<Item> &catalog) {
    //clearing the buffer
    std::cin.ignore();
    //creating a new item
    Item new_item;
    // prompting the user for the manufacturer
    std::cout << "Enter the manufacturer" << std::endl;
    std::getline(std::cin, new_item.manufacturer);

    //prompting the user for a product name
    std::cout << "Enter the product name" << std::endl;
    std::getline(std::cin, new_item.item_name);

    //creating a variable to hold the users input
    int item_type_choice;
    //creating a boolean for the loop
    bool gotten_input = false;
    //looping to ensure the user enters good input
    while (!gotten_input) {
        //printing menu
        std::cout << "Enter the item type" << std::endl;
        std::cout << "1. Audio" << std::endl;
        std::cout << "2. Visual" << std::endl;
        std::cout << "3. AudioMobile" << std::endl;
        std::cout << "4. VisualMobile" << std::endl;
        //reading user choice
        std::cin >> item_type_choice;
        //switch to select correct item code from user choice
        switch (item_type_choice) {
            case 1:
                new_item.item_type_code = "MM";
                gotten_input = true;
                break;
            case 2:
                new_item.item_type_code = "VI";
                gotten_input = true;
                break;
            case 3:
                new_item.item_type_code = "AM";
                gotten_input = true;
                break;
            case 4:
                new_item.item_type_code = "VM";
                gotten_input = true;
                break;
            default:
                std::cout << "Input not understood\n" << std::endl;
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }

    //updating catalog
    catalog.push_back(new_item);

    //opening file
    std::ofstream catalog_out;
    catalog_out.open("ProductLine.csv", std::ios_base::app);

    //writing to file
    catalog_out << new_item.manufacturer << "," << new_item.item_name << "," << new_item.item_type_code << "\n";

    //closing file
    catalog_out.close();

    //providing feedback to user
    std::cout << "Item has been saved" << std::endl;
}

/**
 * @brief This method removes an item from the catalog that is specified by the user.
 * @param catalog The catalog of items
 */
void remove_item(std::vector<Item> &catalog, const std::string &current_user) {
    if (current_user.empty()) {
        std::cout << "You must be logged in to remove items" << std::endl;
    } else {
        //ensuring good input
        unsigned int user_input = 0;
        bool good_input = false;
        //looping until good input is received
        while (!good_input) {
            //prompting user
            std::cout << "Select an available item to be deleted" << std::endl;
            //displaying available items
            for (unsigned int i = 0; i < catalog.size(); i++) {
                std::cout << i + 1 << ". " << catalog[i].manufacturer << ", " << catalog[i].item_name << ", "
                          << catalog[i].item_type_code << std::endl;
            }

            //checking if the user inputs the right type of data
            if (!(std::cin >> user_input)) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Input not understood" << std::endl;
            }

            //checking if the user selection is in the array
            if (user_input <= catalog.size() && user_input > 0) {
                std::vector<Item>::iterator it1;
                it1 = catalog.begin();
                catalog.erase(it1 + user_input - 1);
                good_input = true;
            } else {
                std::cout << "Input not understood" << std::endl;
            }
        }
        //opening file
        std::ofstream catalog_out;
        catalog_out.open("ProductLine.csv", std::ios::trunc);

        for (const Item &item : catalog) {
            //writing to file
            catalog_out << item.manufacturer << "," << item.item_name << "," << item.item_type_code << "\n";
        }
        //closing file
        catalog_out.close();

        //providing feedback to user
        std::cout << "Item has been erased" << std::endl;
    }
}

/**
 * @brief This method gets input from the user and allows them to run other production statistics methods.
 * @param stats The production number and numbers produced for each item type
 */
void production_statistics(const Stats &stats, const std::vector<Item> &catalog) {
    //loading the production log
    std::vector<Product> production_log = load_production_log();
    //declaring variable for user input
    int user_input;
    //declaring boolean to track if the user is done with the program
    bool exit = false;
    //looping to get input from the user
    while (!exit) {
        //prompting the user and showing them options
        std::cout << "\n"; //leaving a blank line for readability for the user
        std::cout << "Welcome to the production statistics tab" << std::endl;
        std::cout << "Please select an option to continue" << std::endl;
        std::cout << "1. Get Production Number From Serial Number" << std::endl;
        std::cout << "2. Show Available Products" << std::endl;
        std::cout << "3. Show Catalog" << std::endl;
        std::cout << "4. Show Production Log" << std::endl;
        std::cout << "5. Show Total Number of Products Produced" << std::endl;
        std::cout << "6. Show Number of Products Produced for a Specific Manufacturer" << std::endl;
        std::cout << "7. Show Number of an Item Produced" << std::endl;
        std::cout << "8. Return to Main Menu" << std::endl;
        //reading input from cin
        std::cin >> user_input;
        //using input to select menu options
        switch (user_input) {
            case 1:
                get_production_from_serial(production_log);
                break;
            case 2:
                show_available_products_sorted(catalog);
                break;
            case 3:
                print_file("ProductLine.csv");
                break;
            case 4:
                print_file("ProductionLog.csv");
                break;
            case 5:
                std::cout << "Total Products: " << stats.total_produced << std::endl;
                std::cout << "Audio: " << stats.MM << std::endl;
                std::cout << "Visual: " << stats.VI << std::endl;
                std::cout << "Audio Mobile: " << stats.AM << std::endl;
                std::cout << "Visual Mobile: " << stats.VM << std::endl;
                std::cout << std::endl;
                break;
            case 6:
                print_produced_by_manufacturer(production_log);
                break;
            case 7:
                print_produced_by_item_name(production_log);
                break;
            case 8:
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
 * @brief This method prompts the user for a serial number and then prints the production number for that serial number
 * to the console.
 */
void get_production_from_serial(const std::vector<Product> &production_log) {
    //creating a variable to hold the user input
    std::string serial_number;

    //prompting the user
    std::cout << "Please enter a serial number" << std::endl;

    //assigning their input to the variable
    std::cin >> serial_number;

    //creating a boolean to test if the serial number was found
    bool found = false;
    //looping to find serial number
    for (const Product &product : production_log) {
        if (product.serial_number == serial_number) {
            std::cout << "The production number is: " << product.production_number << std::endl;
            found = true;
        }
    }
    //telling the user if the serial number was not found
    if (!found) {
        std::cout << "Serial number not found" << std::endl;
    }
}

/**
 * @brief This method prints a sorted list of all of the products that are able to printed to the console.
 */
void show_available_products_sorted(const std::vector<Item> &catalog) {
    //printing to user
    std::cout << "Available items:" << std::endl;

    //creating a vector to hold the product names
    std::vector<std::string> product_line_names(catalog.size());

    for (const Item &product : catalog) {
        product_line_names.push_back(product.item_name);
    }
    //sorting
    sort(product_line_names.begin(), product_line_names.end());
    //printing items
    for (std::string &s : product_line_names)
        std::cout << s << std::endl;
}

/**
 * @brief This method allows the user to add a new user name and password, then it encrypts the password and writes the
 * user name and password to a file.
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
    //showing the user their user name
    std::cout << "Your user name is " << user_name << std::endl;
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
        //looping to test each character of the password to see if it is legal
        for (char c : password) {
            if (islower(c)) {
                lower = true;
            } else if (isupper(c)) {
                upper = true;
            } else if (isdigit(c)) {
                number = true;
            } else {
                symbol = true;
            }
        }
        //if there are no issues with the password
        if (lower && upper && number && !symbol) {
            //pushing the password off the screen (would like to be able to clear console later)
            std::cout << std::string(1000, '\n');
            good_password = true;
            std::cout << "Password accepted" << std::endl;
        } else {
            std::cout << "Password not accepted" << std::endl;
            std::cout << "Please try again" << std::endl;
        }
    }
    //encrypting password
    std::string encrypted_password = encrypt_password(password);

    //creating file
    std::ofstream employee_accounts_out;

    //opening file
    employee_accounts_out.open("Users.txt", std::ios_base::app);

    //writing to file
    employee_accounts_out << user_name << "," << encrypted_password << std::endl;

    //closing file
    employee_accounts_out.close();

    //providing feedback to user
    std::cout << "User name and password have been saved" << std::endl;
}

/**
 * @brief This method prompts the user for a username and password and if it is correct allows them to log in and
 * returns their user name as a string.
 * @return The user name of the user
 */
std::string log_in() {
    //declaring a variable to hold user name
    std::string user_name;
    //declaring a variable to hold user name
    std::string password;
    //creating a variable to hold the string being searched for
    std::string found = "not found";
    //opening file
    std::ifstream employee_accounts_in;
    employee_accounts_in.open("Users.txt");
    //creating a variable to hold a line
    std::string line;
    //checking if file is open
    if (employee_accounts_in.is_open()) {
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
        std::string search_string = user_name;
        search_string.append("," + encrypted_password);
        //looping to search for the password and user name
        while (getline(employee_accounts_in, line)) {
            //checking if the line contains the searched for serial number
            if (line == search_string) {
                //setting the line matching the looked for serial number to found
                found = line;
                //pushing the password off the screen (would like to be able to clear console later)
                std::cout << std::string(1000, '\n');
                std::cout << "Login successful" << std::endl;
                std::cout << "Welcome " << user_name << std::endl;
                //returning user name
                return user_name;
            }
        }
        //pushing password off the screen
        std::cout << std::string(1000, '\n');
        std::cout << "User name or password is incorrect" << std::endl;

        //closing file
        employee_accounts_in.close();
        //returning nothing of no user name
        return "";
    } else {
        std::cout << "No users found, please add an account before logging in" << std::endl;
        return "";
    }
}

/**
 * @brief A method to add leading zeros to an int and return a string that contains the int with leading zeros up to
 * five digits.
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
 * @brief This method encrypts the users password, passed as a string and returns the encrypted password as a string.
 * @param password The users password
 * @return The encrypted password
 */
std::string encrypt_password(std::string password) {
    int encrypt_number = 3;

    if (password.length() == 1) {
        return password;
    } else {
        // get Ascii code of first letter by casting char to int
        int code = (int) password[0];
        // shift the Ascii code by adding the encrypt number
        code += encrypt_number;
        // convert the new Ascii code to a character by casting int to char
        char code_char = (char) code;
        // return the decrypted char + a recursive call to decrypt the next char
        return (code_char + encrypt_password(password.substr(1, password.length() - 1)));
    }
}

/**
 * @brief This is a method to load the catalog from a file and store it in a vector containing structs.
 * @return A vector containing structs that store the catalog items
 */
std::vector<Item> load_catalog() {
    //creating the vector of items
    std::vector<Item> catalog;

    //creating variables to hold information
    std::string line;

    //reading file
    std::ifstream catalog_in("ProductLine.csv");

    //checking if file is open
    if (catalog_in.is_open()) {
        //while there are more lines, takes the line and stores it as variable line
        while (getline(catalog_in, line)) {
            Item temp_struct;
            //assigning product data to struct
            std::stringstream stream(line);
            getline(stream, temp_struct.manufacturer, ',');
            getline(stream, temp_struct.item_name, ',');
            getline(stream, temp_struct.item_type_code, ',');

            //pushing struct to vector
            catalog.push_back(temp_struct);
        }
    }

    //closing file
    catalog_in.close();

    //returning the catalog
    return catalog;
}

/**
 * @brief This is a method to load the production log from a file and store it in a vector containing structs.
 * @return A vector containing structs that store the production log
 */
std::vector<Product> load_production_log() {
    //creating the vector of items
    std::vector<Product> production_log;

    //creating variables to hold information
    std::string line;

    //reading file
    std::ifstream production_log_in("ProductionLog.csv");

    //checking if file is open
    if (production_log_in.is_open()) {
        //while there are more lines, takes the line and stores it as variable line
        while (getline(production_log_in, line)) {
            Product temp_struct;
            //assigning product data to struct
            std::stringstream stream(line);
            getline(stream, temp_struct.manufacturer, ',');
            getline(stream, temp_struct.item_name, ',');
            getline(stream, temp_struct.item_type_code, ',');
            getline(stream, temp_struct.production_number, ',');
            getline(stream, temp_struct.serial_number, ',');
            //pushing struct to vector
            production_log.push_back(temp_struct);
        }
    }

    //closing file
    production_log_in.close();

    //returning the catalog
    return production_log;
}

/**
 * @brief This is a method to load the production statistics from a file and store them in a struct.
 * @return A struct containing production statistics
 */
Stats load_stats() {
    //creating variables to hold information
    Stats stats = {0, 0, 0, 0, 0};
    std::string line;

    //reading file
    std::ifstream stats_in("stats.txt");

    //checking if file is open
    if (stats_in.is_open()) {
        //takes the line and stores it as variable line and stores that variable in the proper part of the struct
        getline(stats_in, line, ',');
        stats.MM = stoi(line);
        getline(stats_in, line, ',');
        stats.VI = stoi(line);
        getline(stats_in, line, ',');
        stats.AM = stoi(line);
        getline(stats_in, line, ',');
        stats.VM = stoi(line);
        getline(stats_in, line, ',');
        stats.total_produced = stoi(line);
        //closing file
        stats_in.close();
    } else {
        //setting variables to initial values
        stats.MM = 0;
        stats.VI = 0;
        stats.AM = 0;
        stats.VM = 0;
        stats.total_produced = 0;
        //creating stats file
        std::ofstream stats_out;
        stats_out.open("stats.txt");
        //writing to file
        stats_out << stats.MM << "," << stats.VI << "," << stats.AM << "," << stats.VM << ","
                  << stats.total_produced << std::endl;
        //closing file
        stats_out.close();
    }

    //returning the catalog
    return stats;
}

/**
 * @brief This method prints the contents of a file that is passed as a parameter.
 * @param file_path The name of the file to be read
 */
void print_file(const std::string &file_path) {
    //opening file
    std::ifstream file_in;
    file_in.open(file_path);
    //creating a variable to hold a line
    std::string line;
    //checking if file is open
    if (file_in.is_open()) {
        std::cout << "Available products:" << std::endl;
        while (getline(file_in, line)) {
            std::cout << line << std::endl;
        }
        std::cout << "\n";
    } else {
        std::cout << "File could not be opened" << std::endl;
    }
}

/**
 * @brief This method displays the number of items produced of for a specific manufacturer entered by the user.
 * @param production_log A vector containing structs that store the production log information
 */
void print_produced_by_manufacturer(const std::vector<Product> &production_log) {
    //creating variable to hold data
    std::string manufacturer;
    //prompting user
    std::cout << "Enter the manufacturer to view the number of items produced for that manufacturer" << std::endl;
    //getting input
    std::cin >> manufacturer;
    //initializing iterator
    int num_produced = 0;
    //looping over the produced items
    for (const Product &product : production_log) {
        if (product.manufacturer == manufacturer) {
            num_produced++;
        }
    }
    //printing output to user
    std::cout << "We have made " << num_produced << " products for " << manufacturer << std::endl;
}

/**
 * @brief This method displays the number of items produced of a specific item name entered by the user.
 * @param production_log A vector containing structs that store the production log information
 */
void print_produced_by_item_name(const std::vector<Product> &production_log) {
    //creating variable to hold data
    std::string item_name;
    //prompting user
    std::cout << "Enter the item name to view the number of that item produced" << std::endl;
    //getting input
    std::cin >> item_name;
    //initializing iterator
    int num_produced = 0;
    //looping over the produced items
    for (const Product &product : production_log) {
        if (product.item_name == item_name) {
            num_produced++;
        }
    }
    //printing output to user
    std::cout << "We have made " << num_produced << " " << item_name << "s" << std::endl;
}