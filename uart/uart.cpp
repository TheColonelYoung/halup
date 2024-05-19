#include "uart.hpp"

UART::UART(UART_HandleTypeDef *UART_Handler_set){
    UART_Handler = UART_Handler_set;

    //HAL_UART_Receive_IT(UART_Handler_set, UART_buffer_temp, 1);
}

int UART::Send(string message){
    if (message.length() == 0) {
        return 0;
    }
    // Save message to buffer if UART is now busy
    if (busy) {
        TX_buffer.emplace_back(message);
        return TX_buffer.size();
    }
    // Send message and set UART as busy
    busy = true;
    TX_buffer.emplace_back(message);
    HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) TX_buffer.front().c_str(), TX_buffer.front().length());
    return message.length();
}

int UART::Send_pool(string message){
    return HAL_UART_Transmit(UART_Handler, (unsigned char *) message.c_str(), message.length(), HAL_MAX_DELAY);
}

int UART::Receive(){
    RX_buffer.push_back(UART_buffer_temp[0]);
    HAL_UART_Receive_IT(UART_Handler, UART_buffer_temp, 1);
    return 0;
}

int UART::Resend(){
    TX_buffer.erase(TX_buffer.begin()); // Erase message which transfer is complete
    if (TX_buffer.size() > 0) {         // Send next message in line
        HAL_UART_Transmit_IT(UART_Handler, (unsigned char *) TX_buffer.front().c_str(), TX_buffer.front().length());
    } else  { // Now is UART unoccupied
        busy = false;
    }
    return TX_buffer.size();
}
