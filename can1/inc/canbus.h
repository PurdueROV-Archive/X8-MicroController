

#pragma once



extern uint8_t CAN1dataReceivedFlag;

extern CanRxMsgTypeDef RxMessage;


void InitializeCANBUS1();
void CAN1SendMessage(uint8_t length, uint8_t *data);


/* canbus.h */
