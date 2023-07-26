#ifndef _MQTT_SOLO_H__
#define _MQTT_SOLO_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mqtt_api.h"
#include "dm_wrapper.h"
#include "cJSON.h"


void mqtt_main(void *pvParam);
void gate_publish();
static void decode_task();
#endif
