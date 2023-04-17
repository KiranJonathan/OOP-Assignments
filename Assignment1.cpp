// Assignment 1
// Kiran Jonathan 18/2/21
// Program to calculate transition energy using simple Bohr formula

#include<iostream>
#include<iomanip>
#include<string>
#include<limits>
#include<cmath>
#include<vector>

const float ev_to_joules{1.6e-19}; 
const float rydberg_energy{13.6};

bool is_positive_integer(std::string test_input)
{
    // Reject any string with characters in it
    for (int i{}; i < test_input.length(); i++) {
        if (!std::isdigit(test_input[i])) 
            return false;
    }

    // Return true otherwise
    return true;
}

int get_valid_input(int min_int, int max_int, std::string error_message)
{
    std::string user_input;
    int validated_input;
    bool valid{false};
    
    // Repeatedly get input until it is valid (positive integer in required range)
    while (!valid) {
        std::cin >> user_input;

        // Check for valid input and stop loop if found
        if (is_positive_integer(user_input)) {
            if (std::stoi(user_input) > min_int && std::stoi(user_input) < max_int) {
                validated_input = std::stoi(user_input);
                valid = true;
            } 
        }

        // Give error and clear input for invalid input
        if (!valid) {
            std::cout << "Error, " << error_message << ": ";  
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    }
    return validated_input;
}

char get_valid_input(std::vector<char> valid_inputs, std::string error_message)
{
    std::string user_input;
    char validated_input;
    bool valid{false};

    // Repeatedly get input until it is valid (one of required chars, not case sensitive)
    while (!valid) {
        std::cin >> user_input;

        // Check for valid input and stop loop if found
        if (user_input.length() == 1) {
            for(int i{}; i < valid_inputs.size(); i++) {
                if (std::tolower(user_input[0]) == std::tolower(valid_inputs[i])) {  
                    validated_input = valid_inputs[i];
                    valid = true;
                }
            }
        }

        // Give error and clear input for invalid input
        if (!valid) {
            std::cout << "Error, " << error_message << ": ";  
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    }
    return validated_input;
}

double energy_level_eV(int atomic_number, int quantum_number)
{
    return -rydberg_energy* std::pow(atomic_number, 2)* std::pow(quantum_number, -2);
    // Energy given by Bohr's formula
}

int main()
{
    bool running{true}; 

    // Main program loop
    while (running) {
        // Obtain valid input for required variables
        std::cout << "Please enter the atomic number: ";
        int atomic_number{get_valid_input(0, INT_MAX, "please enter a positive non-zero integer")};
        std::cout << "Please enter the initial principal quantum number: ";
        int initial_quantum_number{get_valid_input(1, INT_MAX, "please enter an integer greater than 1")};
        std::cout << "Please enter the final principal quantum number: ";
        int final_quantum_number{get_valid_input(0, initial_quantum_number, "please enter a positive non-zero integer less than the initial quantum number")};
        std::cout << "Would you like the photon energy in Joules (J) or eV (e)?: ";
        std::vector<char> valid_units = {'j', 'e'};
        char units{get_valid_input(valid_units, "please enter 'J' for Joules or 'e' for eV")};

        // Calculate energy of photon released by transition
        double initial_energy{energy_level_eV(atomic_number, initial_quantum_number)};
        double final_energy{energy_level_eV(atomic_number, final_quantum_number)};
        double photon_energy{initial_energy - final_energy}; // Photon energy is the negative of the energy transition

        // Format and output calculated photon energy
        std::string formatted_units;
        if (units == 'j') {
            formatted_units = "J";
            photon_energy *= ev_to_joules;
        } else {
            formatted_units = "eV";
        }
        std::cout << std::setprecision(3);
        std::cout << "The released photon energy is: " << photon_energy << " " << formatted_units;

        // Check if user would like to repeat the program
        std::cout << std::endl << "Would you like to repeat the program? Yes (y) or no (n)?: ";
        std::vector<char> valid_answers = {'y','n'};
        char repeat{get_valid_input( valid_answers, "please enter 'y' for yes or 'n' for no")};

        // Stop program loop if the user is finished
        if (repeat == 'n') { 
            std::cout << "Thank you, goodbye";
            running = false; 
        }
    }
    return 0;
}

