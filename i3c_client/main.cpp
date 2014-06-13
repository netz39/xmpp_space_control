#include <stdlib.h>
#include <popt.h>

#include <iostream>

#include <xmppsc/configuredclientfactory.h>
#include <xmppsc/spacecontrolclient.h>
#include <xmppsc/methodhandler.h>
#include <xmppsc/daemon.h>


#include "i2cmethods.h"
#include "i3cmethods.h"
#include "i2cendpoint.h"

class Options {
public:
    Options() : foreground(false), pid_file("") {}

    bool read_options(int argc, const char* argv[]);

    bool foreground;
    std::string pid_file;
};

bool Options::read_options(int argc, const char* argv[]) {
    char* _pid_file=0;

    struct poptOption optionsTable[] = {
        {"foreground", 0, POPT_ARG_NONE | POPT_ARGFLAG_OPTIONAL , 0, 'd', "Run in foreground, not as daemon", NULL},
        {"pidfile", 'p', POPT_ARG_STRING | POPT_ARGFLAG_OPTIONAL, &_pid_file, 0, "PID file", "path to PID file"},
	POPT_AUTOHELP
	{ NULL, 0, 0, NULL, 0 }
    };
    poptContext optCon = poptGetContext(argv[0], argc, argv, optionsTable, 0);

    // opt processing
    char c;
    while ((c = poptGetNextOpt(optCon)) >= 0) {
        switch (c) {
        case 'd':
            this->foreground = true;
            break;
        }
    }


    if (c < -1) {
        // an error occurred during option processing
        std::cerr << poptBadOption(optCon, POPT_BADOPTION_NOALIAS) << ": " << poptStrerror(c) << std::endl;
        return false;
    }

    // Extract the PID file
    this->pid_file = std::string(_pid_file ? _pid_file : "/var/run/i3c_client");

    return true;
}


int main(int argc, const char* argv[]) {
    Options opt;
    if (!opt.read_options(argc, argv)) {
        std::cerr << "Error parsing command line options!" << std::endl;
        exit(EXIT_FAILURE);
    }


    xmppsc::Daemon daemon("I3Cclient", opt.foreground ? "" : opt.pid_file);
    // only seed if foreground option is not set
    if (opt.foreground)
        std::cout << "Running in foreground mode." << std::endl;
    else if (!daemon.seed()) {
        std::cerr << "Daemon already running!" << std::endl;
        exit(EXIT_FAILURE);
    }

    xmppsc::I2CEndpointBroker*  broker = new xmppsc::I2CEndpointBroker();

    //xmppsc::I2CEndpoint* ep = broker->endpoint(0x22);
    // Send a double beep
    //std::cout << std::hex << "0x" << ep->read_reg_16(0x95) << std::endl;

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

    i2ch->add_method(new xmppsc::I2CReadMethod(broker));
    i2ch->add_method(new xmppsc::I2CRead8Method(broker));
    i2ch->add_method(new xmppsc::I2CRead16Method(broker));
    i2ch->add_method(new xmppsc::I2CWriteMethod(broker));
    i2ch->add_method(new xmppsc::I2CWrite8Method(broker));
    i2ch->add_method(new xmppsc::I2CWrite16Method(broker));
    i2ch->add_method(new xmppsc::I3CCallMethod(broker));


    if (client) {
        // Use the "eco" variant
        xmppsc::set_eco_tcp_client(client);

        xmppsc::SpaceControlClient* scc = new xmppsc::SpaceControlClient(client, i2ch,
                new xmppsc::TextSpaceCommandSerializer(), af);

        if (!client->connect(true))
            std::cerr << "could not connect!" << std::endl;

        delete client;
    }

    if (af)
        delete af;

    delete broker;

    return 0;
}
