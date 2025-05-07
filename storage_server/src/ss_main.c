/*
 * STORAGE SERVER MAIN
 * Main entry point for Storage Server. Initializes logger, persistence,
 * registers with Name Server, and starts command processing loop.
 * Also listens for direct client connections to serve file contents.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include "../../common/include/protocol.h"
#include "../include/ss_network.h"
#include "../include/ss_registration.h"
#include "../include/ss_commands.h"
#include "../include/ss_persistence.h"
#include "../include/file_operations.h"
#include "../../common/include/logger.h"
#include "../../common/include/error_codes.h"

Logger *ss_logger = NULL;
static char storage_dir[256] = "";
static FileOperationContext *file_ops_ctx = NULL;
static volatile sig_atomic_t shutdown_requested = 0;
static int client_server_fd_global = -1;
static int ns_fd_global = -1;

static void sigint_handler(int sig)
{
    (void)sig; // Unused parameter
    shutdown_requested = 1;

    // Write newline for cleaner terminal output
    write(STDOUT_FILENO, "\n", 1);
}

static int directory_exists(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0) && S_ISDIR(st.st_mode);
}

static ssize_t read_line_from_socket(int fd, char *buffer, size_t max_len)
{
    size_t pos = 0;
    while (pos < max_len - 1)
    {
        char c;
        ssize_t bytes = recv(fd, &c, 1, 0);
        if (bytes < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            return -1;
        }

        if (bytes == 0)
        {
            return 0;
        }

        if (c == '\r')
        {
            continue;
        }

        if (c == '\n')
        {
            break;
        }

        buffer[pos++] = c;
    }

    buffer[pos] = '\0';
    return (ssize_t)pos;
}

