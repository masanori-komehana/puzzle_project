/*
 * Timer10ms.h
 *
 *  Created on: 2021/11/09
 *      Author: st202109
 */

#ifndef SRC_TIMER10MS_H_
#define SRC_TIMER10MS_H_


// éûä‘ä«óùóp
class Timer10ms {
public:
	Timer10ms();
	Timer10ms(int, int, int);
	virtual ~Timer10ms();
	void clr();

	int min;
	int sec;
	int ms10;

	void operator ++ (int n){
		if(ms10 == 99){
			ms10 = 0;
			if (sec == 59){
				sec = 0;
				if(min==99){
					ms10 = 9;
					sec = 59;
				}else{
					min++;
				}
			}else{
				sec++;
			}
		}else{
			ms10++;
		}
	}
};

#endif /* SRC_TIMER10MS_H_ */
