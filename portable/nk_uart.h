/*
 * nk_uart.h
 *
 *  Created on: July 13, 2021
 *      Author: (nbr) nenad.b.radulovic@gmail.com
 *  13/07/2021: (nbr) Initial CPU interface definition
 */
/**
 * @file
 * @brief   Neon Kit UART
 * @{
 */

#include <stdint.h>
#include <stddef.h>

#include "generic/common/nk_error.h"

#ifndef NEON_KIT_GENERIC_NK_UART_H_
#define NEON_KIT_GENERIC_NK_UART_H_

#if defined(__cplusplus)
extern "C"
{
#endif
    
#define NK_UART__WORDLENGTH             (0x3u << 0)
#define NK_UART__WORDLENGTH_8           (0x1u << 0)
#define NK_UART__WORDLENGTH_9           (0x1u << 1)

#define NK_UART__STOPBITS               (0x7u << 2)
#define NUART_STOPBITS_1                (0x1u << 2)
#define NUART_STOPBITS_1_5              (0x1u << 3)
#define NUART_STOPBITS_2                (0x1u << 4)

#define NK_UART__PARITY                 (0x7u << 5)
#define NK_UART__PARITY_NONE            (0x1u << 5)
#define NK_UART__PARITY_EVEN            (0x1u << 6)
#define NK_UART__PARITY_ODD             (0x1u << 7)

#define NK_UART__MODE                   (0x7u << 8)
#define NK_UART__MODE_RX                (0x1u << 8)
#define NK_UART__MODE_TX                (0x1u << 9)
#define NK_UART__MODE_RX_TX             (0x1u << 10)

#define NUART_STATE_READY               (0x0)
#define NUART_STATE_BUSY_RX             (0x1u << 0)
#define NUART_STATE_BUSY_TX             (0x1u << 1)

#define NUART_WORDLENGTH                                                        \
    (NK_UART__WORDLENGTH_8 | NK_UART__WORDLENGTH_9)

#define NUART_STOPBITS                                                          \
    (NUART_STOPBITS_1 | NUART_STOPBITS_1_5 | NUART_STOPBITS_2)

#define NUART_PARITY                                                            \
    (NK_UART__PARITY_NONE | NK_UART__PARITY_EVEN | NK_UART__PARITY_ODD)

#define NUART_MODE                                                              \
    (NUART_MODE_RX | NUART_MODE_TX | NUART_MODE_RX_TX)

struct nk_uart__driver;

struct nk_uart__driver__config
{
    uint32_t                    flags;
    uint32_t                    baud_rate;
};

extern struct nk_uart__driver nk_uart__device_0;
extern struct nk_uart__driver nk_uart__device_1;
extern struct nk_uart__driver nk_uart__device_2;
extern struct nk_uart__driver nk_uart__device_3;
extern struct nk_uart__driver nk_uart__device_4;
extern struct nk_uart__driver nk_uart__device_5;
extern struct nk_uart__driver nk_uart__device_6;
extern struct nk_uart__driver nk_uart__device_7;

enum nk_error nk_uart__validate_configuration(
    struct nk_uart__driver * driver, 
    const struct nk_uart__driver__config * config);

void nk_uart__configure(
    struct nk_uart__driver * driver, 
    const struct nk_uart__driver__config * config);

void nk_uart__attach_tx_complete(
    struct nk_uart__driver * driver,
    void (* tx_complete)(void *),
    void * arg);

void nk_uart__attach_rx_complete(
    struct nk_uart__driver * driver,
    void (* rx_complete)(void *),
    void * arg);

void nk_uart__rx_start(
    struct nk_uart__driver *    driver,
    void *                      data,
    size_t                      size);

void np_uart__rx_restart(struct nk_uart__driver * driver);

void nk_uart__rx_stop(struct nk_uart__driver * driver);

void nk_uart__tx_start(
    struct nk_uart__driver *    driver,
    const void *                data,
    size_t                      size);

void nk_uart__tx_restart(struct nk_uart__driver * driver);

void nk_uart__tx_stop(struct nk_uart__driver * driver);

#if defined(__cplusplus)
}
#endif

/**
 * @}
 */
#endif /* NEON_KIT_GENERIC_NK_UART_H_ */
