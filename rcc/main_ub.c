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

  RRCConnectionRequest_encoder(&buffer, &len);
  tx_send(&buffer, &len, 13246);
  usleep(5000000);

  rx_receive(&buffer, &len, 13246);
  usleep(1000000);
  RRCConnectionSetup_decoder(buffer, (size_t *)&len);
  tx_send(&buffer, &len, 13246);
  usleep(5000000);

  rx_receive(&buffer, &len, 13246);
  usleep(1000000);
  RRCConnectionSetupComplete_encoder(&buffer, &len);
  tx_send(&buffer, &len, 13246);

  free(buffer);
  return 0;
}
