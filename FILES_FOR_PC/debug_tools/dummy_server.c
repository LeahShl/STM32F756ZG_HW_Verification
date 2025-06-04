/**
 * Dummy server to test hw_tester.
 * Replies success to any message.
 * Make sure you change the IP in main.c to localhost
 * if you want to test hw_tester locally.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 54321
#define BUFSIZE 263

struct OutMsg {
    uint32_t test_id;
    uint8_t peripheral;
    uint8_t n_iter;
    uint8_t p_len;
    char payload[256];
};

struct InMsg {
    uint32_t test_id;
    uint8_t test_result;
};

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buf[BUFSIZE];
    ssize_t recv_len;

    struct OutMsg out_msg;
    struct InMsg in_msg;

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind to local port
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sock);
        exit(EXIT_FAILURE);
    }

    printf("UDP dummy server listening on port %d...\n", PORT);

    while (1) {
        recv_len = recvfrom(sock, buf, BUFSIZE, 0,
                            (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len < 0) {
            perror("recvfrom failed");
            continue;
        }

        // Parse OutMsg
        size_t offset = 0;
        memcpy(&out_msg.test_id, buf + offset, sizeof(uint32_t));
        offset += sizeof(uint32_t);
        out_msg.peripheral = buf[offset++];
        out_msg.n_iter = buf[offset++];
        out_msg.p_len = buf[offset++];
        if (out_msg.p_len > 0 && out_msg.p_len <= 256) {
            memcpy(out_msg.payload, buf + offset, out_msg.p_len);
            out_msg.payload[out_msg.p_len] = '\0';
        } else {
            out_msg.payload[0] = '\0';
        }

        printf("Received test_id=%u, peripheral=%u, n_iter=%u, payload=\"%s\"\n",
               out_msg.test_id, out_msg.peripheral, out_msg.n_iter, out_msg.payload);

        // Prepare InMsg response
        in_msg.test_id = out_msg.test_id;
        in_msg.test_result = 0x01; // TEST_SUCCESS

        // Send reply
        char response_buf[sizeof(in_msg)];
        memcpy(response_buf, &in_msg.test_id, sizeof(uint32_t));
        response_buf[sizeof(uint32_t)] = in_msg.test_result;

        ssize_t sent = sendto(sock, response_buf, sizeof(response_buf), 0,
                              (struct sockaddr *)&client_addr, addr_len);
        if (sent != sizeof(response_buf)) {
            perror("sendto failed");
        } else {
            printf("Sent response for test_id=%u\n", in_msg.test_id);
        }
    }

    close(sock);
    return 0;
}
