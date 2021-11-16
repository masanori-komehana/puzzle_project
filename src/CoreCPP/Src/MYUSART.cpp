

#include "MYUSART.h"


bool MY_USART::is_data_recv(){
    return (USART2->SR & USART_SR_RXNE_Msk) != 0;
}
