

#ifndef SRC_MYUSART_H_ /* beginning of include guard SRC_MYUSART_H_ */
#define SRC_MYUSART_H_


#ifdef __cplusplus
extern "C"{
#include "stm32f4xx_hal.h"
}
#endif

class MY_USART{
private:
    int dat;
public:
    bool is_data_recv();
};

#endif /* end of include guard: SRC_MYUSART_H_ */
