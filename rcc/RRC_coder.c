#include <arpa/inet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "InitialUE-Identity.h"
#include "PhysicalConfigDedicated.h"
#include "RRCConnectionRequest-r8-IEs.h"
#include "RRCConnectionRequest.h"
#include "RRCConnectionSetup-r8-IEs.h"
#include "RRCConnectionSetup.h"
#include "RRCConnectionSetupComplete.h"

void RRCConnectionSetupComplete_encoder(uint8_t **buffer, ssize_t *len) {
  if (buffer == NULL || len == NULL) {
    fprintf(stderr, "Error: Buffer or length pointer is NULL\n");
    return;
  }

  RRCConnectionSetupComplete_t pdu;
  RRCConnectionSetupComplete_r8_IEs_t *r8_IEs = NULL;

  memset(&pdu, 0, sizeof(pdu));

  pdu.rrc_TransactionIdentifier = 1;
  pdu.criticalExtensions.present =
      RRCConnectionSetupComplete__criticalExtensions_PR_c1;
  pdu.criticalExtensions.choice.c1.present =
      RRCConnectionSetupComplete__criticalExtensions__c1_PR_rrcConnectionSetupComplete_r8;
  r8_IEs =
      &pdu.criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8;

  r8_IEs->selectedPLMN_Identity = 1;

  r8_IEs->dedicatedInfoNAS.size = 3;
  r8_IEs->dedicatedInfoNAS.buf = (uint8_t *)calloc(3, sizeof(uint8_t));
  r8_IEs->dedicatedInfoNAS.buf[0] = 0x01;
  r8_IEs->dedicatedInfoNAS.buf[1] = 0x02;
  r8_IEs->dedicatedInfoNAS.buf[2] = 0x03;

  asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
  res = asn_encode_to_new_buffer(NULL, ATS_CANONICAL_XER,
                                 &asn_DEF_RRCConnectionSetupComplete, &pdu);
  free(res.buffer);
  memset(&res, 0, sizeof(res));
  res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER,
                                 &asn_DEF_RRCConnectionSetupComplete, &pdu);
  *buffer = res.buffer;
  *len = res.result.encoded;
  printf("len encoded: %ld\n", res.result.encoded);

  if (*buffer == NULL) {
    fprintf(stderr, "Error encoding RRCConnectionSetupComplete\n");
    return;
  }
  fprintf(stderr, "Encoded RRCConnectionSetupComplete\n");
  for (ssize_t i = 0; i < res.result.encoded; i++) {
    fprintf(stderr, "%02X ", (*buffer)[i]);
  }
  fprintf(stderr, "\n");

  xer_fprint(stdout, &asn_DEF_RRCConnectionSetupComplete, &pdu);
}

void RRCConnectionRequest_encoder(uint8_t **buffer, ssize_t *len) {
  if (buffer == NULL || len == NULL) {
    fprintf(stderr, "Error: Buffer or length pointer is NULL\n");
    return;
  }

  RRCConnectionRequest_t pdu;
  RRCConnectionRequest_r8_IEs_t *r8_IEs = NULL;
  InitialUE_Identity_t *ue_Identity = NULL;

  memset(&pdu, 0, sizeof(pdu));

  pdu.criticalExtensions.present =
      RRCConnectionRequest__criticalExtensions_PR_rrcConnectionRequest_r8;
  r8_IEs = &pdu.criticalExtensions.choice.rrcConnectionRequest_r8;

  ue_Identity = &r8_IEs->ue_Identity;
  ue_Identity->present = InitialUE_Identity_PR_randomValue;
  ue_Identity->choice.randomValue.size = 5;  // 40 бит
  ue_Identity->choice.randomValue.bits_unused = 0;
  ue_Identity->choice.randomValue.buf = (uint8_t *)calloc(5, sizeof(uint8_t));

  for (int i = 0; i < 5; i++) {
    ue_Identity->choice.randomValue.buf[i] = (uint8_t)rand();
  }

  r8_IEs->establishmentCause = EstablishmentCause_mo_Signalling;

  r8_IEs->spare.size = 1;
  r8_IEs->spare.bits_unused = 7;
  r8_IEs->spare.buf = (uint8_t *)calloc(1, sizeof(uint8_t));

  r8_IEs->spare.buf[0] = 0;

  asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
  res = asn_encode_to_new_buffer(NULL, ATS_CANONICAL_XER,
                                 &asn_DEF_RRCConnectionRequest, &pdu);
  free(res.buffer);
  memset(&res, 0, sizeof(res));
  res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER,
                                 &asn_DEF_RRCConnectionRequest, &pdu);
  *buffer = res.buffer;
  *len = res.result.encoded;
  printf("len encoded: %ld\n", res.result.encoded);

  if (*buffer == NULL) {
    fprintf(stderr, "Error enconing RRCConnectionRequest\n");
    return;
  }
  fprintf(stderr, "Encoded pdu\n");
  for (ssize_t i = 0; i < res.result.encoded; i++) {
    fprintf(stderr, "%02X ", (*buffer)[i]);
  }
  fprintf(stderr, "\n");

  xer_fprint(stdout, &asn_DEF_RRCConnectionRequest, &pdu);
}

