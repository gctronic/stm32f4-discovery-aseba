#include "ch.h"
#include "hal.h"
#include "aseba_can_interface.h"
#include <serial-can-bridge/serial_can_bridge.h>


void can_interface_send(struct can_frame *frame);
void can_rx_buffer_flush(void);
void can_tx_buffer_flush(void);

static const CANConfig can1_config = {
    .mcr = (1 << 6)  /* Automatic bus-off management enabled. */
         | (1 << 2), /* Message are prioritized by order of arrival. */

#if defined(BOARD_ST_STM32F4_DISCOVERY)
    /* APB Clock is 42 Mhz
       42MHz / 2 / (1tq + 12tq + 8tq) = 1MHz => 1Mbit */
    .btr = (1 << 0)  /* Baudrate prescaler (10 bits) */
         | (11 << 16)/* Time segment 1 (3 bits) */
         | (7 << 20) /* Time segment 2 (3 bits) */
         | (0 << 24) /* Resync jump width (2 bits) */
#endif

#if 0
         | (1 << 30) /* Loopback mode enabled */
#endif
};

#define CAN_RX_QUEUE_SIZE   512
#define CAN_TX_QUEUE_SIZE   512

memory_pool_t can_rx_pool;
memory_pool_t can_tx_pool;
mailbox_t can_rx_queue;
mailbox_t can_tx_queue;
msg_t rx_mbox_buf[CAN_RX_QUEUE_SIZE];
msg_t tx_mbox_buf[CAN_TX_QUEUE_SIZE];

struct can_frame rx_pool_buf[CAN_RX_QUEUE_SIZE];
struct can_frame tx_pool_buf[CAN_TX_QUEUE_SIZE];

static THD_WORKING_AREA(can_tx_thread_wa, 256);
static THD_FUNCTION(can_tx_thread, arg) {
    (void)arg;
    chRegSetThreadName("CAN tx");
    while (1) {
        struct can_frame *framep;
        msg_t m = chMBFetch(&can_tx_queue, (msg_t *)&framep, TIME_INFINITE);
        if (m != MSG_OK) {
            continue;
        }
        CANTxFrame txf;
        uint32_t id = framep->id;
        txf.RTR = 0;
        if (id & CAN_FRAME_EXT_FLAG) {
            txf.EID = id & CAN_FRAME_EXT_ID_MASK;
            txf.IDE = 1;
        } else {
            txf.SID = id & CAN_FRAME_STD_ID_MASK;
            txf.IDE = 0;
        }

        if (id & CAN_FRAME_RTR_FLAG) {
            txf.RTR = 1;
        }

        txf.DLC = framep->dlc;
        txf.data32[0] = framep->data.u32[0];
        txf.data32[1] = framep->data.u32[1];

        chPoolFree(&can_tx_pool, framep);
        canTransmit(&CAND1, CAN_ANY_MAILBOX, &txf, MS2ST(100));
    }
    return 0;
}

static THD_WORKING_AREA(can_rx_thread_wa, 256);
static THD_FUNCTION(can_rx_thread, arg) {
    (void)arg;
    chRegSetThreadName("CAN rx");
    while (1) {
        uint32_t id;
        CANRxFrame rxf;
        msg_t m = canReceive(&CAND1, CAN_ANY_MAILBOX, &rxf, MS2ST(1000));
        if (m != MSG_OK) {
            continue;
        }
        if (rxf.IDE) {
            id = rxf.EID | CAN_FRAME_EXT_FLAG;
        } else {
            id = rxf.SID;
        }
        if (rxf.RTR) {
            id |= CAN_FRAME_RTR_FLAG;
        }
        struct can_frame *f = (struct can_frame *)chPoolAlloc(&can_rx_pool);
        if (f == NULL) {
            continue;
        }
        f->id = id;
        f->dlc = rxf.DLC;
        f->data.u32[0] = rxf.data32[0];
        f->data.u32[1] = rxf.data32[1];
        if (chMBPost(&can_rx_queue, (msg_t)f, TIME_IMMEDIATE) != MSG_OK) {
            // couldn't post message: drop data & free the memory
            chPoolFree(&can_rx_pool, f);
        }
    }
    return 0;
}

