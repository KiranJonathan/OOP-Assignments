// PHYS 30762 Programming in C++
// Assignment 4
// Kiran Jonathan 19/3/21

// Class for complex numbers with demonstration of member
// functions and overloaded operators

#include<iostream>
#include<string>
#include<cmath>
#include<vector>

class complex
{
    // Make functions to overload insertion and extraction operators friends
    friend std::ostream & operator<<(std::ostream &os, const complex &z);
    friend std::istream & operator>>(std::istream &is, complex &z);
    
    private:
        // Initialize real and imaginary components to zero for default constructor
        double re{0};
        double im{0};
    
    public:
        // Constructors & destructor
        complex() = default;

        complex(double real_part, double im_part) 
        {
            re = real_part; 
            im = im_part;
        }

        complex(const complex &z) 
        {
            re = z.re; 
            im = z.im;
        }

        ~complex(){}

        // Return real component
        double real_part() const
        {
            return re;
        }

        // Return imaginary component
        double im_part() const
        {
            return im;
        }

        // Return modulus
        double modulus() const
        {
            return std::sqrt(std::pow(re, 2) + std::pow(im, 2));
        }

        // Return argument (interval [-pi,pi])
        double argument() const
        {
            return std::atan2(im, re);
        }

        // Return complex conjugate
        complex conjugate() const
        {
            return complex{re, -im};
        }

        // Overload + operator for addition 
        complex operator+(const complex &z) 
        {
            complex sum{this->re + z.re, this->im + z.im};
            return sum;
        }

        // Overload - operator for subtraction
        complex operator-(const complex &z) 
        {
            complex subtraction{this->re - z.re, this->im - z.im};
            return subtraction;
        }

        // Overload * operator for multiplication
        // Uses expansion of (x1+y1i)*(x2+y2i) with real/imaginary terms collected
        complex operator*(const complex &z) 
        {
            complex product{(this->re * z.re) - (this->im * z.im), (this->re * z.im) + (this->im * z.re)};
            return product;
        }

        // Overload / operator for division 
        // Uses method of multiplying denominator by its complex conjugate
        // to make it real and multiplying numerator by the same for consistency
        // (simply gives the modulus of the denominator squared for the denominator)
        complex operator/(const complex &z) 
        {
            double denominator{ std::pow(z.modulus(), 2) };
            complex numerator{ (*this) * z.conjugate() };
            complex division{numerator.re/denominator, numerator.im/denominator};
            return division;
        }

        // Overload = operator for assignment
        void operator=(const complex &z) 
        {
            re = z.re;
            im = z.im;
        }

        // Overload += operator for additive assignment
        void operator+=(const complex &z) 
        {
            (*this) = (*this) + z;
        }

        // Overload -= operator for subtractive assignment
        void operator-=(const complex &z) 
        {
            (*this) = (*this) - z;
        }

        // Overload *= operator for multiplicative assignment
        void operator*=(const complex &z) 
        {
            (*this) = (*this) * z;
        }

        // Overload *= operator for divisive assignment
        void operator/=(const complex &z) 
        {
            (*this) = (*this) / z;
        }

        // Overload ++ operator (prefix) for increment (real part only)
        void operator++() 
        {
            ++re;
        }

        // Overload ++ operator (suffix) for increment (real part only)
        void operator++(int) 
        {
            re++;
        }

        // Overload -- operator (prefix) for decrement (real part only)
        void operator--() 
        {
            --re;
        }

        // Overload -- operator (suffix) for decrement (real part only)
        void operator--(int) 
        {
            re--;
        }

        // Declare print_data() function to print complex number's data
        void print_data();
};

// Function to print a complex number's data
void complex::print_data() 
{
    std::cout << "Data for: " << (*this) << std::endl; // Demonstrates overloaded insertion
    std::cout << "Real part: " << re << std::endl;
    std::cout << "Imaginary part: " << im << std::endl;
    std::cout << "Modulus: " << modulus() << std::endl;
    std::cout << "Argument: " << argument() << std::endl;
    std::cout << "Complex conjugate: " << conjugate() << std::endl << std::endl;
}

// Function to overload insertion operator to print complex numbers
std::ostream & operator<<(std::ostream &os, const complex &z ) 
{ 
    if (z.im >= 0) {
        os << z.re << "+" << std::abs(z.im) << "i";
    } else {
        os << z.re << "-" << std::abs(z.im) << "i";
    }

    return os;            
}

// Function to overload extraction operator. Accepts complex numbers of the 
// form x+iy or x+yi (doesn't check for validity, assumes one of these two forms)
std::istream & operator>>(std::istream &is, complex &z ) 
{ 
    double real_comp; 
    double im_comp; 
    char i_holder; // Simply stores the i character temporarily
    int sign;
    
    // Read in real component
    is >> real_comp;
    
    // Check the sign of the complex component 
    // (real component's sign is read by default when extracting double)
    if (is.get() == '+') {
        sign = +1;
    } else {
        sign = -1;
    }

    // Read in the imaginary component and store the i character
    // (accepts the i before or after the number)
    if (is.peek() == 'i') {
        // Format x+iy
        is >> i_holder;
        is >> im_comp;
    } else {
        // Format x+yi
        is >> im_comp;
        is >> i_holder;
    } 

    // Assign values and return the input stream
    z.re = real_comp;
    z.im = im_comp * sign;
    return is;
}

