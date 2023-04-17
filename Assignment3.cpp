// PHYS 30762 Programming in C++
// Assignment 4
// Kiran Jonathan 12/3/21
// Simple demonstration of a C++ class (a galaxy class)
//
// Hubble types: E[0-7], S0, S[a-c], SB[a-c], Irr
// Redshift z in range [0,10]
// Total mass M_tot in range [1e7,1e12] M_sun
// Stellar mass fraction f_* in range [0,0.05]

#include<iostream>
#include<string>
#include<vector>

class galaxy
{
    private:
        // Galaxy properties
        std::string hubble_type{ "E0" };
        double redshift{ 0 };
        double total_mass{ 1e7 }; // Measured in units of solar mass
        double stellar_mass_fraction{ 0 };
        std::vector<galaxy> satellites;

    public:
        // Constructors
        galaxy() = default;
        galaxy(std::string type, double rshift, double tot_mass, double stellar_mass_frac) 
        {
            hubble_type = type;
            redshift = rshift;
            total_mass = tot_mass;
            stellar_mass_fraction = stellar_mass_frac;
        }

        // Destructor
        ~galaxy() {};

        // Return stellar mass in units of solar mass (M_* = f_* x M_tot)
        double stellar_mass() 
        { 
            return stellar_mass_fraction * total_mass; 
        }

        // Change galaxy's Hubble type
        void change_type(std::string new_type) 
        { 
            hubble_type = new_type; 
        }

        // Add satellite galaxy
        void add_satellite(galaxy new_satellite) 
        { 
            satellites.push_back(new_satellite); 
        }

        // Prototype for function to print out data for the galaxy and its satellites
        void print_data();
};

void galaxy::print_data() 
{
    // Print out a galaxy's own data
    std::cout << "Hubble Type: " << hubble_type << std::endl;
    std::cout << "Redshift: " << redshift << std::endl;
    std::cout << "Total mass: " << total_mass << " M_sun" << std::endl;
    std::cout << "Stellar Mass Fraction: " << stellar_mass_fraction << std::endl;
    std::cout << "Stellar Mass: " << stellar_mass() << " M_sun" << std::endl;
    std::cout << "Number of satellites: " << satellites.size() << std::endl << std::endl;

    // Print out the data of a galaxy's satellites (if it has any)
    if (satellites.size() > 0) {
        std::cout << "Satellite data:" << std::endl << std::endl;
        int satellite_num{1}; // Initialized as one as it makes more sense for non-vector counting
        for(auto satellite_it{ satellites.begin() }; satellite_it < satellites.end(); satellite_it++) {
            std::cout << "Data for satellite " << satellite_num << ": " << std::endl;
            satellite_it -> print_data();
            satellite_num++;
        }
    } 
}

int main()
{
    // Example using default constructor
    std::cout << "Creating a galaxy with the default constructor... " << std::endl << std::endl;
    galaxy galaxy_default; 

    // Example using parameterised constructor
    std::cout << "Creating an Irr galaxy with the parameterized constructor..." << std::endl << std::endl;
    galaxy galaxy_param("Irr", 3, 1e10, 0.03);

    // Showing stellar_mass() can be accessed from main (is public) - used in print_data() for the rest of the code
    std::cout << "Printing the stellar mass of the parameterized constructor galaxy for demonstration purposes:" << std::endl;
    std::cout << "Stellar mass: " << galaxy_param.stellar_mass() << std::endl << std::endl;

    // Adding both galaxies to a vector (copying them - original galaxies not altered/used from now on)
    std::cout << "Adding both galaxies to a vector..." << std::endl << std::endl;
    std::vector<galaxy> galaxies{galaxy_default, galaxy_param};

    // Iterate through galaxy vecotr and print their data
    std::cout << "Iterating through the vector to print all data:" << std::endl << std::endl;
    int galaxy_num{1}; // Initialized as one as it makes more sense for non-vector counting
    for (auto galaxy_it{galaxies.begin()}; galaxy_it < galaxies.end(); galaxy_it++) {
        std::cout << "Data for galaxy " << galaxy_num << ":" << std::endl << std::endl;
        galaxy_it -> print_data();
        galaxy_num++;
    }

    // Create galaxies to act as satellites and show the user their data for future reference
    std::cout << "Creating 3 satellite galaxies..." << std::endl << std::endl;

    std::cout << "Sc type satellite:" << std::endl;
    galaxy satellite_1("Sc", 7, 4e9, 0.02);
    satellite_1.print_data();

    std::cout << "Sba type satellite:" << std::endl;
    galaxy satellite_2("SBa", 1, 2e9, 0.035);
    satellite_2.print_data();

    std::cout << "SBb type satellite:" << std::endl;
    galaxy satellite_3("SBb", 4, 3e8, 0.04);
    satellite_3.print_data();

    // Examples of adding satellites to galaxies (copying them - original satellites not altered/used from now on)
    std::cout << "Adding Sc satellite to galaxy 1 (default constructor)..." << std::endl << std::endl;
    (galaxies[0]).add_satellite(satellite_1);
    std::cout << "Adding SBa satellite and SBb satellite to galaxy 2 (Irr type)..." << std::endl << std::endl;
    (galaxies[1]).add_satellite(satellite_2);
    (galaxies[1]).add_satellite(satellite_3);

    // Example of changing Hubble type from Irr to S0
    std::cout << "Changing galaxy 2 from Irr to S0 type..." << std::endl << std::endl;
    (galaxies[1]).change_type("S0");

    // Iterate through the galaxy vector with the altered galaxies and print their (+ their satellites') data
    std::cout << "Iterating through the vector to print all data:" << std::endl << std::endl;
    galaxy_num = 1; // Set back to one as I have referred to them as galaxy 1 and galaxy 2
    for (auto galaxy_it{galaxies.begin()}; galaxy_it < galaxies.end(); galaxy_it++) {
        std::cout << "Data for galaxy " << galaxy_num << ":" << std::endl << std::endl;
        galaxy_it -> print_data();
        galaxy_num++;
    }

    return 0;
}