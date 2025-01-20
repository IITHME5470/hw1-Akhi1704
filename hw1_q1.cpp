#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

void write_to_file(int n, const std::vector<std::vector<double>>& A, int format_flag) {
    std::ostringstream filename;
    filename << "array " << std::setw(6) << std::setfill('0') << n;
    if (format_flag == 0) {
        filename << " asc.out";
    } else {
        filename << " bin.out";
    }
    std::string file_name = filename.str();

    if (format_flag == 0) {
        // Write in ASCII format
        std::ofstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << file_name << std::endl;
            return;
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                file << std::scientific << std::setprecision(15) << A[i][j] << " ";
            }
            file << "\n";
        }
        file.close();
    } else if (format_flag == 1) {
        // Write in binary format
        std::ofstream file(file_name, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Error opening file for writing: " << file_name << std::endl;
            return;
        }
        for (int i = 0; i < n; ++i) {
            file.write(reinterpret_cast<const char*>(A[i].data()), n * sizeof(double));
        }
        file.close();
    } else {
        std::cerr << "Invalid format flag. Use 0 for ASCII and 1 for binary." << std::endl;
    }
}

int main() {
    int n;
    int format_flag;

    // Read n and format_flag from input1.in
    std::ifstream input("input1.in");
    if (!input.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1;
    }
    input >> n >> format_flag;
    input.close();

    // Dynamically allocate n x n array
    std::vector<std::vector<double>> A(n, std::vector<double>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A[i][j] = i + j;
        }
    }

    // Write the array to the file
    write_to_file(n, A, format_flag);

    std::cout << "File generated successfully!" << std::endl;
    return 0;
}

