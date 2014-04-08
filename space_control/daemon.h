#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <syslog.h>

namespace xmppsc
{
//TODO configure somewhere else
#define PID_FILE    "/var/run/spacecontrol/pid"

extern "C" int read_pid();
extern "C" int send_sighup(const int pid);

class Daemon
{
public:
    Daemon(const char *_name);
    ~Daemon();

    bool seed();
    int read_pid();

    void message(const int level, const char *msg, ...);

    /// Pause for 100ms
    void pause();

    /// SigHUP received?
    bool sighup();

protected:
    /// Store the PID in PID_FILE
    /*!
     * @returns true if successfull, false if the PID was already stored, i.e. the daemon is already running.
     */
    bool store_pid();
private:
    const char *m_name;
    int m_lock;
};


} // namespace wd


#endif // __DAEMON_H__
