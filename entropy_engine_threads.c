#include <stdio.h>
#include <stdlib.h> // Needed for the system() command
#include <time.h>
#include <pthread.h>
#include <unistd.h>  // Needed for alarm()
#include <signal.h>  // Needed for the emergency broadcast

volatile int runner_a_time = 0;
volatile int runner_b_time = 0;
volatile int keep_running = 1; // Master switch

// TREADMILL A
void *treadmill_a(void *arg) {
    while (keep_running) { runner_a_time++; }
    return NULL;
}

// TREADMILL B
void *treadmill_b(void *arg) {
    while (keep_running) { runner_b_time++; }
    return NULL;
}

// THE ALARM CLOCK
void time_is_up(int sig) {
    keep_running = 0; // Flips the switch to OFF after 5 seconds
}

int main() {
    pthread_t thread_a, thread_b;

    // 1. Open the vault door from INSIDE the C program
    // "w" means we are opening raw_chaos.key to Write to it
    FILE *vault = fopen("raw_chaos.key", "w");
    if (vault == NULL) {
        printf("Error: Could not open the vault!\n");
        return 1;
    }

    printf("Engine started. Harvesting hardware chaos for 5 seconds...\n");

    // 2. Set the 5-second physical timer
    signal(SIGALRM, time_is_up);
    alarm(5);

    // 3. Start the runners
    pthread_create(&thread_a, NULL, treadmill_a, NULL);
    pthread_create(&thread_b, NULL, treadmill_b, NULL);

    // 4. The Harvest (Saving directly to the file, not the screen)
    while (keep_running) {
        int score_a = runner_a_time;
        int score_b = runner_b_time;

        if (score_a > score_b) {
            fprintf(vault, "1"); // Print 1 directly into the file
        } else if (score_b > score_a) {
            fprintf(vault, "0"); // Print 0 directly into the file
        }

        runner_a_time = 0;
        runner_b_time = 0;
    }

    // 5. Cleanly shut down the factory
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    
    // 6. Lock the vault door
    fclose(vault);
    printf("Harvest complete. Raw ore saved to raw_chaos.key.\n");
    printf("Refining into cryptographic key...\n\n");
    printf("YOUR UNBREAKABLE KEY:\n");
    printf("----------------------------------------------------------------\n");

    // 7. The Master Command: Tell Linux to hash the file from inside C
    system("sha256sum raw_chaos.key");
    
    printf("----------------------------------------------------------------\n");

    return 0;
}