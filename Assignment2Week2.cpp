// PHYS 30762 Programming in C++
// Assignment 2 Week 2 Code
// Kiran Jonathan 5/3/21

// Program to compute mean, standard deviation and standard
// error of a set of courses. Data is read from a file
// assumed to be called "courselist.dat". The user is able
// to select which year of courses to view and whether to
// print the courses in numerical or alphabetical order.

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<string>
#include<limits>
#include<vector>
#include<numeric>
#include<algorithm>

bool is_positive_integer(std::string test_input)
{
    // Checks if a string only contains a positive integer

    for (int i{}; i < test_input.length(); i++) {
        if (!std::isdigit(test_input[i])) 
            return false;
    }

    return true;
}

int get_valid_input(int min_int, int max_int, std::string error_message)
{
    // Repeatedly asks the user for input until valid input is given (a positive integer in required range).
    // Returns the first valid input

    std::string user_input;
    int validated_input;
    bool valid{false};
    
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
    // Repeatedly asks the user for input until valid input is given
    // (one of the characters in the char vector valid_inputs - not case-sensitive).
    // Returns the first valid input

    std::string user_input;
    char validated_input;
    bool valid{false};

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

double mean(std::vector<double> data_points)
{
    // Calculates and returns the mean value of a vector of doubles

    double sum_of_points;

    for (auto point_it{data_points.begin()}; point_it< data_points.end(); point_it++) {
        sum_of_points += *point_it;
    }

    return sum_of_points / data_points.size();
}

double standard_deviation(std::vector<double> data_points)
{
    // Calculates and returns the standard deviation of a vector of doubles

    double sum_of_deviations{};

    double mean_of_points{ mean(data_points) };

    for (auto point_it{data_points.begin()}; point_it< data_points.end(); point_it++) {
        sum_of_deviations += std::pow(*point_it - mean_of_points, 2);
    }

    double standard_deviation = std::sqrt( std::pow(data_points.size() - 1, -1) * sum_of_deviations );

    return standard_deviation;
}

double standard_error(double standard_deviation, int number_of_points)
{
    // Calculates and returns the standard error of a dataset with 
    // given standard deviation and number of points

    return standard_deviation * std::pow(number_of_points, -0.5);
}

std::string get_file_name()
{
    // Continually asks the user to enter a file name until a valid file is given
    // and returns the valid file name

    std::string file_name;
    std::cout << "Enter data filename: ";
    std::cin >> file_name;

    std::ifstream data_file{file_name};

    while (!data_file.good()) {
        std::cerr << "Error: file could not be opened" << std::endl;
        std::cout << "Please enter a valid file name: ";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> file_name;
        data_file.close();
        data_file.open(file_name);
    }

    data_file.close();

    return file_name;
}

int first_digit(int number) 
{ 
    // Finds and returns the first digit of a given integer

    while (number >= 10)  
        number /= 10; 
      
    return number; 
} 

int get_index(std::vector<int> vector, int element)
{
    // Finds and returns the first index where a given integer is 
    // found in a given vector of integers. Returns -1 if the
    // element is not found

    auto element_it = std::find(vector.begin(), vector.end(), element);
 
    if (element_it != vector.end()) {
        int index = element_it - vector.begin();
        return index;
    } else {
        return -1;
    }
}

int get_index(std::vector<std::string> vector, std::string element)
{
    // Finds and returns the first index where a given string is 
    // found in a given vector of strings. Returns -1 if the
    // element is not found

    auto element_it = std::find(vector.begin(), vector.end(), element);
 
    if (element_it != vector.end()) {
        int index = element_it - vector.begin();
        return index;
    } else {
        return -1;
    }
}

int main()
{
    // Try to open the expected file
    std::string expected_file_name{"courselist.dat"};
    std::ifstream course_file{expected_file_name};
    std::string course_file_name;

    // If the expected file is not found/invalid, ask user for valid file and open it
    if (!course_file.good()){
        course_file.close();
        std::cerr << "Sorry, the expected file: " << expected_file_name << " could not be found" << std::endl;
        course_file_name = get_file_name();
        course_file.open(course_file_name);
    } 

    // Read data from file into single vector of strings
    std::vector<std::string> course_data;
    std::string single_course_data;

    while (getline(course_file, single_course_data)) {
        course_data.push_back(single_course_data);
    }

    // Close file
    course_file.close();

    // Find out which part of the data to select (all courses or a specific year)
    std::cout << "Would you like to view courses from a specific year? (y/n): ";
    std::vector<char> yes_no_inputs{'y', 'n'};
    char view_specific_year_answer{get_valid_input(yes_no_inputs, "please enter 'y' for yes or 'n' for no")};

    int year_to_view;
    bool view_specific_year;

    if (view_specific_year_answer == 'y') {
        std::cout << "Which year would you like to view? ";
        year_to_view = get_valid_input(0,5, "please enter a positive non-zero integer less than 5");
        view_specific_year = true;
    } else {
        view_specific_year = false;
    }        
    
    // Find out sorting order for courses
    std::cout << "Would you like to view the courses in alphabetical (a) or numerical (n) order?: ";
    std::vector<char> alphabetical_numerical_inputs{'a', 'n'};
    char alphabetical_or_numerical{ get_valid_input(alphabetical_numerical_inputs, "please enter 'a' for alphabetical order or 'n' for numerical order") };

    // Select the wanted data (all courses or a specific year) and split it into the 3 different required variables
    std::vector<double> selected_course_marks;
    std::vector<int> selected_course_codes;
    std::vector<std::string> selected_course_names;
    std::stringstream data_stream;
    double single_course_mark;
    int single_course_code;
    std::string single_course_name;

    for(auto course_data_it{course_data.begin()}; course_data_it < course_data.end(); course_data_it++){
        data_stream << *course_data_it;

        // Split all of the data into the 3 required variables
        data_stream >> single_course_mark;
        data_stream >> single_course_code;
        getline(data_stream, single_course_name, '\n');

        // Store only the selected data in 3 separate vectors
        if (first_digit(single_course_code) == year_to_view || !view_specific_year) {
            selected_course_marks.push_back(single_course_mark);
            selected_course_codes.push_back(single_course_code);
            selected_course_names.push_back(single_course_name);
        }

        // Clear the string stream for it to be reused
        data_stream.ignore(std::numeric_limits<std::streamsize>::max());
        data_stream.clear();
    }

    // Perform coupled sort on the selected course names and codes 
    // alphabetically or numerically based on user choice
    std::vector<int> unsorted_course_codes{selected_course_codes}; // Create a copy to check index mapping by primary sort
    std::vector<std::string> unsorted_course_names{selected_course_names}; // Create a copy to check index mapping by primary sort
    std::vector<int> sorted_indexes;
    std::vector<int> sorted_course_codes;
    std::vector<std::string> sorted_course_names;

    if (alphabetical_or_numerical == 'n') {
        // Perform primary sort on course codes (increasing numerical order)
        std::sort(selected_course_codes.begin(), selected_course_codes.end());
        sorted_course_codes = selected_course_codes;

        // Find new indices of course codes in the sorted vector (construct index map)
        for(auto sorted_code_it{selected_course_codes.begin()}; sorted_code_it < selected_course_codes.end(); sorted_code_it++){
            sorted_indexes.push_back(get_index(unsorted_course_codes, *sorted_code_it));
        }

        // Transform the course name vector to the same new indices (sort it in the same order as the course codes)
        for(auto sorted_index_it{sorted_indexes.begin()}; sorted_index_it < sorted_indexes.end(); sorted_index_it++){
            sorted_course_names.push_back(unsorted_course_names[*sorted_index_it]);
        }
    } else if (alphabetical_or_numerical == 'a') {
        // Perform primary sort on course names (alphabetical order)
        std::sort(selected_course_names.begin(), selected_course_names.end());
        sorted_course_names = selected_course_names;

        // Find new indices of course names in the sorted vector (construct index map)
        for(auto sorted_name_it{selected_course_names.begin()}; sorted_name_it < selected_course_names.end(); sorted_name_it++){
            sorted_indexes.push_back(get_index(unsorted_course_names, *sorted_name_it));
        }

        // Transform the course code vector to the same new indices (sort it in the same order as the course names)
        for(auto sorted_index_it{sorted_indexes.begin()}; sorted_index_it < sorted_indexes.end(); sorted_index_it++){
            sorted_course_codes.push_back(unsorted_course_codes[*sorted_index_it]);
        }
    }

    // Use a stringstream to recombine the integer course code and the string course name
    // into a single vector of strings to be printed
    std::vector<std::string> sorted_course_data;
    std::string single_sorted_course_data;
    std::stringstream sorted_data_stream;

    for (int i{}; i< sorted_indexes.size(); i++) {
        sorted_data_stream << "PHYS" << sorted_course_codes[i] << sorted_course_names[i];
        getline(sorted_data_stream, single_sorted_course_data, '\n');
        sorted_course_data.push_back(single_sorted_course_data);
        sorted_data_stream.ignore(std::numeric_limits<std::streamsize>::max());
        sorted_data_stream.clear();
    }

    // Print selected courses in their sorted order
    std::cout << "The selected courses are:" << std::endl;
    for (auto course_info_it{sorted_course_data.begin()}; course_info_it < sorted_course_data.end(); course_info_it++) {
        std::cout << *course_info_it << std::endl;
    }

    // Compute mean, standard deviation, and standard error of mean
    double mean_mark{mean(selected_course_marks)};
    double mark_standard_deviation{standard_deviation(selected_course_marks)};
    double mark_standard_error{standard_error(mark_standard_deviation, selected_course_marks.size())};

    // Print calculated statistics
    std::cout << "Number of courses read form file: " << course_data.size() << std::endl;
    std::cout << "Number of courses selected: " << selected_course_marks.size() << std::endl;
    std::cout << std::setprecision(3);
    std::cout << "The mean mark of the selected courses is: " << mean_mark << std::endl;
    std::cout << "With a standard deviation of: " << mark_standard_deviation << std::endl;
    std::cout << "And a standard error of: " << mark_standard_error << std::endl;

    return 0;
}