#include "hal_data.h"
#include <stdio.h>

int _write(int file, char *ptr, int len);
int _read(int file, char *buffer, int count);


//전송완료 flag 전역변수
volatile bool g_data_received_flag = false;
volatile bool g_data_transmit_flag = false;


//인터럽트 콜백함수 (함수 이름 수정 필요)
void user_uart_callback(uart_callback_args_t *p_args)
{
    if (UART_EVENT_RX_COMPLETE == p_args->event)
    {
        g_data_received_flag = true;
    }
    else if (UART_EVENT_TX_COMPLETE == p_args->event)
    {
        g_data_transmit_flag = true;
    }
}


//scanf를 위한 _read 함수 재정의
int _read(int file, char *buffer, int count)
{
    FSP_PARAMETER_NOT_USED(file);

    int bytesReceived = 0;


    for (int i = 0; i < count; i++)
    {
        // Start Transmission
        g_data_received_flag = false;
        fsp_err_t err = R_SCI_UART_Read(&g_uart0_ctrl, (uint8_t * const)(buffer + i), 1U);
        if (FSP_SUCCESS != err)
        {
             return -1;
        }
        /* Wait for event receive complete */
        while (!g_data_received_flag)
        {
        }

        bytesReceived++;

        //input char to print uart
        g_data_transmit_flag = false;
        R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t * const)(buffer + i), 1U);
        while (!g_data_transmit_flag);

        //Break out of the loop if ENTER is pressed
        if ((char)(buffer[i]) == '\r')
        {
            break;
        }
    }
    __NOP();
    return bytesReceived;
}


//printf를 위한 _read 함수 재정의
int _write(int file, char *ptr, int len)
{
  FSP_PARAMETER_NOT_USED(file);
  int bytesTransmitted = 0;


  for (int i = 0; i < len; i++)
  {
      // Start Transmission
      g_data_transmit_flag = false;
      fsp_err_t err = R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t * const)(ptr + i), 1U);
      if (FSP_SUCCESS != err)
      {
           return -1;
      }
      /* Wait for event receive complete */
      while (!g_data_transmit_flag)
      {
      }

      bytesTransmitted++;
  }

  return bytesTransmitted;
}
