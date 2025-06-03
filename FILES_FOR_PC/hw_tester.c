/*
 * @file hw_tester.c
 *
 * @brief Tester program for Linux, accompanying an STM32F756ZG testing project.
 *
 * USAGE:
 *  1) At least one flag (u, s, i, a, t, or --all) must be present.
 *  2) No flag may appear more than once.
 *  3) Flags can be stacked (e.g. -usi, -at)
 *  4) If a stack includes (u, s, or i) and is immediately followed by a non-flag token,
 *     that token is taken as the single message for all of {u,s,i} in the stack.
 *  5) A stack that contains a or t must NOT be followed by a message.
 *  6) Separate flags like `-u "msg"` are allowed; same rules for message.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define UUT_ADDR "10.0.1.100"
#define PORT 54321
#define BUFSIZE 263 // Max possible size of OutMsg

#define TEST_TIM 1
#define TEST_UART 2
#define TEST_SPI 4
#define TEST_I2C 8
#define TEST_ADC 16

#define TEST_SUCCESS 0x01
#define TEST_FAILED 0xff

static struct OutMsg
{
	uint32_t test_id; 
	uint8_t peripheral;
	uint8_t n_iter;
	uint8_t p_len;
	char payload[256];
}out_msg;

static struct InMsg
{
	uint32_t test_id;
	uint8_t test_result;
}in_msg;

static const char *DEFAULT_U_MSG = "Hello UART";
static const char *DEFAULT_S_MSG = "Hello SPI";
static const char *DEFAULT_I_MSG = "Hello I2C";

static int sock;
static struct sockaddr_in server_addr;
static struct hostent *host;
static char send_data[BUFSIZE];

void print_usage(const char *progname);
void init_network();

int main(int argc, char *argv[]) {
    bool want_u = false, want_s = false, want_i = false, want_a = false, want_t = false;
    bool seen_u = false, seen_s = false, seen_i = false, seen_a = false, seen_t = false;
    bool used_all = false;
    const char *msg_u = NULL, *msg_s = NULL, *msg_i = NULL;
    bool have_msg_u = false, have_msg_s = false, have_msg_i = false;

    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    int idx = 1;
    while (idx < argc) {
        char *arg = argv[idx];

        // Print help menu
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        }

        // Handle --all flag
        if (strcmp(arg, "--all") == 0) {
            if (used_all) {
                fprintf(stderr, "Error: '--all' cannot be repeated.\n");
                return EXIT_FAILURE;
            }
            used_all = true;
            want_u = want_s = want_i = want_a = want_t = true;
            seen_u = seen_s = seen_i = seen_a = seen_t = true;
            
            // Check for message
            if ( (idx + 1) < argc && argv[idx+1][0] != '-' ) {
                const char *shared = argv[idx+1];
                msg_u = msg_s = msg_i = shared;
                have_msg_u = have_msg_s = have_msg_i = true;
                idx += 1;
            }
            idx += 1;
            continue;
        }

        // Handle all other flags
        if (arg[0] == '-' && arg[1] != '\0' && arg[1] != '-') {
            size_t len = strlen(arg);
            bool stack_has_u = false, stack_has_s = false, stack_has_i = false;
            bool stack_has_a = false, stack_has_t = false;
            for (size_t k = 1; k < len; ++k) {
                char c = arg[k];
                switch (c) {
                    case 'u':
                        if (seen_u) { fprintf(stderr, "Error: '-u' repeated.\n"); return EXIT_FAILURE; }
                        seen_u = true; want_u = true; stack_has_u = true;
                        break;
                    case 's':
                        if (seen_s) { fprintf(stderr, "Error: '-s' repeated.\n"); return EXIT_FAILURE; }
                        seen_s = true; want_s = true; stack_has_s = true;
                        break;
                    case 'i':
                        if (seen_i) { fprintf(stderr, "Error: '-i' repeated.\n"); return EXIT_FAILURE; }
                        seen_i = true; want_i = true; stack_has_i = true;
                        break;
                    case 'a':
                        if (seen_a) { fprintf(stderr, "Error: '-a' repeated.\n"); return EXIT_FAILURE; }
                        seen_a = true; want_a = true; stack_has_a = true;
                        break;
                    case 't':
                        if (seen_t) { fprintf(stderr, "Error: '-t' repeated.\n"); return EXIT_FAILURE; }
                        seen_t = true; want_t = true; stack_has_t = true;
                        break;
                    default:
                        fprintf(stderr, "Error: Unknown option '-%c'.\n", c);
                        return EXIT_FAILURE;
                }
            }

            bool stack_any_us_i = stack_has_u || stack_has_s || stack_has_i;
            bool stack_any_a_t   = stack_has_a || stack_has_t;

            // Check for message string
            bool next_is_msg = false;
            if ( (idx + 1) < argc && argv[idx+1][0] != '-' ) {
                next_is_msg = true;
            }

            if (next_is_msg) {
                if (stack_any_a_t) {
                    fprintf(stderr, "Error: Cannot supply a message to a stack containing 'a' or 't'.\n");
                    return EXIT_FAILURE;
                }

                const char *shared = argv[idx+1];
                if (stack_has_u) { msg_u = shared; have_msg_u = true; }
                if (stack_has_s) { msg_s = shared; have_msg_s = true; }
                if (stack_has_i) { msg_i = shared; have_msg_i = true; }
                idx += 1;
            }

            idx += 1;
            continue;
        }

        // If reached here, there's an illegal argument
        fprintf(stderr, "Error: Unexpected token '%s'.\n", arg);
        return EXIT_FAILURE;
    }

    if (!(want_u || want_s || want_i || want_a || want_t)) {
        fprintf(stderr, "Error: At least one of -u, -s, -i, -a, -t, or --all must be provided.\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }


    // If reached here, then all the flags are ok

    if (want_u && !have_msg_u) {
        msg_u = DEFAULT_U_MSG;
    }
    if (want_s && !have_msg_s) {
        msg_s = DEFAULT_S_MSG;
    }
    if (want_i && !have_msg_i) {
        msg_i = DEFAULT_I_MSG;
    }


    // Init network
    
    

    if (want_u) {
        printf("[UART] message = \"%s\"\n", msg_u);
        
    }
    if (want_s) {
        printf("[SPI] message = \"%s\"\n", msg_s);
        
    }
    if (want_i) {
        printf("[I2C] message = \"%s\"\n", msg_i);
        
    }
    if (want_a) {
        printf("[ADC] (no message)\n");
        
    }
    if (want_t) {
        printf("[TIM] (no message)\n");
        
    }

    return EXIT_SUCCESS;
}

void print_usage(const char *progname) {
    fprintf(stdout,
        "Usage: %s [OPTIONS]\n"
        "OPTIONS:\n"
        "  -u [\"msg\"]   Run UART test (with optional message, default if none)\n"
        "  -s [\"msg\"]   Run SPI test (with optional message, default if none)\n"
        "  -i [\"msg\"]   Run I2C test (with optional message, default if none)\n"
        "  -a             Run ADC test (no message allowed)\n"
        "  -t             Run TIM test (no message allowed)\n"
        "  --all [\"msg\"]  Run all five tests (u,s,i use msg or their defaults)\n"
        "  -h, --help    Show this help and exit\n\n"
        "Flags u, s, i may be stacked (e.g. -usi). If stacked, you may supply exactly\n"
        "one message immediately after the entire stack (applies to all of u,s,i). Example:\n"
        "    %s -si \"shared message\" -a -t\n"
        "Flags a and t may be stacked with each other (e.g. -at), but if a or t appear\n"
        "in a stack, you cannot follow that stack with a message.\n\n"
        "At least one of u, s, i, a, t (or --all) must be provided. No letter may appear twice.\n",
        progname, progname
    );
}

void init_network()
{
    host = (struct hostent *) gethostbyname((char *)UUT_ADDR);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(server_addr.sin_zero),8);
}
