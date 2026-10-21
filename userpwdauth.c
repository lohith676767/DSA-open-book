#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <signal.h>

// Shared memory structure to hold synchronization and user data
typedef struct {
    sem_t auth_sem;       // Semaphore to signal the child
    int auth_success;     // Flag: 1 for success, 0 for failure
    char username[50];    // Stores the authenticated username
} SharedData;

// Signal handler for the 7-second timeout
void timeout_handler(int sig) {
    printf("\n\n[System] 7 seconds elapsed! Authentication timeout. Terminating...\n");
    // kill(0, SIGKILL) sends the kill signal to all processes in the current process group (parent and child)
    kill(0, SIGKILL); 
}

int main() {
    // 1. Set up the 7-second timeout countdown
    signal(SIGALRM, timeout_handler);
    alarm(7); 

    // 2. Allocate shared memory for parent/child communication
    SharedData *shm = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
    // Initialize the semaphore (pshared = 1 for multi-process, initial value = 0)
    sem_init(&shm->auth_sem, 1, 0);
    shm->auth_success = 0;

    printf("[System] You have 7 seconds to authenticate.\n");

    // 3. Create the child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid > 0) {
        // --- PARENT PROCESS (Authenticator) ---
        char user[50];
        char pass[50];

        // Prompt for credentials
        printf("Username: ");
        if (fgets(user, sizeof(user), stdin) != NULL) {
            user[strcspn(user, "\n")] = 0; // Remove trailing newline
        }

        printf("Password: ");
        if (fgets(pass, sizeof(pass), stdin) != NULL) {
            pass[strcspn(pass, "\n")] = 0; 
        }

        // Hardcoded authentication check
        if (strcmp(user, "admin") == 0 && strcmp(pass, "secret123") == 0) {
            shm->auth_success = 1;
            strcpy(shm->username, user);
        } else {
            shm->auth_success = 0;
        }

        // Signal the child process that authentication is finished
        sem_post(&shm->auth_sem);

        // Wait for the child to print its message and finish
        wait(NULL);

        // Cleanup
        sem_destroy(&shm->auth_sem);
        munmap(shm, sizeof(SharedData));
        
        // Turn off the alarm if authentication finished before 7 seconds
        alarm(0); 
    } 
    else {
        // --- CHILD PROCESS (Greeter) ---
        printf("[Child] Waiting for parent to authenticate...\n");

        // Wait for the parent to post the semaphore
        sem_wait(&shm->auth_sem);

        // Check the shared flag and print the result
        if (shm->auth_success) {
            printf("\n[Child] Access Granted! Welcome to the system, %s!\n", shm->username);
        } else {
            printf("\n[Child] Access Denied! Invalid credentials.\n");
        }
        
        exit(0);
    }

    return 0;
}