void RRCConnectionSetup_encoder(uint8_t **buffer, ssize_t *len) {
  if (buffer == NULL || len == NULL) {
    fprintf(stderr, "Error: Buffer or length pointer is NULL\n");
    return;
  }

  RRCConnectionSetup_t pdu;
  RRCConnectionSetup_r8_IEs_t *r8_IEs = NULL;
  RadioResourceConfigDedicated_t *radioResourceConfigDedicated = NULL;
  PhysicalConfigDedicated_t *physicalConfigDedicated = NULL;

  memset(&pdu, 0, sizeof(pdu));

  pdu.rrc_TransactionIdentifier = 1;  // INTEGER (0..3)
  pdu.criticalExtensions.present = RRCConnectionSetup__criticalExtensions_PR_c1;
  pdu.criticalExtensions.choice.c1.present =
      RRCConnectionSetup__criticalExtensions__c1_PR_rrcConnectionSetup_r8;
  r8_IEs = &pdu.criticalExtensions.choice.c1.choice.rrcConnectionSetup_r8;

  radioResourceConfigDedicated = (RadioResourceConfigDedicated_t *)calloc(
      1, sizeof(RadioResourceConfigDedicated_t));
  r8_IEs->radioResourceConfigDedicated = *radioResourceConfigDedicated;

  asn_encode_to_new_buffer_result_t res = {NULL, {0, NULL, NULL}};
  res = asn_encode_to_new_buffer(NULL, ATS_CANONICAL_XER,
                                 &asn_DEF_RRCConnectionSetup, &pdu);
  free(res.buffer);
  memset(&res, 0, sizeof(res));
  res = asn_encode_to_new_buffer(NULL, ATS_ALIGNED_CANONICAL_PER,
                                 &asn_DEF_RRCConnectionSetup, &pdu);
  *buffer = res.buffer;
  *len = res.result.encoded;
  printf("len encoded: %ld\n", res.result.encoded);

  if (*buffer == NULL) {
    fprintf(stderr, "Error enconing RRC pdu\n");
    return;
  }
  fprintf(stderr, "Encoded pdu\n");
  for (ssize_t i = 0; i < res.result.encoded; i++) {
    fprintf(stderr, "%02X ", (*buffer)[i]);
  }
  fprintf(stderr, "\n");

  xer_fprint(stdout, &asn_DEF_RRCConnectionSetup, &pdu);
}

void RRCConnectionSetup_decoder(uint8_t *buffer, size_t *len) {
  if (buffer == NULL) {
    fprintf(stderr, "Error: Buffer is NULL\n");
    return;
  }
  if (len == NULL) {
    fprintf(stderr, "Error: Length data is NULL\n");
    return;
  }
  if (*len == 0) {
    fprintf(stderr, "Error: Buffer size is 0\n");
    return;
  }

  asn_dec_rval_t rval;
  RRCConnectionSetup_t *pdu = NULL;

  rval = asn_decode(NULL, ATS_ALIGNED_CANONICAL_PER,
                    &asn_DEF_RRCConnectionSetup, (void **)&pdu, buffer, *len);
  if (rval.code != RC_OK) {
    fprintf(stderr, "Error decoding RRCConnectionSetup message\n");
    return;
  } else {
    fprintf(stderr, "Decoded RRCConnectionSetup message\n");
  }

  printf("rrc_TransactionIdentifier: %ld\n", pdu->rrc_TransactionIdentifier);
  xer_fprint(stdout, &asn_DEF_RRCConnectionSetup, pdu);

  ASN_STRUCT_FREE(asn_DEF_RRCConnectionSetup, pdu);
}

void RRCConnectionRequest_decoder(uint8_t *buffer, size_t *len) {
  if (buffer == NULL) {
    fprintf(stderr, "Error: Buffer is NULL\n");
    return;
  }
  if (len == NULL) {
    fprintf(stderr, "Error: Length data is NULL\n");
    return;
  }
  if (*len == 0) {
    fprintf(stderr, "Error: Buffer size is 0\n");
    return;
  }

  asn_dec_rval_t rval;
  RRCConnectionRequest_t *pdu = NULL;

  rval = asn_decode(NULL, ATS_ALIGNED_CANONICAL_PER,
                    &asn_DEF_RRCConnectionRequest, (void **)&pdu, buffer, *len);
  if (rval.code != RC_OK) {
    fprintf(stderr, "Error decoding RRCConnectionRequest message\n");
    return;
  } else {
    fprintf(stderr, "Decoded RRCConnectionRequest message\n");
  }

  if (pdu->criticalExtensions.present ==
      RRCConnectionRequest__criticalExtensions_PR_rrcConnectionRequest_r8) {
    RRCConnectionRequest_r8_IEs_t *r8_IEs =
        &pdu->criticalExtensions.choice.rrcConnectionRequest_r8;
    if (r8_IEs->ue_Identity.present == InitialUE_Identity_PR_randomValue) {
      printf("UE Random Value: ");
      for (int i = 0; i < r8_IEs->ue_Identity.choice.randomValue.size; i++) {
        printf("%02X", r8_IEs->ue_Identity.choice.randomValue.buf[i]);
      }
      printf("\n");
    }
    printf("Establishment Cause: %ld\n", r8_IEs->establishmentCause);
    printf("Spare: %02X\n", r8_IEs->spare.buf[0]);
  }

  xer_fprint(stdout, &asn_DEF_RRCConnectionRequest, pdu);
  ASN_STRUCT_FREE(asn_DEF_RRCConnectionRequest, pdu);
}

