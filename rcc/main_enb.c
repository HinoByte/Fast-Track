// #include <stdio.h>
// #include <sys/types.h>

// #include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "RRC_coder.h"

int main() {
  uint8_t *buffer;
  ssize_t len;

  while (1) {
    rx_receive(&buffer, &len, 13246);
    usleep(1000000);
    RRCConnectionRequest_decoder(buffer, (size_t *)&len);

    RRCConnectionSetup_encoder(&buffer, &len);
    tx_send(&buffer, &len, 13246);
    usleep(5000000);

    rx_receive(&buffer, &len, 13246);
    usleep(1000000);
    RRCConnectionSetupComplete_decoder(buffer, (size_t *)&len);
    free(buffer);
  }
  return 0;
}
