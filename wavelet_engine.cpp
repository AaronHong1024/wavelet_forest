//
// Created by yu hong on 5/8/24.
//
#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/wt_huff.hpp>
#include <sdsl/wavelet_trees.hpp>

int main(){
    //load file from disk
    std::string data_file = "random_data.bin";
    const size_t blockSize = 1 * 1024 * 1024; // block size: 1MB

    //construct the wavelet tree
    sdsl::int_vector<8> data;
    sdsl::load_from_file(data, data_file);
    sdsl::wt_huff<sdsl::bit_vector, sdsl::rank_support_v<>> wt;
    sdsl::construct_im(wt, data);

    size_t pos = 10;  // simple test
    if (pos < wt.size()) {
        std::cout << "Element at position " << pos << " is " << int(wt[pos]) << "." << std::endl;
    }

    //Save the wavelet tree to disk
    std::string wt_file = "path to the wavelet tree";
    sdsl::store_to_file(wt, wt_file);

    //construct the wavelet forest
    std::vector<sdsl::wt_huff<>> forest;
    forest.resize((data.size() + blockSize - 1) / blockSize);

    for(size_t i = 0; i < data.size(); i+= blockSize){
        size_t actual_block_size = blockSize;
        if(i + blockSize > data.size()){
            actual_block_size = data.size() - i;
        }
        sdsl::int_vector<8> block(actual_block_size);
        for(size_t j = 0; j < actual_block_size; j++){
            block[j] = data[i + j];
        }
        sdsl::wt_huff<> wt_block;
        sdsl::construct_im(wt_block, block);
        forest[i/blockSize] = wt_block;
    }
    //simple test for the forest
    pos = 10;

    size_t block_idx = pos / blockSize;
    size_t block_pos = pos % blockSize;
    if (block_idx < forest.size() && block_pos < forest[block_idx].size()) {
        std::cout << "Element at position " << pos << " is " << int(forest[block_idx][block_pos]) << "." << std::endl;
    }

    //Save the wavelet forest to disk
    sdsl::store_to_file(forest, "path to the wavelet forest");

}
