#include "daemon.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdarg.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <syslog.h>


using namespace xmppsc;

static bool hup_received = false;

// Signal handler for SIGHUP
void sig_handler(int signo)
{
    switch(signo) {
    case SIGHUP:
        hup_received = true;
        syslog(LOG_NOTICE, "SIGHUP received.");
        break;
    case SIGTERM:
        hup_received = true;
        syslog(LOG_NOTICE, "SIGTERM received.");
        break;
    default:
        syslog(LOG_ERR, "Unknown signal: %d", signo);
    }
}

extern "C" int read_pid()
{
    // we are a bit quick and dirty here …
    std::ifstream is(PID_FILE, std::ios::in);
    if(!is.is_open()) {
        return -1;
    }

    std::stringstream buf("");
    buf << is.rdbuf();

    const int pid = atoi(buf.str().c_str());

    return pid;
}

extern "C" int send_sighup(const int pid)
{
    return kill(pid, SIGHUP);
}

Daemon::Daemon(const char *_name)
    : m_name(_name), m_lock(0)
{
    openlog(m_name, LOG_PID, LOG_USER);

    signal(SIGHUP, sig_handler);
    signal(SIGTERM, sig_handler);
}

Daemon::~Daemon()
{
    // close the lock file which should lead to its removal
    //if (m_lock)
    //     close(m_lock);

    // close the syslog
    closelog();
}

bool Daemon::seed()
{
    const int childpid = fork();

    if(childpid  < 0) {
        //check to see if we can get a child
        return false;
    } else if(childpid > 0) {
        //if we have a child then parent can exit
        exit(0);
    }
    //Set our sid and continue normal runtime as a forked process
    setsid();
    umask(0);    //Xxx set the unmask

    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    close(STDIN_FILENO);

    // check if there is already an instance running
    if(!store_pid()) {
        return false;
    }

    message(LOG_NOTICE, "Daemon started with PID %d.", getpid());
    return true;
}

bool Daemon::store_pid()
{
    const char *lockfile = PID_FILE;

    if((m_lock = open(lockfile, O_RDWR | O_CREAT, 0644)) == -1) {
        message(LOG_ERR, "Failed to open the PID file.");
        // TODO rather an exception
        return false;
    }

    if(lockf(m_lock, F_TLOCK, 0) == -1) {
        message(LOG_ERR, "A daemon instance is already running!");
        return false;
    }

    std::stringstream path("");
    path << getpid() << std::endl;

    const std::string _s_path = path.str();
    write(m_lock, _s_path.c_str(), _s_path.size());

    return true;
}

void Daemon::message(const int level, const char *msg, ...)
{
    va_list argp;
    va_start(argp, msg);

    vsyslog(level, msg, argp);

    va_end(argp);
}

void Daemon::pause()
{
    usleep(100 * 1000);
}

bool Daemon::sighup()
{
    return hup_received;
}

