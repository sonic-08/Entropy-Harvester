#include <iostream>
#include <fstream>      // Needed for C++ File I/O (saving to the vault)
#include <x86intrin.h>
#include <unistd.h>
#include <csignal>      // Needed for the alarm clock
#include <cstdlib>      // Needed for system()

// The Master Switch
volatile int keep_running = 1;

// The Alarm Clock
void time_is_up(int sig) {
    keep_running = 0;
}

// The Hardware Stopwatch
inline unsigned long long get_time() {
    unsigned int ui;
    return __rdtscp(&ui);
}

int main() {
    // 1. Open the vault using C++ file streams
    std::ofstream vault("raw_chaos.key");
    if (!vault.is_open()) {
        std::cerr << "Error: Could not open the vault!\n";
        return 1;
    }

    std::cout << "Kernel-Chaos Harvester started. Harvesting OS jitter for 5 seconds...\n";

    // 2. Set the 5-second timer
    signal(SIGALRM, time_is_up);
    alarm(5);

    // 3. The Harvest
    while (keep_running) {
        // RACE 1
        unsigned long long t1 = get_time();
        usleep(0); // Force the Linux Kernel context switch
        unsigned long long t2 = get_time();
        unsigned long long race1 = t2 - t1;

        // RACE 2
        unsigned long long t3 = get_time();
        usleep(0); // Force the Linux Kernel context switch
        unsigned long long t4 = get_time();
        unsigned long long race2 = t4 - t3;

        // The Von Neumann Unbiaser (Saving to file instead of screen)
        if (race1 < race2) {
            vault << "0";
        } else if (race1 > race2) {
            vault << "1";
        }
    }

    // 4. Cleanly shut down and lock the vault
    vault.close();

    std::cout << "Harvest complete. Raw OS chaos saved to raw_chaos.key.\n";
    std::cout << "Refining into cryptographic key...\n\n";
    std::cout << "YOUR OS-DERIVED KEY:\n";
    std::cout << "----------------------------------------------------------------\n";

    // 5. The Master Command: Tell Linux to hash the file
    system("sha256sum raw_chaos.key");
    
    std::cout << "----------------------------------------------------------------\n";

    return 0;
}