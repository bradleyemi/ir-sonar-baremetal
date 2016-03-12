#ifndef SONAR_H
#define SONAR_H

void sonar_init(void);
int sonar_trigger_and_read(int trig, int echo);

#endif