/*
 * CLIENT MAIN
 * Main entry point for Client application. Connects to Name Server,
 * implements command loop for user interaction.
 */

 // LLM GENERATED CODE STARTS HERE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "../include/client_network.h"
#include "../include/client_commands.h"
#include "../../common/include/protocol.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <nameserver_ip>\n", argv[0]);
        return 1;
    }

    client_set_ns_ip(argv[1]);

    printf("=== Distributed File System Client ===\n");

    // Get username
    char username[32];
    while (1)
    {
        printf("Enter username: ");
        if (!fgets(username, sizeof(username), stdin))
        {
            fprintf(stderr, "Failed to read username. Please try again.\n");
            clearerr(stdin);
            continue;
        }

        // Remove newline
        username[strcspn(username, "\n")] = 0;

        // Trim leading whitespace
        char *start = username;
        while (*start && isspace((unsigned char)*start))
        {
            start++;
        }

        // Trim trailing whitespace
        char *end = start + strlen(start);
        while (end > start && isspace((unsigned char)*(end - 1)))
        {
            *(--end) = '\0';
        }

        if (start != username)
        {
            memmove(username, start, strlen(start) + 1);
        }

        if (strlen(username) == 0)
        {
            fprintf(stderr, "Username cannot be empty. Please try again.\n");
            continue;
        }

        break;
    }

    // Connect to Name Server
    