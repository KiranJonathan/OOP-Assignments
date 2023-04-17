// PHYS 30762 Programming in C++
// Assignment 5
// Kiran Jonathan 16/4/21

// Class for matrices with demonstration of member
// functions and overloaded operators, all loops and
// element calls count up from 1 in the mathematical
// convention instead of the typical C++ convention of 0

#include<iostream>
#include<iomanip>
#include<cmath>
#include<vector>

class matrix
{
    // Make functions to overload operator<< and operator>> friends
    friend std::ostream & operator<<(std::ostream &os, const matrix &m);
    friend std::istream & operator>>(std::istream &is, matrix &m);

    private:
        // MATRIX DATA //
        int rows{};
        int columns{};
        double *elements{nullptr};

    public:
        // CONSTRUCTORS AND DESTRUCTOR //
        matrix() = default;        
        matrix(const int num_rows, const int num_cols, const double *matrix_elements);
        matrix(const int num_rows, const int num_cols);
        matrix(const matrix &m);
        matrix(matrix &&m);
        ~matrix() 
        {
            delete[] elements;
        }

        // MEMBER FUNCTIONS //

        matrix cut_down (int del_row_index, int del_col_index) const;
        double determinant() const;

        // Returns the value of the element at the ith row, jth column (counting from 1)
        // Does not check if row/column are within range of matrix
        double element(int i, int j) const
        {
            return elements[ (j-1) + (i-1) * columns ];
        }

        // OVERLOADED OPERATORS //

        matrix & operator=(const matrix &m);
        matrix & operator=(matrix &&m);
        matrix operator+(const matrix &m) const;
        matrix operator-(const matrix &m) const;
        matrix operator*(const matrix &m) const;

        // Overloaded () operator to return reference to element at ith row, jth column (counting from 1)
        // Does not check if row/column are within range of matrix
        double & operator()(int i, int j)
        {
            return elements[ (j-1) + (i-1) * columns ];
        }
};

// ALL CONSTRUCTORS //

// Parameterised constructor with element data given
matrix::matrix(const int num_rows, const int num_cols, const double *matrix_elements)
{
    rows = num_rows;
    columns = num_cols;
    elements = new double[num_rows * num_cols];

    // Perform deep copy of array data
    for (int i{}; i < num_rows * num_cols; i++) {
        elements[i] = matrix_elements[i];
    }
}

// Parameterized constructor with element data not given
matrix::matrix(const int num_rows, const int num_cols)
{
    rows = num_rows;
    columns = num_cols;
    elements = new double[num_rows * num_cols];    
}

// Copy constructor (deep copy)
matrix::matrix(const matrix &m) 
{
    rows = m.rows;
    columns = m.columns;

    if (m.rows > 0 && m.columns > 0) {
        elements = new double[m.rows * m.columns];

        for (int i{}; i < m.rows * m.columns; i++) {
            elements[i] = m.elements[i];
        }
    }
}

// Move constructor
matrix::matrix(matrix &&m) 
{
    std::swap(rows, m.rows);
    std::swap(columns, m.columns);
    std::swap(elements, m.elements);
}

// MEMBER FUNCTIONS //

// Returns matrix with kth row and lth column removed (counting from 1)
// Does not check if row/column are within range of matrix
matrix matrix::cut_down (int k, int l) const
{   
    int num_rows{rows - 1};
    int num_cols{columns - 1};
    double *cut_m_elements;
    cut_m_elements = new double[num_rows * num_cols]; // CHECK IF THIS NEEDS TO BE CLEARED
    int cut_index{};

    // Loop through elements row by row and store non-deleted elements
    for (int i{1}; i <= rows; i++) {

        for (int j{1}; j <= columns; j++) {

            if (i != k && j != l) {
                cut_m_elements[cut_index] = element(i,j);
                cut_index ++;
            }
        }
    }

    return matrix(num_rows, num_cols, cut_m_elements);
}

// Function to calculate the determinant recursively (for square matrices - returns NAN otherwise)
double matrix::determinant() const
{
    if (rows != columns) {
        std::cerr << "Error, this matrix is not a square matrix so a determinant could not be calculated. ";
        return NAN;
    }

    double det{};
    int sign;

    if( rows == 2 && columns == 2 ) {
        det = element(1,1) * element(2,2) - element(1,2) * element(2,1);
    } else {

        for (int i{1}; i <= columns; i++) {
            
            // Find checkerboard sign
            if(i % 2 == 0) {
                sign = -1;
            } else {
                sign = +1;
            }

            // Recursively sum all determinant contributions from the (1,i) minors
            det += sign * element(1,i) * (this->cut_down(1,i)).determinant();
        }
    }

    return det;
}

