/*
 *  Gyverlamp project  "Многофункциональный RGB светильник"
 *  Description      : defines and prototypes for main sketch
 */

#define MQTT_MAX_PACKET_SIZE 1024

#define DEFAULT_BRIGHNESS   50
#define DEFAULT_SPEED       30
#define DEFAULT_SCALE       40
#define TIMER_DEMO       60000   //msec, время переключения эффектов в "Демо" режиме
#define TIMER_MQTT       60000   //msec, время обновления статусов MQTT


#define NTP_SERVER COUNTRY ".pool.ntp.org"

#define BAUD                115200  // serial speed

#define TIMER_SCROLLER      120   // String scroller period, ms

// NTP time
#include <coredecls.h>                  // settimeofday_cb()
#include <TZ.h>
#include <sntp.h>
