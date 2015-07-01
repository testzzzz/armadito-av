#ifndef _LIBUMWSU_SCAN_H_
#define _LIBUMWSU_SCAN_H_

#include <libumwsu/status.h>

#ifdef __cplusplus
extern "C" {
#endif

struct umwsu;
struct umwsu_report;
struct umwsu_scan;

struct umwsu *umwsu_open(int is_remote);

void umwsu_set_verbose(struct umwsu *u, int verbosity);

int umwsu_get_verbose(struct umwsu *u);

void umwsu_print(struct umwsu *u);

void umwsu_close(struct umwsu *u);

enum umwsu_scan_flags {
  UMWSU_SCAN_THREADED   = 1 << 0,
  UMWSU_SCAN_RECURSE    = 1 << 1,
};

enum umwsu_scan_status {
  UMWSU_SCAN_OK = 1,
  UMWSU_SCAN_CANNOT_CONNECT,
  UMWSU_SCAN_CONTINUE,
  UMWSU_SCAN_COMPLETED,
};

struct umwsu_scan *umwsu_scan_new(struct umwsu *umwsu, const char *path, enum umwsu_scan_flags flags);

typedef void (*umwsu_scan_callback_t)(struct umwsu_report *report, void *callback_data);

void umwsu_scan_add_callback(struct umwsu_scan *scan, umwsu_scan_callback_t callback, void *callback_data);

enum umwsu_scan_status umwsu_scan_start(struct umwsu_scan *scan);

int umwsu_scan_get_poll_fd(struct umwsu_scan *scan);
enum umwsu_scan_status umwsu_scan_run(struct umwsu_scan *scan);

/* enum umwsu_scan_status umwsu_scan_wait_for_completion(struct umwsu_scan *scan); */

void umwsu_scan_free(struct umwsu_scan *scan);

enum umwsu_action {
  UMWSU_ACTION_NONE         = 0,
  UMWSU_ACTION_ALERT        = 1 << 1,
  UMWSU_ACTION_QUARANTINE   = 1 << 2,
  UMWSU_ACTION_REMOVE       = 1 << 3,
};

struct umwsu_report {
  char *path;
  enum umwsu_file_status status;
  enum umwsu_action action;
  char *mod_name;
  char *mod_report;
};

const char *umwsu_file_status_str(enum umwsu_file_status status);
const char *umwsu_file_status_pretty_str(enum umwsu_file_status status);
/* const char *umwsu_action_str(enum umwsu_action action); */
const char *umwsu_action_pretty_str(enum umwsu_action action);

void umwsu_report_print(struct umwsu_report *report, FILE *out);

enum umwsu_watch_event_type {
  UMWSU_WATCH_NONE,
  UMWSU_WATCH_DIRECTORY_CREATE,
  UMWSU_WATCH_DIRECTORY_OPEN,
  UMWSU_WATCH_DIRECTORY_CLOSE_NO_WRITE,
  UMWSU_WATCH_DIRECTORY_CLOSE_WRITE,
  UMWSU_WATCH_DIRECTORY_DELETE,
  UMWSU_WATCH_FILE_CREATE,
  UMWSU_WATCH_FILE_OPEN,
  UMWSU_WATCH_FILE_CLOSE_NO_WRITE,
  UMWSU_WATCH_FILE_CLOSE_WRITE,
  UMWSU_WATCH_FILE_DELETE,
  /* etc */
};

struct umwsu_watch_event {
  enum umwsu_watch_event_type event_type;
  char *full_path;
};

void umwsu_watch(struct umwsu *u, const char *dir);

int umwsu_watch_next_event(struct umwsu *u, struct umwsu_watch_event *event);

#ifdef __cplusplus
}
#endif

#endif