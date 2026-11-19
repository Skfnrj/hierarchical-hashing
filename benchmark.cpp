#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "sha256.h"
#include "blake2s256.h"

void benchmark(const std::string& name, HashFunction& algorithm,
                const std::string& input, int repetitions){
    std::vector<double> times;

    //Use one byte of each digest so the result is used
    volatile char digest_byte = algorithm.hash(input).front();//Warm-up

    for(int batch = 0; batch < 5; batch++){
        auto start = std::chrono::steady_clock::now();

        for(int i=0; i < repetitions; i++){
            digest_byte = algorithm.hash(input).front();
        }

        auto stop = std::chrono::steady_clock::now();
        double elapsed_ms = std::chrono::duration<double, std::milli>(stop-start).count();

        times.push_back(elapsed_ms / repetitions);
    }

        (void)digest_byte;

        std::cout << name << ',' << input.size() << ','<< repetitions;
        for(double time : times){
            std::cout << ',' << time;
        }

        std::sort(times.begin(), times.end());
        std::cout << ',' << times[2] << '\n';
    }

int main(){
    SHA256 sha;
    BLAKE2s256 blake;

    std::string small(1024, 'a');
    std::string medium(64 * 1024, 'a');
    std::string large(1024 * 1024, 'a');

    std::cout <<std::fixed << std::setprecision(6);
    std::cout << "algorithm,input_bytes,calls_per_batch,"
                    "batch1,batch2,batch3,batch4,batch5,median_ms_per_hash\n";

    benchmark("SHA-256", sha, small, 10000);
    benchmark("BLAKE2s-256", blake, small, 10000);

    benchmark("SHA-256", sha, medium, 100);
    benchmark("BLAKE2s-256", blake, medium, 100);

    benchmark("SHA-256", sha, large, 20);
    benchmark("BLAKE2s-256", blake, large, 20);

    return 0;
    }
