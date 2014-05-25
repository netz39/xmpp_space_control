#include <iostream>
#include <thread>

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

int main(int argc, char **argv) {
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

    xmppsc::MethodHandler* i2ch = new xmppsc::MethodHandler();

    if (client) {
        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, i2ch,
                new xmppsc::TextSpaceCommandSerializer(), af);

        // connect
        std::thread client_thread(run_client, client);

        while (client->state() != gloox::ConnectionState::StateConnected)
            sleep(1);

	std::cout << "Connected." << std::endl;

        xmppsc::SpaceCommand::space_command_params parms;
        parms["device"] = "0x22";
        parms["command"] = "0x1";
        parms["data"] = "0x5";
        const xmppsc::SpaceCommand sc("i3c.call", parms);

        xmppsc::SpaceCommandSink* sink = scc->create_sink(gloox::JID("i2c@platon"), "cli_thread");

        sink->sendSpaceCommand(sc);

        // wait for response
        sleep(1);

        delete sink;

        std::cout << "Disconnecct." << std::endl;
        client->disconnect();

        // wait for client thread
        client_thread.join();

        delete client;
    }

    if (af)
        delete af;

    return 0;
}