// OVERLOADED OPERATORS //

// Overload = operator for deep copy assignment (ignores self assignment)
matrix & matrix::operator=(const matrix &m) 
{
    if (&m == this) {
        return *this;
    }

    // Clear existing data and reset to default values
    delete[] elements;
    elements = nullptr;
    rows = columns = 0;

    // Perform deep copy of data (if it exists)
    if (m.rows > 0 && m.columns > 0) {
        rows = m.rows;
        columns = m.columns;

        if (m.rows > 0 && m.columns > 0) {

            elements = new double[m.rows * m.columns];

            for (int i{}; i < m.rows * m.columns; i++) {
                elements[i] = m.elements[i];
            }
        }
    }

    return *this;  
}

// Overload = operator for move assignment (ignores self assignment)
matrix & matrix::operator=(matrix &&m) 
{
    if (&m != this) {
        std::swap(rows, m.rows);
        std::swap(columns, m.columns);
        std::swap(elements, m.elements);
    } 

    return *this;
}

// Overload + operator for addition 
// (checks dimensions and returns default empty matrix if they are not compatible)
matrix matrix::operator+(const matrix &m) const
{
    matrix sum;

    if (this->rows == m.rows && this->columns == m.columns) {       
        sum.rows = m.rows;
        sum.columns = m.columns;
        sum.elements = new double[sum.rows * sum.columns];

        for (int i{}; i < sum.rows * sum.columns; i++) {
            sum.elements[i] = this->elements[i] + m.elements[i];
        }
    } else {
        std::cerr << "Error, matrices are not of the same dimensions and can't be added" << std::endl;
    }
    
    return sum;
}

// Overload - operator for subtraction 
// (checks dimensions and returns default empty matrix if they are not compatible)
matrix matrix::operator-(const matrix &m) const
{
    matrix subtraction;

    if (this->rows == m.rows && this->columns == m.columns) {
        subtraction.rows = m.rows;
        subtraction.columns = m.columns;
        subtraction.elements = new double[subtraction.rows * subtraction.columns];

        for (int i{}; i < subtraction.rows * subtraction.columns; i++) {
            subtraction.elements[i] = this->elements[i] - m.elements[i];
        }
    } else {
        std::cerr << "Error, matrices are not of the same dimensions and can't be subtracted" << std::endl;
    }
    
    return subtraction;
}

// Overload * operator for multiplication 
// (checks dimensions and returns default empty matrix if they are not compatible)
matrix matrix::operator*(const matrix &m) const
{
    matrix product;
    int product_index;
    int m1_index;
    int m2_index;
    double product_element;

    if (this->columns == m.rows) {
        product.rows = this->rows;
        product.columns = m.columns;
        product.elements = new double[product.rows * product.columns];

        // Uses formula c(i,j) = a(i,k) * b(k,j) for c = a * b where k is summed over the whole row/column 
        for (int i{1}; i <= product.rows; i++) {

            for (int j{1}; j <= product.columns; j++) {

                product_index = (j-1) + (i-1) * product.columns;
                product_element = 0;

                for (int k{1}; k <= this->columns; k++) {
                    product_element += this->element(i,k) * m.element(k,j);
                }

                product.elements[product_index] = product_element;
            }
        }
    } else {
        std::cerr << "Error, matrices are not of the correct dimensions and can't be multiplied" << std::endl;
    }
    
    return product;
}

// OVERLOADED FRIENDED IOSTREAM OPERATORS //

// Function to overload operator<< to print matrix numbers.
// Prints numbers in left aligned commons to 3 s.f and in exponential format where necessary,
// with left square bracket at start of first row and right square bracket at end of last row.
std::ostream & operator<<( std::ostream &os, const matrix &m )
{ 
    // Handle emptry matrix output
    if (m.rows == 0 || m.columns == 0 || m.elements == nullptr) {
        os << "[ ]" << std::endl;
        return os;
    }

    std::streamsize original_precision{os.precision()}; // Store ostream's precision level
    os << std::setprecision(3); 

    int index;
    os << std::endl << "[";

    for (int i{1}; i <= m.rows; i++) {

        if (i != 1) // Ensures all rows are aligned with first row with the left square bracket
            os << " ";

        for (int j{1}; j <= m.columns; j++) {
            index = (j-1) + (i-1) * m.columns;
            
            if (j != m.columns) {
                os << std::setw(9) << std::left << m.elements[index];
                os << " ";
            } else {
                os << m.elements[index];
            }
        }

        if (i != m.rows)
            os << std::endl;
    }

    os << "]" << std::endl;

    os << std::setprecision(original_precision); // Restore ostream's precision level

    return os;
}

