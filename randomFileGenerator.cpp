#include <iostream>
#include <fstream>
#include <random>
#include <sdsl/int_vector.hpp>

int main() {
    const size_t fileSize = 8 * 1024 * 1024; // target file size: 8MB
    sdsl::int_vector<8> intVector(fileSize); // Each character took 8 bits fileSize * 8 bits

    std::random_device rd;
    std::mt19937 gen(rd()); // random seed generator
    std::uniform_int_distribution<unsigned char> dis(0, 255);  // choose random number between 0 and 255

    for (size_t i = 0; i < fileSize; ++i) {
        intVector[i] = dis(gen); // assign random number to each character
    }

    // Use SDSL's function to store the bit_vector to a file
    sdsl::store_to_file(intVector, "/Users/yuhong/CLionProjects/wavelet_forest/random_data.bin");

    std::cout << "Data has been stored using SDSL's store_to_file." << std::endl;

    return 0;
}