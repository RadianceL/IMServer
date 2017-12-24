#ifndef WEBTEST_WEBCONNECTION_H
#define WEBTEST_WEBCONNECTION_H

#include "../SETCONFIG/CONNECTIONSET.h"

int initServer();

void thread(void* args);

void connection();

#endif
