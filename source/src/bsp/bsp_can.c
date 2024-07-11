/*
 * bsp_can.c
 *
 *  Created on: 2024. 5. 21.
 *      Author: User
 */
#include "main.h"

#define traing_kit_canA_rx  70
#define traing_kit_canA_tx  71

void init_can(void) {
    //  Allocated shared peripheral to C28x
    SysCtl_allocateSharedPeripheral(SYSCTL_PALLOCATE_CAN_A,0x0U);
    //  initialize CAN Module
    CAN_initModule(CANA_BASE);
    //  Refer to the Driver Library User Guide for information on how to set tighter timing control.
    //  Additionally, consult the device data sheet for more information about the CAN module clocking.
    CAN_setBitTiming(CANA_BASE, 15, 0, 15, 7, 3);
    //  Enable CAN test mode
    CAN_enableTestMode(CANA_BASE, CAN_TEST_EXL);

    //  Initialize the transmit message object used for sending CAN messages.
    //  Send Message
#if 1
    //  Message Object Parameters:
    //      Message Object ID Number: 1
    //      Message Identifier: 4
    //      Message Frame: CAN_MSG_FRAME_STD
    //      Message Type: CAN_MSG_OBJ_TYPE_TX
    //      Message ID Mask: 0
    //      Message Object Flags:
    //      Message Data Length: 2 Bytes
    CAN_setupMessageObject(CANA_BASE, 1, 4, CAN_MSG_FRAME_STD, CAN_MSG_OBJ_TYPE_TX, 0, 0, 2);
#endif
    //  Receive Message
#if 1
    //  Initialize the transmit message object used for sending CAN messages.
    //  Message Object Parameters:
    //      Message Object ID Number: 2
    //      Message Identifier: 4
    //      Message Frame: CAN_MSG_FRAME_STD
    //      Message Type: CAN_MSG_OBJ_TYPE_RX
    //      Message ID Mask: 0
    //      Message Object Flags:
    //      Message Data Length: 0 Bytes
    CAN_setupMessageObject(CANA_BASE, 2, 4, CAN_MSG_FRAME_STD,CAN_MSG_OBJ_TYPE_RX, 0, 0, 0);
#endif

    //  Start CAN module operations
    CAN_startModule(CANA_BASE);
}

void bsp_can_gpio(void) {
    GPIO_setPinConfig(GPIO_70_CANA_RX);
    GPIO_setPadConfig(traing_kit_canA_rx, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(traing_kit_canA_rx, GPIO_QUAL_ASYNC);
    GPIO_setPinConfig(GPIO_71_CANA_TX);
    GPIO_setPadConfig(traing_kit_canA_tx, GPIO_PIN_TYPE_STD | GPIO_PIN_TYPE_PULLUP);
    GPIO_setQualificationMode(traing_kit_canA_tx, GPIO_QUAL_ASYNC);
}

void test_send_can_msg(void) {
    UARTprintf("can test\n");
    uint16_t txMsgData[2];

    txMsgData[0] = 0;
    txMsgData[1] = 0;

    // Send CAN message data from message object 1
    CAN_sendMessage(CANA_BASE, 1, 2, txMsgData);

    // Delay before receiving the data
    DEVICE_DELAY_US(500000);

    // Send CAN message data from message object 1
    CAN_sendMessage(CANA_BASE, 2, 2, txMsgData);

    //  Return success.
    set_run_flag(0);
    return ;
}
