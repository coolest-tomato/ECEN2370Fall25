
#include <stdint.h>

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

//#define LED_TOGGLE_EVENT (1 << 0)
//#define DELAY_EVENT      (1 << 1)
//#define POLL_BUTTON 	 (1 << 2)
#define APP_DELAY_FLAG_EVENT 	 (1 << 0)
#define DROP_COIN (1 << 1)
#define TURN_OVER (1 << 2)

uint32_t getScheduledEvents(void);

void addSchedulerEvent(uint32_t event);

void removeSchedulerEvent(uint32_t event);

#endif /* SCHEDULER_H_ */