void tx_send(uint8_t **buffer, ssize_t *len, uint16_t port) {
  struct sockaddr_in servaddr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr.s_addr = inet_addr("192.168.1.200"),
  };

  int sockfd;

  sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
  if (sockfd < 0) {
    printf("Error when opening socket\n");
    exit(1);
  }

  int ret = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (ret < 0) {
    printf("Error when connecting socket\n");
    exit(1);
  }

  ret = sctp_sendmsg(sockfd, *buffer, *len, NULL, 0, 0, 0, 0, 0, 0);
  if (ret < 0) {
    printf("Error when sending msg\n");
    exit(1);
  }

  printf("Sent packet\n");

  close(sockfd);
}


void RRCConnectionSetupComplete_decoder(uint8_t *buffer, size_t *len) {
  if (buffer == NULL) {
    fprintf(stderr, "Error: Buffer is NULL\n");
    return;
  }
  if (len == NULL) {
    fprintf(stderr, "Error: Length data is NULL\n");
    return;
  }
  if (*len == 0) {
    fprintf(stderr, "Error: Buffer size is 0\n");
    return;
  }

  asn_dec_rval_t rval;
  RRCConnectionSetupComplete_t *pdu = NULL;

  rval = asn_decode(NULL, ATS_ALIGNED_CANONICAL_PER,
                    &asn_DEF_RRCConnectionSetupComplete, (void **)&pdu, buffer,
                    *len);
  if (rval.code != RC_OK) {
    fprintf(stderr, "Error decoding RRCConnectionSetupComplete message\n");
    return;
  } else {
    fprintf(stderr, "Decoded RRCConnectionSetupComplete message\n");
  }

  if (pdu->criticalExtensions.present ==
          RRCConnectionSetupComplete__criticalExtensions_PR_c1 &&
      pdu->criticalExtensions.choice.c1.present ==
          RRCConnectionSetupComplete__criticalExtensions__c1_PR_rrcConnectionSetupComplete_r8) {
    RRCConnectionSetupComplete_r8_IEs_t *r8_IEs =
        &pdu->criticalExtensions.choice.c1.choice.rrcConnectionSetupComplete_r8;

    printf("RRC Transaction Identifier: %ld\n", pdu->rrc_TransactionIdentifier);
    printf("Selected PLMN Identity: %ld\n", r8_IEs->selectedPLMN_Identity);
    printf("Dedicated Info NAS: ");
    for (int i = 0; i < r8_IEs->dedicatedInfoNAS.size; i++) {
      printf("%02X ", r8_IEs->dedicatedInfoNAS.buf[i]);
    }
    printf("\n");
  }

  xer_fprint(stdout, &asn_DEF_RRCConnectionSetupComplete, pdu);
  ASN_STRUCT_FREE(asn_DEF_RRCConnectionSetupComplete, pdu);
}

void rx_receive(uint8_t **buffer, ssize_t *len, uint16_t port) {
  if (buffer == NULL || len == NULL) {
    printf("Error: Invalid buffer or len data\n");
    return;
  }

  if (port == 0) {
    printf("Error: Invalid port number\n");
    return;
  }
  const int BUFFER_SIZE = 100;
  *len = BUFFER_SIZE;

  struct sockaddr_in servaddr = {
      .sin_family = AF_INET,
      .sin_port = htons(port),
      .sin_addr.s_addr = inet_addr("192.168.1.200"),
  };

  *buffer = (uint8_t *)calloc(BUFFER_SIZE, sizeof(uint8_t));

  if (*buffer == NULL) {
    printf("Error: Failed to allocate memory for buffer\n");
    return;
  }

  int sockfd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
  if (sockfd < 0) {
    printf("Error when opening the socket\n");
    exit(1);
  }

  int bind_err = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
  if (bind_err < 0) {
    printf("Error when binding to the socket\n");
    exit(1);
  }

  int listen_err = listen(sockfd, 2);
  if (listen_err < 0) {
    printf("Error when listening to the socket\n");
    return;
  }
  *len = BUFFER_SIZE;
  int received_len =
      sctp_recvmsg(sockfd, *buffer, (size_t)*len, NULL, NULL, 0, 0);
  *len = received_len;

  if (received_len < 0) {
    printf("Error: Failed to receive message\n");
    close(sockfd);
    return;
  }

  printf("Received packet\n");
  printf("Received message length: %d bytes\n", received_len);
  printf("Buffer content: ");
  for (int i = 0; i < received_len; i++) {
    printf("%02X ", (*buffer)[i]);
  }
  printf("\n");
  printf("Buffer size: %ld\n", *len);

  close(sockfd);
}