int main()
{  
    // Ask the user to input a complex number (demonstrate overloaded extraction operator)
    std::cout << "Creating a complex number with the default constructor and then inserting user input: " << std::endl << std::endl;
    complex z_user;
    std::cout << "Please enter a complex number in the format x+yi or x+iy: ";
    std::cin >> z_user;
    std::cout << std::endl << "Printing the data for your complex number: " << std::endl << std::endl;

    // Demonstrating that all member functions for accessing imaginary/real part, argument, modulus are public
    // print_data() will be used from now on for conciseness
    std::cout << "Data for: " << z_user << std::endl; // Demonstrates overloaded insertion
    std::cout << "Real part: " << z_user.real_part() << std::endl;
    std::cout << "Imaginary part: " << z_user.im_part() << std::endl;
    std::cout << "Modulus: " << z_user.modulus() << std::endl;
    std::cout << "Argument: " << z_user.argument() << std::endl;
    std::cout << "Complex conjugate: " << z_user.conjugate() << std::endl << std::endl;

    // Create two complex numbers given in assignment (demonstrate parameterized constructor)
    std::cout << "Creating the two required complex numbers with the parameterized constructor..." << std::endl << std::endl;
    complex z1{3, 4};
    complex z2{1, -2};

    std::cout << "Printing their data: " << std::endl << std::endl;
    z1.print_data();
    z2.print_data();

    // Demonstrate default constructor
    std::cout << "Creating a complex number with the default constructor..." << std::endl << std::endl;
    complex z_default;

    std::cout << "Printing its data: " << std::endl << std::endl;
    z_default.print_data();

    // Demonstrate assignment operator
    std::cout << "Reassigining the default constructor complex number to the user inputed complex number (overloaded =)..." << std::endl << std::endl;
    z_default = z_user;

    std::cout << "Printing its data: " << std::endl << std::endl;
    z_default.print_data();

    // Demonstrate copy constructor
    std::cout << "Creating a complex number with the copy constructor (copying first complex number 3+4i)..." << std::endl << std::endl;
    complex z_copy{z1};

    std::cout << "Printing its data: " << std::endl << std::endl;
    z_copy.print_data();

    // Demonstrate overloaded arithmetic operations with the two given complex numbers from the assignment
    std::cout << "Demonstrating overloaded arithmetic operators for " << "(" << z1 << ")"  << " and " << "(" << z2 << ")"  << ": " << std::endl << std::endl;
    complex sum{z1 + z2};
    complex subtraction{z1 - z2};
    complex multiplication{z1 * z2};
    complex division{z1 / z2};

    std::cout  << "(" << z1 << ")"  << " + " << "(" << z2 << ")"  << " = " << sum << std::endl;
    std::cout  << "(" << z1 << ")"  << " - " << "(" << z2 << ")"  << " = " << subtraction << std::endl;
    std::cout  << "(" << z1 << ")"  << " * " << "(" << z2 << ")"  << " = " << multiplication << std::endl;
    std::cout  << "(" << z1 << ")"  << " / " << "(" << z2 << ")"  << " = " << division << std::endl << std::endl;

    // Demonstrate compound assignment operations with the user's complex number
    std::cout << "Demonstrating overloaded compound assignment operators, starting with user's complex number: " << std::endl << std::endl;
    std::cout << "z: " << z_user  << std::endl << std::endl;

    std::cout << "z+= "  << "(" << z1 << ")" << std::endl;
    z_user+= z1;
    std::cout << "z: " << z_user << std::endl << std::endl;

    std::cout << "z-= "  << "(" << z1 << ")" << std::endl;
    z_user-= z1;
    std::cout << "z: " << z_user << std::endl << std::endl;

    std::cout << "z*= "  << "(" << z1 << ")"  << std::endl;
    z_user*= z1;
    std::cout << "z: " << z_user << std::endl << std::endl;

    std::cout << "z/= "  << "(" << z1 << ")" << std::endl;
    z_user/= z1;
    std::cout << "z: " << z_user << std::endl << std::endl;

    // Demonstrate incrementation/decrementation operations
    std::cout << "Demonstrating basic incrementers/decrementers (only affect real part): "  << std::endl << std::endl;
    std::cout << "z: " << z_user << std::endl << std::endl;

    std::cout << "z++ "<< std::endl;
    z_user++;
    std::cout << "z: " << z_user << std::endl << std::endl;

    std::cout << "z-- "<< std::endl;
    z_user--;
    std::cout << "z: " << z_user << std::endl << std::endl;

    std::cout << "++z "<< std::endl;
    ++z_user;
    std::cout << "z: " << z_user << std::endl << std::endl;

    std::cout << "--z "<< std::endl;
    --z_user;
    std::cout << "z: " << z_user << std::endl << std::endl;

    return 0;
}   