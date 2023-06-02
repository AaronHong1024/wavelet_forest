//
// Created by yu hong on 6/2/23.
//
#include <iostream>
#include <sdsl/vectors.hpp>
#include <random>
#include <vector>
#include <sdsl/wt_huff.hpp>
#include <sdsl/wavelet_trees.hpp>

int main() {
    std::string tree_file = "/blue/boucher/yu.hong/test/wt_huff_8G";
    std::string forest_file = "/blue/boucher/yu.hong/test/forest_huff_8G";
    sdsl::int_vector<8> data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned char> dis(0, 255);

    const size_t total_size = 1024ULL * 1024ULL * 1024ULL * 8ULL;
    data.resize(total_size);
    // Interpret the data in different formats
    sdsl::int_vector<1> base_2_data(data.size());
    sdsl::int_vector<2> base_4_data(data.size());
    sdsl::int_vector<3> oct_data(data.size());
    sdsl::int_vector<4> hex_data(data.size());
//
    load_from_file(data, "/blue/boucher/yu.hong/random_file_generator/data_ascii.sdsl");
    load_from_file(base_2_data, "/blue/boucher/yu.hong/random_file_generator/data_base_2.sdsl");
    load_from_file(base_4_data, "/blue/boucher/yu.hong/random_file_generator/data_base_4.sdsl");
    load_from_file(oct_data, "/blue/boucher/yu.hong/random_file_generator/data_oct.sdsl");
    load_from_file(hex_data, "/blue/boucher/yu.hong/random_file_generator/data_hex.sdsl");
    std::cout << "load successed: " << std::endl;

#pragma omp parallel for
    for (size_t i = 0; i < data.size(); ++i) {
        base_2_data[i] = data[i] % 2;
        base_4_data[i] = data[i] % 4;
        oct_data[i] = data[i] % 8;
        hex_data[i] = data[i] % 16;
    }


    // Construct Wavelet Tree for base-2 data
    sdsl::wt_huff<sdsl::bit_vector, sdsl::rank_support_v<>> wt_base_2;
    sdsl::construct_im(wt_base_2, base_2_data);

    // Construct Wavelet Tree for base-4 data
    sdsl::wt_huff<sdsl::bit_vector, sdsl::rank_support_v<>> wt_base_4;
    sdsl::construct_im(wt_base_4, base_4_data);

    // Construct Wavelet Tree for octal data
    sdsl::wt_huff<sdsl::bit_vector, sdsl::rank_support_v<>> wt_oct;
    sdsl::construct_im(wt_oct, oct_data);

    // Construct Wavelet Tree for hexadecimal data
    sdsl::wt_huff<sdsl::bit_vector, sdsl::rank_support_v<>> wt_hex;
    sdsl::construct_im(wt_hex, hex_data);

    //Construct Wavelet Tree for ascii data
    sdsl::wt_huff<sdsl::bit_vector, sdsl::rank_support_v<>> wt;
    sdsl::construct_im(wt, data);
    std::cout << "construct tree successed: " << std::endl;

    // Store the Wavelet Trees
    store_to_file(wt_base_2, tree_file+"_base_2.sdsl");
    store_to_file(wt_base_4, tree_file+"_base_4.sdsl");
    store_to_file(wt_oct, tree_file+"_oct.sdsl");
    store_to_file(wt_hex, tree_file+"_hex.sdsl");
    store_to_file(wt, tree_file+"_ascii.sdsl");

    // construct the wavelet forest
    std::cout << "constructing the wavelet forest" << std::endl;
    size_t block_size = 500000; // approximate 0.5MB

    std::vector<sdsl::wt_huff<>> forest_base_2;
    std::vector<sdsl::wt_huff<>> forest_base_4;
    std::vector<sdsl::wt_huff<>> forest_oct;
    std::vector<sdsl::wt_huff<>> forest_hex;
    std::vector<sdsl::wt_huff<>> forest_ascii;

    forest_base_2.resize((data.size() + block_size - 1) / block_size);
    forest_base_4.resize((data.size() + block_size - 1) / block_size);
    forest_oct.resize((data.size() + block_size - 1) / block_size);
    forest_hex.resize((data.size() + block_size - 1) / block_size);
    forest_ascii.resize((data.size() + block_size - 1) / block_size);

#pragma omp parallel for
    for (size_t i = 0; i < data.size(); i += block_size) {
        size_t actual_block_size = block_size;

        // Check if this is the last, incomplete block
        if (i + block_size > data.size()) {
            actual_block_size = data.size() - i;  // Adjust the block size
        }

        sdsl::int_vector<1> base_2_block(actual_block_size);
        sdsl::int_vector<8> ascii_block(actual_block_size);
        sdsl::int_vector<2> base_4_block(actual_block_size);
        sdsl::int_vector<3> oct_block(actual_block_size);
        sdsl::int_vector<4> hex_block(actual_block_size);
        for (size_t j = 0; j < actual_block_size; ++j) {
                base_2_block[j] = base_2_data[i + j];
            ascii_block[j] = data[i + j];
                base_4_block[j] = base_4_data[i + j];
                oct_block[j] = oct_data[i + j];
                hex_block[j] = hex_data[i + j];
        }
        sdsl::wt_huff<> wt_base_2;
        sdsl::wt_huff<> wt_ascii;
        sdsl::wt_huff<> wt_base_4;
        sdsl::wt_huff<> wt_oct;
        sdsl::wt_huff<> wt_hex;
        sdsl::construct_im(wt_base_2, base_2_block);
        sdsl::construct_im(wt_ascii, ascii_block);
        sdsl::construct_im(wt_base_4, base_4_block);
        sdsl::construct_im(wt_oct, oct_block);
        sdsl::construct_im(wt_hex, hex_block);

        forest_base_2[i / block_size] = wt_base_2;
#pragma omp critical
        {

            forest_ascii[i / block_size] = wt_ascii;
        };
        forest_base_4[i / block_size] = wt_base_4;
        forest_oct[i / block_size] = wt_oct;
        forest_hex[i / block_size] = wt_hex;

    }
    store_to_file(forest_base_2, forest_file+"_base_2_forest.sdsl");
    store_to_file(forest_ascii, forest_file+"_ascii_forest.sdsl");
    store_to_file(forest_base_4, forest_file+"_base_4_forest.sdsl");
    store_to_file(forest_oct, forest_file+"_oct_forest.sdsl");
    store_to_file(forest_hex, forest_file+"_hex_forest.sdsl");



    return 0;

}
