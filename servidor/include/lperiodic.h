
#ifndef LPERIODIC_H
#define LPERIODIC_H

#include <signal.h>
#include <time.h>

struct periodic_info
{
	int sig;
	sigset_t alarm_sig;
};

static int make_periodic(int unsigned period, struct periodic_info *info);

static void wait_period(struct periodic_info *info);

#endif
