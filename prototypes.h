//
// Created by cam on 6/19/2019.
//

#ifndef PROCEDURALPROJECT_PROTOTYPES_H
#define PROCEDURALPROJECT_PROTOTYPES_H
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

#endif //PROCEDURALPROJECT_PROTOTYPES_H
