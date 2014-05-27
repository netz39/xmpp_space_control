#ifndef __DAEMON_H__
#define __DAEMON_H__

#include <string>

#include <syslog.h>

namespace xmppsc {

extern "C" int send_sighup(const int pid);

class Daemon {
public:
    Daemon(const std::string& _name, const std::string& _pid_path="");
    ~Daemon();

    bool seed();
    int read_pid();

    void message(const int level, const char *msg, ...);

    /// SigHUP received?
    bool sighup();

protected:
    /// Store the PID in PID_FILE
    /*!
     * @returns true if successfull, false if the PID was already stored, i.e. the daemon is already running.
     */
    bool store_pid();
private:
    const std::string m_name;
    const std::string m_pid_path;
    int m_lock;
};


} // namespace xmppsc


#endif // __DAEMON_H__
