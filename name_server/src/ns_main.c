/*
 * NAME SERVER MAIN
 * Central coordinator of the distributed file system (similar to HDFS NameNode)
 * Responsibilities:
 * - Accept connections from Storage Servers and Clients on separate ports
 * - Register Storage Servers and track their file lists
 * - Register Clients and handle their requests
 * - Route file requests to appropriate Storage Server using Trie lookup
 * - Maintain file metadata and provide efficient search using LRU cache
 * Uses pthread for concurrent connection handling
 */

#include "../../common/include/protocol.h"
#include "../include/ns_network.h"
#include "../include/ns_registration.h"
#include "../include/ns_storage.h"
#include "../include/ns_routing.h"
#include "../include/ns_metadata.h"
#include "../include/ns_acl.h"
#include "../include/ns_cache.h"
#include "../include/ns_exec.h"
#include "../../common/include/logger.h"
#include "../../common/include/error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

Logger *global_logger = NULL;
TrieNode *global_file_trie = NULL;
static char ns_bound_ip[INET_ADDRSTRLEN] = "0.0.0.0";
static pthread_mutex_t active_ss_lock = PTHREAD_MUTEX_INITIALIZER;
static int current_active_ss_id = -1;

static void ensure_directory(const char *path)
{
    struct stat st;
    if (stat(path, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
            return;
        fprintf(stderr, "Path exists but is not a directory: %s\n", path);
        return;
    }

    if (mkdir(path, 0755) != 0 && errno != EEXIST)
    {
        fprintf(stderr, "Failed to create directory %s: %s\n", path, strerror(errno));
    }
}

static void parse_file_list_chunk(const char *data, char files[][128], int *file_count, int *list_complete)
{
    if (!data || !*data || *list_complete)
        return;

    char temp[4096];
    strncpy(temp, data, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';

    char *save_ptr;
    char *line = strtok_r(temp, "\n", &save_ptr);
    while (line && !*list_complete)
    {
        if (strncmp(line, "FILE ", 5) == 0)
        {
            if (*file_count < 128)
            {
                strncpy(files[*file_count], line + 5, 128);
                files[*file_count][127] = '\0';
                (*file_count)++;
            }
        }
        else if (strcmp(line, "END_FILE_LIST") == 0)
        {
            *list_complete = 1;
        }
        line = strtok_r(NULL, "\n", &save_ptr);
    }
}

typedef struct
{
    int ss_id;
} StorageServerMonitorArgs;

