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

    xmppsc::MethodHandler* mh = new xmppsc::MethodHandler();

    if (client) {
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

        // TODO wait for response
        sleep(1);

        delete sink;

        // go on in client thread (response arrives here)
        client_thread.join();

        // cleanup
        std::cout << "Disconnect." << std::endl;
        client->disconnect();
	wait_for_state(client, gloox::ConnectionState::StateDisconnected);


        delete client;
    }

    if (af)
        delete af;

    return 0;
}