// Function to overload extraction operator. Accepts matrices of the form
// [e11,e12|e21,e22|e31,e32] where square brackets [] denote the start and end of 
// the matrix, vertical lines | separate rows, and commas , separate elements.
// Doesn't perform any validation, assumes inputted in correct form and with the
// correct number of elements per row.
std::istream & operator>>(std::istream &is, matrix &m ) 
{ 
    int new_rows{}; 
    int new_cols; 
    double element;
    char punct_holder;
    std::vector<double> elements;

    while (is.peek() != ']') {
        is >> punct_holder >> element;

        if (punct_holder == '|') {
            new_rows++;
        }
        
        elements.push_back(element);
    }

    new_rows++;
    new_cols = elements.size() / new_rows; // Assumes user doesn't input a different number of values per row

    double new_elements[new_rows * new_cols];
    
    for (int i{}; i < new_rows * new_cols; i++) {
        new_elements[i] = elements[i];
    }

    m = matrix{new_rows, new_cols, new_elements};

    return is;
}

// Main code to demonstrate use of matrix class
int main()
{  
    // Create 4 example matrices with the parameterized constructor //
    // and print them using overloaded insertion operator           //

    std::cout << "Demonstrating parameterized constructor and overloaded insertion operator: \n\n";

    // Creating 2 square 3x3 matrices
    std::cout << "Creating 2 square 3x3 matrices... \n\n";

    double square_elements_a[9]{1,2,3,4,5,6,7,8,9};
    matrix m_square_a{3, 3, square_elements_a};
    std::cout << "Square matrix a: " << m_square_a << std::endl;

    double square_elements_b[9]{2,0,0,0,2,0,0,0,2};
    matrix m_square_b{3,3,square_elements_b};
    std::cout << "Square matrix b: " << m_square_b << std::endl;

    // Creating a column  (example of parameterized constructor with element data not given)
    std::cout << "Creating a column vector... \n\n";
    matrix m_vector{3, 1};
    m_vector(1,1) = 1;
    m_vector(2,1) = m_vector(3,1) = 0;
    std::cout << "Column vector: " << m_vector << std::endl;
    
    // Creating a 2x4 matrix
    std::cout << "Creating a rectangular 2x4 matrix... \n\n";
    double rectangle_elements[9]{1.34,2012304,-3.75,402,5010,0.01223,7.532,80012};
    matrix m_rectangle{2, 4, rectangle_elements};
    std::cout << "2x4 matrix: " << m_rectangle << std::endl;

    // Demonstrate arithmetic overloads //

    std::cout << "Demonstrating the overloaded arithmetic operations: \n\n";

    // Demonstrate overloaded addition (successful)
    std::cout << "Square matrix a + square matrix b: " << m_square_a + m_square_b << std::endl;
    // Demonstrate overloaded addition (unsuccessful)
    std::cout << "Column vector + rectangular matrix: " << m_vector + m_rectangle << std::endl;

    // Demonstrate overloaded subtraction (successful)
    std::cout << "Square matrix a - square matrix b: " << m_square_a - m_square_b << std::endl;
    // Demonstrate overloaded subctraction (unsuccessful)
    std::cout << "Column vector - rectangular matrix: " << m_vector - m_rectangle << std::endl;

    // Demonstrate overloaded multiplication (successful)
    std::cout << "Square matrix a * square matrix b: " << m_square_a * m_square_b << std::endl;
    std::cout << "Square matrix a * column vector: " << m_square_a * m_vector << std::endl;
    std::cout << "Square matrix b * column vector: " << m_square_b * m_vector << std::endl;
    // Demonstrate overloaded multiplication (unsuccessful)
    std::cout << "Column vector * rectangular matrix: " << m_vector * m_rectangle << std::endl;

    // Demonstrate member functions
    std::cout << "Demonstrating the member functions: \n\n";

    // Demonstrate function to remove row i and column j
    std::cout << "Removing the 3rd row and 3rd column from square matrix a: " << m_square_a.cut_down(3,3) << std::endl;
    std::cout << "Removing the 2nd row and 1st column from square matrix b: " << m_square_b.cut_down(2,1) << std::endl;

    // Demonstrate function to get element at ith row and jth column
    std::cout << "The (1,3) element of square matrix a: " << m_square_a.element(1,3) << std::endl;
    std::cout << "The (2,2) element of square matrix b: " << m_square_b.element(2,2) << std::endl << std::endl;

    // Demonstrate the determinant function
    std::cout << "Determinant of square matrix a: " << m_square_a.determinant() << std::endl;
    std::cout << "Determinant of square matrix b: " << m_square_b.determinant() << std::endl;
    std::cout << "Determinant of column vector: " << m_vector.determinant() << std::endl << std::endl;

    std::cout << "Creating a larger 4x4 matrix to demonstrate recursive determinant calculation fully... \n";
    double large_elements[16]{1,6,4,12,2.5,5,0,7,1,7,8,4,9,5,1,3};
    matrix m_large{4,4,large_elements};
    std::cout << "4x4 matrix: " << m_large;
    std::cout << "Its determinant: " << m_large.determinant() << std::endl << std::endl; 

    // Demonstrate different copy constructors / assignment operator //

    std::cout << "Demonstrating the deep copy constructors and assignment operators: \n\n";
    std::cout << "Matrix to be copied is square matrix a: " << m_square_a << std::endl;

    // Demonstrate self assignment
    std::cout << "Checking self assignment: \n\n";
    std::cout << "Square matrix a before self assignment: " << m_square_a << std::endl;
    m_square_a = m_square_a;
    std::cout << "Square matrix a after self assignment: " << m_square_a << std::endl;

    // Demonstrate copy constructor
    matrix m_copy_a{m_square_a};
    std::cout << "Matrix made with copy constructor matrix m_copy_a{m_square_a}: " << m_copy_a << std::endl;

    // Demonstrate overloaded assignment operator
    matrix m_copy_c;
    m_copy_c = m_square_a;
    std::cout << "Matrix intiliazed with default constructor and copied via overloaded assignment operator: " << m_copy_c << std::endl;

    // Demonstrate deep copying by changing original matrix and reprinting copies
    std::cout << "Changing the original matrix, setting diagonal elements to zero... \n\n";
    m_square_a(1,1) = m_square_a(2,2) = m_square_a(3,3) = 0;
    std::cout << "Square matrix a now: " << m_square_a << std::endl;

    // Demonstrate copy constructor
    std::cout << "Matrix made with copy constructor matrix m_copy_a{m_square_a} now: " << m_copy_a << std::endl;

    // Demonstrate overloaded assignment operator
    std::cout << "Matrix intiliazed with default constructor and copied via overloaded assignment operator now: " << m_copy_c << std::endl;

    // Demonstrate move constructor / assignment
    std::cout << "Demonstrating the move constructors and assignment operators: \n\n";
    
    // Demonstrate move constructor
    std::cout << "First matrix to move data from is square matrix b: " << m_square_b << std::endl;
    matrix m_move_a{ std::move(m_square_b) };
    std::cout << "Matrix made with the move constructor: " << m_move_a << std::endl;
    std::cout << "Matrix the data was moved from (square matrix b) now: " << m_square_b << std::endl;

    // Demonstrate move assignment
    std::cout << "Second matrix to move data from is the rectangular matrix: " << m_rectangle << std::endl;
    matrix m_move_b;
    m_move_b = std::move(m_rectangle);
    std::cout << "Initializing a matrix with the default constructor to move the data into... \n\n";
    std::cout << "Matrix the data was moved into using the overloaded move assignment operator: " << m_move_b << std::endl;
    std::cout << "Matrix the data was moved from (rectangular matrix) now: " << m_rectangle << std::endl;

    // Demonstrate self assignment for move
    std::cout << "Checking self assignment for move assignment: \n\n";
    std::cout << "Column vector before self (move) assignment: " << m_vector << std::endl;
    m_vector = std::move(m_vector);
    std::cout << "Column vector after self (move) assignment: " << m_vector << std::endl;

    // Demonstrate overloaded extraction operator for user input //

    std::cout << "Demonstrating the overloaded extraction operator: \n\n";
    std::cout << "Creating a matrix with the default constructor to hold the user's matrix... \n\n";
    matrix m_user;

    std::cout << "Matrices should be entered in a tight format with: \n";
    std::cout << "> Square brackets [] to denote the start and end of the matrix \n";
    std::cout << "> Commas , to separate elements \n";
    std::cout << "> Vertical lines | to separate rows \n";
    std::cout << "> e.g. [1,2|4,5|7,8] would input a matrix equivalent to: ";
    double example_elements[6]{1,2,4,5,7,8};
    std::cout << matrix{3,2, example_elements} << std::endl;
    std::cout << "Please enter your matrix: ";
    std::cin >> m_user;

    std::cout << "Displaying your entered matrix: " << m_user;

    return 0;
}   