#include <iostream>
#include <CLI/CLI.hpp>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/wt_huff.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <random>
#include <vector>

std::vector<size_t> getRandomNUmbers(size_t size){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distrib(0, size);
    std::vector<size_t> randomNumbers;
#pragma omp parallel for
    for (int i = 0; i < 10000; ++i) {
#pragma omp critical
        randomNumbers.push_back(distrib(gen));
    }

    return randomNumbers;
}


int main() {
    std::string wt_file = "/Users/yuhong/CLionProjects/wavelet_forest/wt_huff_8G_oct.sdsl";
    std::string forest_file = "/Users/yuhong/CLionProjects/wavelet_forest/forest_huff_8G_hex_forest.sdsl";
    size_t block_size = 10000000; // approximate 10MB


// use the wt_huff class to compress the bit vector
    sdsl::wt_huff<sdsl::bit_vector, sdsl::rank_support_v<>> wt;
    load_from_file(wt, wt_file);
    std::cout << "wt size: "<<wt.size()<<std::endl;

    std::vector<size_t> randomNumbers = getRandomNUmbers(wt.size());
    auto start_time = std::chrono::high_resolution_clock::now();
//    read_wavelet_tree(wt, randomNumbers);
    for(size_t i:randomNumbers) {
        size_t ch = wt[i];
//        std::cout << ch << " ";
    }
//    std::cout << std::endl;

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time; // This will be a std::chrono::duration
    double elapsed_time = std::chrono::duration<double>(duration).count();

    std::cout << "wavelet tree time usage: "<<elapsed_time<<std::endl;

    //start the forest
    std::vector<sdsl::wt_huff<>> forest;
    load_from_file(forest, forest_file);
    size_t total_size = 0;
    for (int i = 0; i < forest.size(); ++i) {
        total_size += forest[i].size();
    }
    std::cout << "forest size: "<<total_size<<std::endl;
    start_time = std::chrono::high_resolution_clock::now();
//    read_wavelet_forest(forest, randomNumbers, block_size);
    for(size_t idx:randomNumbers){
        size_t block_idx = idx / block_size;
        size_t offset = (idx % block_size) ;
        size_t ch = forest[block_idx][offset];
        std::cout << ch << " ";
    }
    std::cout << std::endl;

    end_time = std::chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    elapsed_time = std::chrono::duration<double>(duration).count();

    std::cout <<"wavelet forest time usage: " <<elapsed_time<<std::endl;

    return 0;
}
