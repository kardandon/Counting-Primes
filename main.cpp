#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

typedef std::chrono::high_resolution_clock Clock;

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

void primeNormal(int n, std::vector<int>& counts) {
    auto start = Clock::now();
    int count = 0;
    for (int i = 2; i <= n; ++i) {
        if (isPrime(i)) {
            ++count;
        }
    }
    auto end = Clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Normal took: " << elapsed_seconds.count() << " seconds" << std::endl;
    counts.push_back(count);
}

void primeSieve(int n, std::vector<int>& counts) {
    auto start = Clock::now();
    std::vector<bool> primes(n + 1, true);
    for (int p = 2; p * p <= n; ++p) {
        if (primes[p]) {
            for (int i = p * p; i <= n; i += p) {
                primes[i] = false;
            }
        }
    }
    int count = 0;
    for (int p = 2; p <= n; ++p) {
        if (primes[p]) {
            ++count;
        }
    }
    auto end = Clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "Sieve took: " << elapsed_seconds.count() << " seconds" << std::endl;
    counts.push_back(count);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number>" << std::endl;
        return 1;
    }

    int n;
    std::istringstream iss(argv[1]);
    if (!(iss >> n)) {
        std::cerr << "Error: Please provide a valid number." << std::endl;
        return 1;
    }

    std::vector<int> counts;
    std::thread t1(primeNormal, n, std::ref(counts));
    std::thread t2(primeSieve, n, std::ref(counts));

    t1.join();
    t2.join();

    std::cout << "Normal: " << counts[0] << std::endl;
    std::cout << "Sieve: " << counts[1] << std::endl;

    return 0;
}