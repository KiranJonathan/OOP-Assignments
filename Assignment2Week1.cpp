// PHYS 30762 Programming in C++
// Assignment 2 Week 1 Code
// Kiran Jonathan 5/3/21

// Program to compute mean, standard deviation and standard
// error of a set of courses. Data is read from a file
// assumed to be called "courselist.dat"

#include<iostream>
#include<iomanip>
#include<fstream>
#include<cmath>
#include<string>
#include<limits>

double calculate_mean(double data_points[], int number_of_points)
{
    // Calculates and returns the mean value of an array 
    // of doubles of length number_of_points

    double sum_of_points{};

    for (int i{}; i < number_of_points; i++) {
        sum_of_points += data_points[i];
    }

    return sum_of_points/number_of_points;
}

double calculate_standard_deviation(double data_points[], int number_of_points)
{
    // Calculates and returns the standard deviation of an array 
    // of doubles of length number_of_points

    double mean_value{ calculate_mean(data_points, number_of_points) };
    double sum_of_deviations{};

    for (int i{}; i < number_of_points; i++) {
        sum_of_deviations += std::pow(data_points[i] - mean_value, 2);
    }

    double standard_deviation = std::sqrt( std::pow(number_of_points-1, -1) * sum_of_deviations );

    return standard_deviation;
}

double calculate_standard_error(double standard_deviation, int number_of_points)
{
    // Calculates and returns the standard error of a dataset with a 
    // a given standard deviation and number of points

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

int get_number_of_lines(std::string file_name)
{
    // Finds and returns the number of lines with text in them 
    // in a file with the given file name

    std::ifstream data_file{file_name};

    std::string data_holder; // Temporarily holds line data, doesn't need to be stored
    int number_of_lines{};

    while (getline(data_file, data_holder)) {
        number_of_lines++;
    }   

    data_file.close();

    return number_of_lines;
}

int main()
{
    // Try to open expected file
    std::string expected_file_name{"courselist.dat"};
    std::ifstream course_file{expected_file_name};
    std::string course_file_name;

    // If the expected file is not found/invalid, ask user for valid file and open it
    if (!course_file.good()) {
        course_file.close();
        std::cerr << "Sorry, the expected file: " << expected_file_name << " could not be found" << std::endl;
        course_file_name = get_file_name();
        course_file.open(course_file_name);
    } else {
        course_file_name = expected_file_name;
    }

    // Calculate number of courses in file
    int number_of_courses{ get_number_of_lines(course_file_name) };

    // Allocate memory for course data 
    double *course_mark_array{ new double[number_of_courses] };

    // Read course data from opened file
    std::string course_mark;
    std::string unneccessary_info; // All of the information on course code/name

    for (int i{}; i < number_of_courses; i++) {
        getline(course_file, course_mark, ' '); 
        getline(course_file, unneccessary_info, '\n');

        course_mark_array[i] = std::stod(course_mark); // Only store required data
    }

    // Close file
    course_file.close();

    // Compute mean, standard deviation, and standard error of mean
    double mean_mark{ calculate_mean(course_mark_array, number_of_courses) };
    double mark_standard_deviation{ calculate_standard_deviation(course_mark_array, number_of_courses) };
    double mark_standard_error{ calculate_standard_error(mark_standard_deviation, number_of_courses) };

    // Free memory
    delete course_mark_array;

    // Print results
    std::cout << "Number of courses read from file: " << number_of_courses << std::endl;
    std::cout << std::setprecision(3);
    std::cout << "The mean mark is: " << mean_mark << std::endl;
    std::cout << "With a standard deviation of: " << mark_standard_deviation << std::endl;
    std::cout << "And a standard error of: " << mark_standard_error << std::endl;

    return 0;
}