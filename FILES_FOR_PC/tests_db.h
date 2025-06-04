#pragma once
#include <stdint.h>

/* Uncomment next line if you want the database to be saved locally
 * (in the directory where 'tests_db.c' is) instead of the user's 
 * home folder */
// #define LOCAL_DB_PATH 1

int init_db (void);

int log_test (uint32_t test_id, const char *timestamp,
             double duration_sec, int result);

int print_all_logs (void);

int print_log_by_id (uint32_t test_id);

int get_next_id (uint32_t *dest);
