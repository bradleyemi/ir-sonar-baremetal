#ifndef SENSOR_H
#define SENSOR_H

void sensor_init(void);
char sensor_read(void);
void sensor_int_handler(void);

#endif
