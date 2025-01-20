#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <string>

// Function to read matrix from a file
std::vector<std::vector<double>> read_matrix(const std::string& folder, int n) {
    std::ostringstream filename;
    filename << folder << "/mat_" << std::setw(6) << std::setfill('0') << n << ".in";
    std::ifstream file(filename.str());
    if (!file.is_open()) {
        throw std::runtime_error("Error opening matrix file: " + filename.str());
    }

    std::vector<std::vector<double>> matrix(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file >> matrix[i][j];
        }
    }

    file.close();
    return matrix;
}

// Function to read a vector from a file
std::vector<double> read_vector(const std::string& folder, int n, int vecnum) {
    std::ostringstream filename;
    filename << folder << "/vec_" << std::setw(6) << std::setfill('0') << n << "_"
             << std::setw(6) << std::setfill('0') << vecnum << ".in";
    std::ifstream file(filename.str());
    if (!file.is_open()) {
        throw std::runtime_error("Error opening vector file: " + filename.str());
    }

    std::vector<double> vec(n);
    for (int i = 0; i < n; ++i) {
        file >> vec[i];
    }

    file.close();
    return vec;
}

// Function to check if a vector is an eigenvector and compute the eigenvalue
bool is_eigenvector(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vec, double& eigenvalue, double tolerance = 1e-6) {
    int n = matrix.size();
    std::vector<double> result(n, 0.0);

    // Compute A * vec
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    // Check if A * vec = lambda * vec
    std::vector<double> ratios(n);
    for (int i = 0; i < n; ++i) {
        if (std::abs(vec[i]) < tolerance) {
            if (std::abs(result[i]) > tolerance) {
                return false; // Division by zero check
            }
            ratios[i] = 0;
        } else {
            ratios[i] = result[i] / vec[i];
        }
    }

    // Verify if all ratios are the same
    for (int i = 1; i < n; ++i) {
        if (std::abs(ratios[i] - ratios[0]) > tolerance) {
            return false;
        }
    }

    eigenvalue = ratios[0];
    return true;
}

// Main function to process matrix and vectors
void process_eigenvector_check(const std::string& folder, int n) {
    // Read matrix
    std::vector<std::vector<double>> matrix = read_matrix(folder, n);

    // Check vectors
    for (int vecnum = 1; vecnum <= 4; ++vecnum) { // Assuming there are 4 vectors for each size
        try {
            std::vector<double> vec = read_vector(folder, n, vecnum);
            double eigenvalue = 0.0;
            if (is_eigenvector(matrix, vec, eigenvalue)) {
                std::cout << "vec_" << std::setw(6) << std::setfill('0') << n
                          << "_" << std::setw(6) << std::setfill('0') << vecnum
                          << ".in : Yes : " << eigenvalue << std::endl;

                // Append eigenvalue to the vector file
                std::ostringstream vector_filename;
                vector_filename << folder << "/vec_" << std::setw(6) << std::setfill('0') << n
                                << "_" << std::setw(6) << std::setfill('0') << vecnum << ".in";
                std::ofstream file(vector_filename.str(), std::ios::app);
                file << "\nEigenvalue: " << eigenvalue;
                file.close();
            } else {
                std::cout << "vec_" << std::setw(6) << std::setfill('0') << n
                          << "_" << std::setw(6) << std::setfill('0') << vecnum
                          << ".in : Not an eigenvector" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

int main() {
    std::ifstream input("input2.in");
    if (!input.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1;
    }

    std::string folder = "inputfiles"; // Specify the folder containing input files
    int n;
    while (input >> n) {
        process_eigenvector_check(folder, n);
    }

    input.close();
    return 0;
}

