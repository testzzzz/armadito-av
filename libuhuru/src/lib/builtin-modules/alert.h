#ifndef _LIBUHURU_ALERT_H_
#define _LIBUHURU_ALERT_H_

#include <libuhuru/module.h>
#include <libuhuru/scan.h>

void alert_callback(struct uhuru_report *report, void *callback_data);

extern struct uhuru_module alert_module;

#endif