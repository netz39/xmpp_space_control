#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>

#include "../space_control/configuredclientfactory.h"
#include "../space_control/spacecontrolclient.h"
#include "../space_control/methodhandler.h"

#include "../i3c_client/i2cmethods.h"

void run_client(gloox::Client* client) {
    std::cout << "Connecting GLOOX client." << std::endl;
    if (!client->connect(true)) {
        std::cerr << "could not connect!" << std::endl;
        return;
    }

    std::cout << "Client connection finished." << std::endl;
}

void wait_for_state(const gloox::Client* client, const gloox::ConnectionState& state) {
    while (client->state() != state)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

class FinalizingCommandMethod : public xmppsc::CommandMethod {
public:
    FinalizingCommandMethod(gloox::Client* _client, const std::string _command)
        : CommandMethod(_command), m_client(_client) {}

    FinalizingCommandMethod(gloox::Client* _client, const t_command_set _commands)
        : CommandMethod(_commands), m_client(_client) {}

    //! Evaluate the result and decide if the client may be terminated.
    virtual bool evaluate_result(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink) = 0;

    virtual void handleSpaceCommand(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink) {
        if (evaluate_result(peer, sc, sink)) {
            client()->disconnect();
            wait_for_state(client(), gloox::ConnectionState::StateDisconnected);
        }
    }
protected:
    gloox::Client* client() const throw() {
        return m_client;
    }

private:
    gloox::Client* m_client;
};

class I3CResponseMethod : public FinalizingCommandMethod {
public:
    I3CResponseMethod(gloox::Client* _client) throw()
        : FinalizingCommandMethod(_client, "i3c.response") {}
    virtual ~I3CResponseMethod() throw() {}

    virtual bool evaluate_result(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink) {
        // evaluate result
        try {
            const std::string response = sc.param("response");
            std::cout << response << std::endl;
        } catch (const xmppsc::MissingCommandParameterException& e) {
            std::cerr << "Missing response!" << std::endl;
        }

        return true;
    }
};

class I3CExceptionHandler : public FinalizingCommandMethod {
public:
    I3CExceptionHandler(gloox::Client* _client) throw()
        : FinalizingCommandMethod(_client, "exception")  {}

    virtual ~I3CExceptionHandler() throw() {}

    virtual bool evaluate_result(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink) {
        // print the exception

        std::cerr << "Exception: " << std::endl;

        for (xmppsc::SpaceCommand::space_command_params::const_iterator it = sc.params().cbegin();
                it != sc.params().cend(); it++)
            std::cerr << it->first << ": " << it->second << std::endl;

        return true;
    }
};


class I3CTimeoutHandler : public FinalizingCommandMethod {
public:
    I3CTimeoutHandler(gloox::Client* _client) throw()
        : FinalizingCommandMethod(_client, "i3c.timeout")  {}

    virtual ~I3CTimeoutHandler() throw() {}

    virtual bool evaluate_result(gloox::JID peer, const xmppsc::SpaceCommand& sc, xmppsc::SpaceCommandSink* sink) {
        // print timeout error message

        std::cerr << "TTL-Timeout on I2C sending, something went wrong with bus, message or processing on the device!" << std::endl;

        return true;
    }
};

//TODO exception handling (denied)

int main(int argc, char **argv) {
    //TODO evaluate CLI arguments

    const std::string peer("i2c@platon");
    const std::string i3c_device("0x22");
    const std::string i3c_command("0x1");
    const std::string i3c_data("0x5");

    // Client configuration
    gloox::Client* client=0;
    xmppsc::AccessFilter* af=0;
    try {
        xmppsc::ConfiguredClientFactory ccf("spacecontrol.config");
        client = ccf.newClient();
        af = ccf.newAccessFilter();
    } catch (xmppsc::ConfiguredClientFactoryException &ccfe) {
        std::cerr << "ConfiguredClientFactoryException: " << ccfe.what() << std::endl;
        return (-1);
    }


    if (client) {
        xmppsc::MethodHandler* mh = new xmppsc::MethodHandler();
        mh->add_method(new I3CResponseMethod(client));
        mh->add_method(new I3CExceptionHandler(client));
        mh->add_method(new I3CTimeoutHandler(client));


        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, mh,
                new xmppsc::TextSpaceCommandSerializer(), af);

        // connect
        std::thread client_thread(run_client, client);
        wait_for_state(client, gloox::ConnectionState::StateConnected);
        std::cout << "Connected." << std::endl;


        // Send the I3C command
        xmppsc::SpaceCommand::space_command_params parms;
        parms["device"] = i3c_device;
        parms["command"] = i3c_command;
        parms["data"] = i3c_data;
        const xmppsc::SpaceCommand sc("i3c.call", parms);

        xmppsc::SpaceCommandSink* sink = scc->create_sink(gloox::JID(peer), "cli_thread");

        sink->sendSpaceCommand(sc);

        // go on in client thread (response arrives here)
        client_thread.join();

        // cleanup
        delete sink;


        delete client;
    }

    if (af)
        delete af;

    return 0;
}
