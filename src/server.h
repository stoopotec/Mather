#pragma once

#include <stdlib.h>


char* read_all_alloc(size_t* len, int socketfd);
void write_all(char* data, size_t data_len, int socketfd);


int serve_client(int socketfd);


int send_small_file(int socketfd, const char* filename);