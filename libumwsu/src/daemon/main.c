#include <libumwsu/scan.h>
#include "poll.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

struct umwsu_daemon_options {
  int no_daemon;
  /* more options later */
  char *sock_path;
};

static struct option long_options[] = {
  {"help",         no_argument,       0, 'h'},
  {"no-daemon",    no_argument,       0, 'n'},
  {0, 0, 0, 0}
};

static void usage(void)
{
  fprintf(stderr, "usage: uhuru-daemon [options] UNIX_SOCKET_PATH\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Uhuru antivirus scanner daemon\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  --help  -h               print help and quit\n");
  fprintf(stderr, "  --no-daemon -n           do not fork and go to background\n");
  fprintf(stderr, "\n");

  exit(1);
}

static void parse_options(int argc, char *argv[], struct umwsu_daemon_options *opts)
{
  int c;

  opts->no_daemon = 0;
  opts->sock_path = NULL;

  while (1) {
    int option_index = 0;

    c = getopt_long(argc, argv, "hn", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 'h':
      usage();
      break;
    case 'n':
      opts->no_daemon = 1;
      break;
    default:
      usage();
    }
  }

  if (optind < argc)
    opts->sock_path = argv[optind];

  if (opts->sock_path == NULL)
    usage();
}

static int daemonize(void)
{
  pid_t pid, sid;
        
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  } else if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);
                
  /* Open any logs here */        
                
  sid = setsid();
  if (sid < 0) {
    /* Log the failure */
    exit(EXIT_FAILURE);
  }
        
  if ((chdir("/")) < 0) {
    /* Log the failure */
    exit(EXIT_FAILURE);
  }
        
  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
}

/* refs:
   http://stackoverflow.com/questions/17954432/creating-a-daemon-in-linux 
   http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
   http://shahmirj.com/blog/beginners-guide-to-creating-a-daemon-in-linux
*/
static int other_daemonize(char* name, char* path, char* outfile, char* errfile, char* infile )
{
  if(!path) { path="/"; }
  if(!name) { name="medaemon"; }
  if(!infile) { infile="/dev/null"; }
  if(!outfile) { outfile="/dev/null"; }
  if(!errfile) { errfile="/dev/null"; }
  //printf("%s %s %s %s\n",name,path,outfile,infile);
  pid_t child;

  //fork, detach from process group leader
  if( (child=fork())<0 ) { //failed fork
    fprintf(stderr,"error: failed fork\n");
    exit(EXIT_FAILURE);
  }
  if (child>0) { //parent
    exit(EXIT_SUCCESS);
  }
  if( setsid()<0 ) { //failed to become session leader
    fprintf(stderr,"error: failed setsid\n");
    exit(EXIT_FAILURE);
  }

  //catch/ignore signals
  signal(SIGCHLD,SIG_IGN);
  signal(SIGHUP,SIG_IGN);

  //fork second time
  if ( (child=fork())<0) { //failed fork
    fprintf(stderr,"error: failed fork\n");
    exit(EXIT_FAILURE);
  }
  if( child>0 ) { //parent
    exit(EXIT_SUCCESS);
  }

  //new file permissions
  umask(0);
  //change to path directory
  chdir(path);

  //Close all open file descriptors
  int fd;
  for( fd=sysconf(_SC_OPEN_MAX); fd>0; --fd )
    {
      close(fd);
    }

  //reopen stdin, stdout, stderr
  stdin=fopen(infile,"r");   //fd=0
  stdout=fopen(outfile,"w+");  //fd=1
  stderr=fopen(errfile,"w+");  //fd=2
  
  //open syslog
  openlog(name,LOG_PID,LOG_DAEMON);
  return(0);
}

static int foo(int sock, void *data)
{
  char buff[100];

  memset(buff, 0, 100);
  read(sock, buff, 100);
  fprintf(stderr, "foo %d %s\n", sock, buff);
}

static void daemon_loop(const char *sock_path)
{
  int listen_sock;
  struct poll_set *ps;

  listen_sock = server_socket_create(sock_path);
  ps = poll_set_new(listen_sock);

  poll_set_loop(ps);
}

int main(int argc, char **argv)
{
  struct umwsu_daemon_options opts;
  int listen_sock;

  parse_options(argc, argv, &opts);

  if (!opts.no_daemon)
    daemonize();

  daemon_loop(opts.sock_path);

  return 0;
}


#if 0
  struct umwsu *u;

  u = umwsu_open();

  umwsu_set_verbose(u, 1);

#if 0
  umwsu_print(u);
#endif

  umwsu_close(u);
#endif