void can_init(void)
{
    // rx queue
    chMBObjectInit(&can_rx_queue, rx_mbox_buf, CAN_RX_QUEUE_SIZE);
    chPoolObjectInit(&can_rx_pool, sizeof(struct can_frame), NULL);
    chPoolLoadArray(&can_rx_pool, rx_pool_buf, sizeof(rx_pool_buf)/sizeof(struct can_frame));

    // tx queue
    chMBObjectInit(&can_tx_queue, tx_mbox_buf, CAN_TX_QUEUE_SIZE);
    chPoolObjectInit(&can_tx_pool, sizeof(struct can_frame), NULL);
    chPoolLoadArray(&can_tx_pool, tx_pool_buf, sizeof(tx_pool_buf)/sizeof(struct can_frame));

#if defined(BOARD_ST_STM32F4_DISCOVERY)
    // CAN1 gpio init
    iomode_t mode = PAL_STM32_MODE_ALTERNATE | PAL_STM32_OTYPE_PUSHPULL
        | PAL_STM32_OSPEED_HIGHEST | PAL_STM32_PUDR_FLOATING
        | PAL_STM32_ALTERNATE(9);
    palSetPadMode(GPIOD, GPIOD_PIN0, mode); // RX
    palSetPadMode(GPIOD, GPIOD_PIN1, mode); // TX
    canStart(&CAND1, &can1_config);
    // canSTM32SetFilters(uint32_t can2sb, uint32_t num, const CANFilter *cfp);
#endif
}

void can_rx_buffer_flush(void)
{
    void *p;
    while (chMBFetch(&can_rx_queue, (msg_t *)&p, TIME_IMMEDIATE) == MSG_OK) {
        chPoolFree(&can_rx_pool, p);
    }
}

void can_tx_buffer_flush(void)
{
    void *p;
    while (chMBFetch(&can_tx_queue, (msg_t *)&p, TIME_IMMEDIATE) == MSG_OK) {
        chPoolFree(&can_tx_pool, p);
    }
}

char hex4(uint8_t b)
{
    b &= 0x0f;
    if (b < 10) {
        return '0' + b;
    } else {
        return 'a' - 10 + b;
    }
}

uint32_t hex_read(const char *s)
{
    uint32_t x = 0;
    while (*s) {
        if (*s >= '0' && *s <= '9') {
            x = (x << 4) | (*s - '0');
        } else if (*s >= 'a' && *s <= 'f') {
            x = (x << 4) | (*s - 'a' + 0x0a);
        } else if (*s >= 'A' && *s <= 'F') {
            x = (x << 4) | (*s - 'A' + 0x0A);
        } else {
            break;
        }
        s++;
    }
    return x;
}

void can_interface_send(struct can_frame *frame)
{
    struct can_frame *tx = (struct can_frame *)chPoolAlloc(&can_tx_pool);
    if (tx == NULL) {
        return;
    }
    tx->id = frame->id;
    tx->dlc = frame->dlc;
    tx->data.u32[0] = frame->data.u32[0];
    tx->data.u32[1] = frame->data.u32[1];
    if (chMBPost(&can_tx_queue, (msg_t)tx, MS2ST(100)) != MSG_OK) {
        // couldn't post, free memory
        chPoolFree(&can_tx_pool, tx);
    }
    return;
}

void aseba_can_start(void)
{
    can_init();
    chThdCreateStatic(can_tx_thread_wa, sizeof(can_tx_thread_wa), NORMALPRIO, can_tx_thread, NULL);
    chThdCreateStatic(can_rx_thread_wa, sizeof(can_rx_thread_wa), NORMALPRIO+1, can_rx_thread, NULL);
}

// bool aseba_can_send_frame(uint8_t* frame)
// {
//     (void) frame;
// }

/*
 * For Aseba Can init
 * bool can_send_frame(can_frame):
 *   return true if there was enough space (success sensing frame)
 *   return false otherwise
 * isFrameRoomFP:
 *   need a function that returns a bool
 *   true if there is enough space to send the frame
 *   false otherwise (buffer full)
 * drop packet handling:
 *   empty functions
 * AsebaCanFrameSend / Recv see comments in header file
 */
