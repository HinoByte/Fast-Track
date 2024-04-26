#ifndef _RELEASECODER_H
#define _RELEASECODER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void RRCConnectionRequest_encoder(uint8_t **buffer, ssize_t *len);
void RRCConnectionSetup_encoder(uint8_t **buffer, ssize_t *len);
void RRCConnectionSetupComplete_encoder(uint8_t **buffer, ssize_t *len);

void RRCConnectionRequest_decoder(uint8_t *buffer, size_t *len);
void RRCConnectionSetupComplete_decoder(uint8_t *buffer, size_t *len);
void RRCConnectionSetup_decoder(uint8_t *buffer, size_t *len);

void rx_receive(uint8_t **buffer, ssize_t *len, uint16_t port);
void tx_send(uint8_t **buffer, ssize_t *len, uint16_t port);

#endif