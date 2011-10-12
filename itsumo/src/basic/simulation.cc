/*
 Copyright (C) 2005 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
 Bruno Castro da Silva (bcs@inf.ufrgs.br)
 Diogo Becker de Brum (dbrum@inf.ufrgs.br)
 Maicon de Brito do Amarante (mbamarante@inf.ufrgs.br)
 http://www.inf.ufrgs.br/~mas/traffic/siscot

 This file is part of Itsumo.

 Itsumo is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 Itsumo is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Itsumo; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include "simulation.hh"
#include "../socket/ClientSocket.h"
#include "../socket/SocketException.h"

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "../drivers/routerlibs/geradorOD.hh"
#include "../drivers/routed.hh"

#include "sqlite3.h"
//#include "my_global.h" // Include this file first to avoid problems
#include "mysql/mysql.h" // MySQL Include File

#define SERVER "127.0.0.1"
#define USER "root"
#define PASSWORD "root"
#define DATABASE "itsumo"

dcopparams dcop;

int global_odsize;
string global_network;
int global_replan;
int global_comm;
int global_view;
string global_odfile;

char* itoa(int val, int base) {
	static char buf[32] = { 0 };
	int i = 30;
	for (; val && i; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i + 1];
}

vector<DriverLoader *> * Simulation::getDriverLoaderList() {
	return &vet_drivers;
}

Simulation::Simulation(string file) {
	active_cars.reserve(ACTIVE_CARS_PREBUFFER);
	//replace this for something better
	system("rm -f drivers.txt");

	t = new Topology(this);
	n = new Network(t);

	parse_config_file(file);

}

Simulation::~Simulation() {
	//JB::TODO This must be redone! 
	//    just delete the drivers and clear the vector
	vector<DriverLoader *>::iterator dit = vet_drivers.begin();
	DriverLoader * d;
	while (dit != vet_drivers.end()) {
		d = (*dit);
		dit = vet_drivers.erase(dit);
		delete d;
	}

	delete n;
	delete t;

	driverLogger.close();
}

void Simulation::load_xml(string file) {
	load_data_from_file(file, t, n);
}

node_vector_t Simulation::stringToNode(string ids, int isdcop) {
	cout << "string to node" << ids << endl;
	node_vector_t ans;
	ans.clear();
	int isDCOP = 0;
	const char* temp;

	if (ids.substr(0, 1) == "p") // is dcop
		isDCOP = 1;

	if (ids == "a;")//all tl info
		ans = n->tlNodeList();
	else {
		string item;

		// correcao da denise
		string::size_type start = 2;

		// correcao da denise
		string::size_type pos = ids.find(";", 0);
		// unsigned int pos = ids.find(";", 0);

		pos = ids.find(";", 2); //ignoramos o primeiro item

		// read dcop params
		if (isDCOP) {

			temp = ids.substr(start, pos - start).c_str();
			dcop.alg = atoi(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			temp = ids.substr(start, pos - start).c_str();
			dcop.alpha = atof(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			temp = ids.substr(start, pos - start).c_str();
			dcop.gammasynch = atof(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			temp = ids.substr(start, pos - start).c_str();
			dcop.gammaunsynch = atof(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			temp = ids.substr(start, pos - start).c_str();
			dcop.aa = atof(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			temp = ids.substr(start, pos - start).c_str();
			dcop.an = atof(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			temp = ids.substr(start, pos - start).c_str();
			dcop.na = atof(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			temp = ids.substr(start, pos - start).c_str();
			dcop.nn = atof(temp);
			start = pos + 1;
			pos = ids.find(";", start + 1);

			// 	cout << "DCOP PARAMS" << endl;
			// 	cout << "\t" << dcop.alg << endl;
			// 	cout << "\t" << dcop.alpha << endl;
			// 	cout << "\t" << dcop.gammasynch << endl;
			// 	cout << "\t" << dcop.gammaunsynch << endl;
			// 	cout << "\t" << dcop.aa << endl;
			// 	cout << "\t" << dcop.an << endl;
			// 	cout << "\t" << dcop.na << endl;
			// 	cout << "\t" << dcop.nn << endl;

		}

		while (pos != string::npos) {
			string tlId = ids.substr(start, pos - start);

			Node* nd = n->getNodeByTL(tlId);
			if (nd != NULL) {
				ans.push_back(nd);
			} else {
				cerr << "Traffic light " << tlId
						<< " does not exist in this network!!" << endl;
				exit(1);
			}
			start = pos + 1;

			pos = ids.find(";", start + 1);
		}

	}

	return ans;
}

void Simulation::simulate(string round, string table_name) {

	//	sensorsHeaders(numberOfSteps);
	int timeoutSeconds = 1; //JB:: Modified for tests; normal value 10 seconds.
	bool saturationInfo = false;
	bool javasock = false; // for javasocket connections

	unsigned int currentRound = 0;
	unsigned int currentEpisode = 0; // control current episode
	bool reduceOccupationSize = false;

	int countCarsDiff = 0;
	int lastCars = 0;

	MYSQL *connect; // Create a pointer to the MySQL instance
	connect=mysql_init(NULL); // Initialise the instance

	connect=mysql_real_connect(connect,SERVER,USER,PASSWORD,DATABASE,0,NULL,0);
	mysql_query(connect, "START TRANSACTION;");

	try {
		// Create the socket
		ServerSocket server(30000);

		cout << endl << endl << "Waiting for traffic light agents..." << endl;
		cout << "\tPress ENTER when ready to start simulation (or timeout="
				<< timeoutSeconds << " seconds)" << endl << endl;
		for (;;) //implementar: enquanto usuario nao teclar nada
		{

			fd_set handlers;
			struct timeval timeout;
			int retval;

			// inicializa o conjunto de handlers a serem observados
			FD_ZERO(&handlers);
			// inclui stdin (fd=0) no conjunto de handlers
			FD_SET(0, &handlers);
			// inclui socket onde o servidor escuta
			FD_SET( server.getHandler(), &handlers);

			// inicializa estrutura de timeout
			timeout.tv_sec = timeoutSeconds;
			timeout.tv_usec = 0;

			retval = select(server.getHandler() + 1, &handlers, NULL, NULL,
					&timeout);

			if (retval == -1)
				perror("Error in select()!");
			else {
				if (retval) {
					if (FD_ISSET(0, &handlers)) {
						//printf("Ae chegou alguma coisa no teclado\n");
						break;
					} else {
						cout << "New connection arrived" << endl;
					}
				} else {
					break;
				}
			}

			ServerSocket* new_sock = new ServerSocket();
			server.accept(*new_sock);

			try {
				std::string data;
				*new_sock >> data;

				if (data.substr(0, 1) == "j") {
					javasock = true;
				}

				if ((data.substr(0, 1) == "a") || javasock
						|| (data.substr(0, 1) == "s"))//tipo a = pedido de conexao tipo s= conexao com informacao de saturação j=conexao de socket java
				{
					cout
							<< "\tAgent will control the following traffic lights: "
							<< data.substr(2) << endl;
					//devolve mensagem de ok
					if (data.substr(0, 1) == "s") {
						saturationInfo = true;
						cout
								<< "\tAgent will receive flow and degree of saturation info."
								<< endl;
					}
					// Insert values
					node_vector_t nodeList = stringToNode(data, 0);
					tlight_agent_t agent(new_sock, nodeList);
					tlight_agents.push_back(agent);

					if (javasock) {
						*new_sock << n -> tlightConfiguration(nodeList) << "\n";
					} else {
						*new_sock << n -> tlightConfiguration(nodeList);
					}

					cout << "\tAgent registered in ITSUMO with success" << endl;
				} else if (data.substr(0, 1) == "p")// dcop params
				{
					cout
							<< "\tDCOP agent will control the following traffic lights: "
							<< data.substr(2) << endl;
					//devolve mensagem de ok

					// Insert values
					node_vector_t nodeList = stringToNode(data, 1);
					tlight_agent_t agent(new_sock, nodeList);
					tlight_agents.push_back(agent);

					*new_sock << n -> tlightConfiguration(nodeList);

					cout << "\tDCOP agent registered in ITSUMO with success"
							<< endl;
				} else
					*new_sock << "NO";

			} catch (SocketException&) {
			}

		}
	} catch (SocketException& e) {
		std::cout << "Exception was caught:" << e.description()
				<< "\nExiting.\n";
	}

	/*
	 * ROUND loop control
	 * this loop reset network costs
	 * */
	for (currentRound = 1; currentRound <= numberOfRounds; currentRound++) {

		/*
		 * EPISODE loop control
		 * this loop keep global and local network costs unchanged
		 * */
		for (currentEpisode = 1; currentEpisode <= numberOfEpisodes; currentEpisode++) {

			bool staticSituation = false;

			cout << "Starting Simulation Episode # " << currentEpisode << " @ round # " << currentRound
					<< " now!" << endl;

			sensorsHeaders(numberOfSteps, currentEpisode, numberOfEpisodes,
					currentRound, numberOfRounds); // opening sensors
			clock_t startclock = clock();

			int currentStep = 0; // # of steps for each episode
			int countLog = 1; // control log messages exhibition

			size_t active_cars_old_size = active_cars.size();
			int active_cars_old_round = 0;

			/*
			 * new loop control (while)
			 * 	1. run until all steps were complete
			 * 	2. when no iterations (# of steps) were determined in config file, run until no cars left
			 * 	3. always runs at least one step
			 * */
			// This loop executes the simulation steps
			// for (int rounds=0; rounds < iterations; rounds ++ )
			while (((currentStep < numberOfSteps && numberOfSteps > 0)
					|| (numberOfSteps == 0 && active_cars.size() > 0)
					|| currentStep == 0)
						&& !staticSituation) {
				currentStep++;

				if (STEPS_DETECT_SIMULATION_END > 0) {
					if (active_cars_old_size != active_cars.size()) {
						active_cars_old_size = active_cars.size();
						active_cars_old_round = currentStep;
					}
					if (currentStep - active_cars_old_round
							> STEPS_DETECT_SIMULATION_END && numberOfSteps == 1) { // execute when undefined number of steps
						//numberOfSteps = currentStep + 1;	// reduce number of steps to finish!
						staticSituation = true;
						cout << "Running... step = " << currentStep << "| travelling drivers = " << active_cars.size() << endl;
						cout << "Detected static situation; finishing simulation at " << numberOfSteps << " steps." << endl;
					}
				}

				//JB::TODO optimize countLog
				if (countLog == 100) {

					cout << "Running... step = " << currentStep
							<< "| travelling drivers = " << active_cars.size()
							<< endl;

					countLog = 1;

				} else
					countLog++;

				node_vector_t childNodes = n -> getChildNodes();
				node_vector_t::iterator c;

				// manage special drivers
				for (vector<DriverLoader *>::iterator dit = vet_drivers.begin(); dit
						!= vet_drivers.end(); dit++) {
					(*dit)->update(currentStep, reduceOccupationSize, connect, global_odsize, global_network, round, global_odfile, table_name);
				}

				// updates the driver's decisions
				for (active_cars_vector_t::iterator i = active_cars.begin(); i
						!= active_cars.end(); i++) {
					(*i) -> updateDecision();
				}

				// commits the decisions and updates the cars' status
				active_cars_vector_t::iterator i = active_cars.begin();
				active_cars_vector_t::iterator j = active_cars.begin();
				active_cars_vector_t::iterator size = active_cars.end();
				active_cars_vector_t::iterator temp;
				while (i < size) {
					j = i;
					temp = size;
					(*i) -> commitDecision();

					if (currentStep % 300 == 0) { // a cada x steps checa se o driver está parado
						if ((*i) -> getCurrentCell() == (*i) -> getLastCell()) {
							// dealock handler
							(*i) -> getDriver()->force2Out(true);		// remove from network
							(*i) -> getDriver()->force2Wait(true);		// remove from running and put into waiting list
						} else {
							(*i) -> setLastCell((*i) -> getCurrentCell());
						}
					}

					size = active_cars.end();
					if (temp > size) { //if a car has been removed from the vector
						i = j;
					} else {
						i++;
					}
				}

				//Send info messages to traffic light agents
				tlight_agents_t::iterator agt;

				if ((currentStep % sensorInterval) == 0)
					for (agt = tlight_agents.begin(); agt
							!= tlight_agents.end(); agt++) {
						string networkStateInfo, ans;
						//			std::cout << "SIMULATION: Passing message to the client.." << endl;
						if (saturationInfo) {
							networkStateInfo = n->tlightAgentSatFlowData(
									(*agt).second, currentStep);
						} else {
							networkStateInfo = n -> tlightAgentData(
									(*agt).second, currentStep);
						}
						//			cout << "Net info=" << networkStateInfo << endl;
						if (javasock) {
							*((*agt).first) << networkStateInfo << "\n";
						} else {
							*((*agt).first) << networkStateInfo;
						}
						*((*agt).first) >> ans;
						//			cout << "SIMULATION: Received answer:" << ans << endl;
					}

				//Send change request messages to traffic light agents
				if ((currentStep % agentInterval) == 0) {
					int type = dcop.alg; //1=modi_adopt, 2=dpop, 3=optapo, 4=jomi_adopt

					// If we're using a dcop algorithm then we need to generate the log
					if (type) {
						char algType[5];
						sprintf(algType, "%d", type);
						char cRounds[7];
						sprintf(cRounds, "%d", currentStep);
						// log output
						n->generateDCOP(tlight_agents, dcop);

						// Here we need to call the dcop algorithm using the log just created
						char cmd[60] = "../trafficAgents/dcop/rundcop ";
						strcat(cmd, algType);
						strcat(cmd, " ");
						strcat(cmd, cRounds);
						system(cmd);
					}

					for (agt = tlight_agents.begin(); agt
							!= tlight_agents.end(); agt++) {
						string request;
						std::cout
								<< "SIMULATION: Sending change request to the client.."
								<< endl;
						request = "r;";
						if (javasock) {
							request = "r;\n";
						}
						*((*agt).first) << request;

					}
					// AQUI FOI COLOCADO ESSE FOR PRA *ENVIAR* todos os requests antes de *RECEBER* os requests!
					for (agt = tlight_agents.begin(); agt
							!= tlight_agents.end(); agt++) {
						string ans;
						fd_set handlers;
						struct timeval timeout;
						int retval;

						// inicializa o conjunto de handlers a serem observados
						FD_ZERO(&handlers);
						// inclui socket onde o servidor escuta
						FD_SET( (*((*agt).first)).getHandler(), &handlers);

						// inicializa estrutura de timeout
						timeout.tv_sec = 20;
						timeout.tv_usec = 0;

						retval = select((*((*agt).first)).getHandler() + 1,
								&handlers, NULL, NULL, &timeout);

						if (retval == -1) {
							perror("Talk Error!");
						} else {
							if (retval) {
								*((*agt).first) >> ans;

								n -> process_tlight_update(ans);
								//n->XMLOutput();
								//cout << "SIMULATION: Received answer:" << ans << endl;
							}
						}
					}
				}

				// updates the nodes and all the objects they contain (sources, sinks, tlights)
				for (c = childNodes.begin(); c != childNodes.end(); c++) {
					(*c) -> update(currentStep);
				}

				sensorsPrint(currentStep);

				if (currentStep == (numberOfSteps - 1)) //last iteration
					for (agt = tlight_agents.begin(); agt
							!= tlight_agents.end(); agt++) {
						string endMsg, ans;
						std::cout
								<< "SIMULATION: Sending termination message to the client.."
								<< endl;
						if (javasock) {
							endMsg = "end;\n";
						} else {
							endMsg = "end;";
						}
						*((*agt).first) << endMsg;
						*((*agt).first) >> ans;

						cout << "SIMULATION: Received answer:" << ans << endl;
					}
			}

			closeSensors();
			cout << "end of simulation - ";
			cout << "simulation time was:" << (double) (clock() - startclock)
					/ CLOCKS_PER_SEC << endl << endl;

			for (vector<DriverLoader *>::iterator dit = vet_drivers.begin(); dit
					!= vet_drivers.end(); dit++) {
				(*dit)->prepareNewEpisode();
			}

			// reset graph global and local costs
			vet_drivers.front()->prepareNewRound();
		} // episode loop end
	} // round loop end

	n->makeMap(connect, global_odsize, global_network, round, global_replan, global_comm, global_view);

	mysql_query(connect, "COMMIT;");
    mysql_close(connect);

}

void Simulation::sensorsHeaders(int iterations, int currentEpisode,
		int numberOfEpisodes, int currentRound, int numberOfRounds) {
	vector<pair<string, pair<string, ofstream *> > >::iterator i;
	char tmp_char[100];
	string tmp_str;

	stringstream sst;

	// append current round
	if (numberOfRounds > 1) { // when one episode, keep unchange
		sst << "-round_" << currentRound;
	}

	// append current episode
	if (numberOfEpisodes > 1) {
		sst << "-episode_" << currentEpisode;
	}

	for (i = vet_sensors.begin(); i != vet_sensors.end(); i++) {

		string filename = (*i).second.first;
		size_t found = filename.find_last_of(".");
		filename.insert(found, sst.str());

		//(*i).second.second->open(((*i).second.first + sst.str()).c_str());
		(*i).second.second->open(filename.c_str());

		if ((*i).first == "visualization_of_section_densities") {
			*(*i).second.second << n -> printTopologyForVisualization();
			*(*i).second.second << "DATA SECTIONS" << endl;
			// prints the number of iterations the visualization tools should expect
			*(*i).second.second << iterations << endl;

		} else if ((*i).first == "visualization_of_section_relative_densities") {
			*(*i).second.second << n -> printTopologyForVisualization();
			*(*i).second.second << "DATA SECTIONS" << endl;
			// prints the number of iterations the visualization tools should expect
			*(*i).second.second << iterations << endl;

		} else if ((*i).first == "visualization_of_stopped_cars_in_sections") {
			*(*i).second.second << n -> printTopologyForVisualization();
			*(*i).second.second << "DATA SECTIONS" << endl;
			// prints the number of iterations the visualization tools should expect
			*(*i).second.second << iterations << endl;

		} else if ((*i).first == "visualization_of_laneset_densities") {
			*(*i).second.second << n -> printTopologyForVisualization();
			*(*i).second.second << "DATA LANESETS" << endl;
			// prints the number of iterations the visualization tools should expect
			*(*i).second.second << iterations << endl;

		} else if ((*i).first == "visualization_microscopic_level") {
			*(*i).second.second << n -> printTopologyForVisualization();
			*(*i).second.second << "DATA LANES" << endl;
			// prints the number of iterations the visualization tools should expect
			*(*i).second.second << iterations << endl;

		} else if ((*i).first == "controlled_lanes_summary") {
			*(*i).second.second
					<< "#step\tavg.Sat.Deg(%)\tavg.QueueSize(veh)\tTotalQueuedVehicles(veh)\tavg.Speed(km/h)\tavg.DelayTime(s)\tTotal Distance Travelled(veh-Km/hour)"
					<< endl;

		} else if ((*i).first == "total_stopped_cars_in_network") {
			*(*i).second.second
					<< "#Using 'total_stopped_cars_in_network' sensor. For each iteration, the total number"
					<< endl
					<< "#of stopped cars (in the whole network) will be printed."
					<< endl;

		} else if ((*i).first
				== "total_stopped_cars_in_network_without_sources") {
			*(*i).second.second
					<< "#Using 'total_stopped_cars_in_network_without_sources' sensor. For each iteration, the total number"
					<< endl
					<< "#of stopped cars in the whole network except the cars on lanes connected to a source will be printed."
					<< endl;

		} else if ((*i).first == "lane_sensor") {
			*(*i).second.second
					<< "# Step\tLane\tDensity\tAverage speed\tArray of cells"
					<< endl;

		} else if ((*i).first == "laneset_sensor") {
			*(*i).second.second
					<< "# Step\tNode\tLaneSet\tDensity\tDirection of LaneSet"
					<< endl;

		} else if ((*i).first == "laneset_sensor_without_border_nodes") {
			*(*i).second.second
					<< "# Step\tNode\tLaneSet\tDensity\tDirection of LaneSet"
					<< endl;

		} else if ((*i).first == "stopped_cars_in_lanesets") {
			laneSet_vector_t childLaneSets = n->getChildLaneSets();
			laneSet_vector_t::iterator laneset;
			*(*i).second.second
					<< "#Using 'stopped_cars_in_lanesets'. Each line will contain the iteration and the"
					<< endl
					<< "#total number of cars and the number of cars to the following lanesets:"
					<< endl;
			tmp_str = "# step\t";
			for (laneset = childLaneSets.begin(); laneset
					!= childLaneSets.end(); laneset++) {
				sprintf(tmp_char, "%d\t", (*laneset)->getId());
				tmp_str += tmp_char;
			}
			*(*i).second.second << tmp_str << endl;

		} else if ((*i).first == "laneset_densities_sensor") {
			laneSet_vector_t childLaneSets = n->getChildLaneSets();
			laneSet_vector_t::iterator laneset;
			*(*i).second.second
					<< "#Using 'laneset densities' sensor of densities for the following lanesets"
					<< endl;
			tmp_str = "# step\t";
			for (laneset = childLaneSets.begin(); laneset
					!= childLaneSets.end(); laneset++) {
				sprintf(tmp_char, "%d\t", (*laneset)->getId());
				tmp_str += tmp_char;
			}
			*(*i).second.second << tmp_str << endl;

		} else if ((*i).first == "section_in_columns_sensor") {
			street_sections_t::iterator section;
			section_vector_t childSections = n->getChildSections();
			*(*i).second.second
					<< "#Using 'section-in-columns' sensor of densities and avg speed ";
			*(*i).second.second << "for the following sections:" << endl
					<< "# step\t";
			for (section = childSections.begin(); section
					!= childSections.end(); section++)
				*(*i).second.second << (*section)->getId() << "\t";
			*(*i).second.second << endl;

		} else if ((*i).first == "section_in_columns_sensor_printing_names") {
			street_sections_t::iterator section;
			section_vector_t childSections = n->getChildSections();
			*(*i).second.second
					<< "#Using 'section-in-columns' sensor of densities and avg ";
			*(*i).second.second << "speed for the following sections:" << endl
					<< "# step\t";
			for (section = childSections.begin(); section
					!= childSections.end(); section++)
				*(*i).second.second << (*section)->getName() << "\t";
			*(*i).second.second << endl;
		} else if ((*i).first == "travel_times") {
			*(*i).second.second
					<< "#TrvTime(steps)\tStepInserted\tStepRemoved\tReplans\tReplanChanges\tTolerance\tTouteSize"
					<< endl;
		} else if ((*i).first == "avg_travel_times") {
			*(*i).second.second
					<< "#AverageTravelTime\tLastStepIn\tLastStepOut\tReplans\tReplanChanges"
					<< endl;
		} else {
			cout << "Sensor " << (*i).first << " unknown!" << endl;
			exit(1);
		}
	}

}

void Simulation::closeSensors() {

	vector<pair<string, pair<string, ofstream *> > >::iterator i;

	for (i = vet_sensors.begin(); i != vet_sensors.end(); i++) {
		if ((*i).first == "travel_times") {
			/******************* TRAVEL TIME SENSOR ********************/
			for (vector<DriverLoader *>::iterator it = vet_drivers.begin(); it
					!= vet_drivers.end(); ++it)
				*(*i).second.second << (*it)->printTravelTimesSensor();
		}

		if ((*i).first == "avg_travel_times") {
			/******************* AVG TRAVEL TIME SENSOR ********************/
			for (vector<DriverLoader *>::iterator it = vet_drivers.begin(); it
					!= vet_drivers.end(); ++it)
				*(*i).second.second << (*it)->printAvgTravelTimesSensor();
		}

		(*i).second.second->close();

	}
}

void Simulation::sensorsPrint(int step) {
	vector<pair<string, pair<string, ofstream *> > >::iterator i;

	for (i = vet_sensors.begin(); i != vet_sensors.end(); i++)
		if ((step % sensorInterval) == 0) {
			if ((*i).first == "controlled_lanes_summary")
				*((*i).second.second) << n->controlledLanesSensorSummary(step)
						<< flush;

			if ((*i).first == "lane_sensor")
				*((*i).second.second) << n -> laneSensorSpeedUp(step) << flush;

			if ((*i).first == "laneset_sensor")
				*((*i).second.second) << n -> laneSetSensor(step) << flush;

			if ((*i).first == "laneset_sensor_without_border_nodes")
				*((*i).second.second) << n -> laneSetSensorWithoutBorderNodes(
						step) << flush;

			if ((*i).first == "laneset_densities_sensor")
				*((*i).second.second) << n -> laneSetDensitiesSensor(step)
						<< flush;

			if ((*i).first == "section_in_columns_sensor")
				*((*i).second.second) << n -> sectionSensor(step) << flush;

			if ((*i).first == "section_in_columns_sensor_printing_names")
				*((*i).second.second) << n -> sectionSensor(step) << flush;

			if ((*i).first == "visualization_of_laneset_densities")
				*((*i).second.second)
						<< n -> visualLaneSetDensitiesSensor(step) << flush;

			if ((*i).first == "visualization_microscopic_level")
				*((*i).second.second) << n -> visualMicroscopic(step) << flush;

			if ((*i).first == "visualization_of_section_densities")
				*((*i).second.second)
						<< n -> visualSectionDensitiesSensor(step) << flush;

			if ((*i).first == "visualization_of_section_relative_densities")
				*((*i).second.second)
						<< n -> visualSectionRelativeDensitiesSensor(step)
						<< flush;

			if ((*i).first == "visualization_of_stopped_cars_in_sections")
				*((*i).second.second) << n -> visualStoppedCarsInSectionSensor(
						step) << flush;

			if ((*i).first == "stopped_cars_in_lanesets")
				*((*i).second.second) << n -> stoppedCarsInLaneSetSensor(step)
						<< flush;

			if ((*i).first == "total_stopped_cars_in_network")
				*((*i).second.second) << n -> totalStoppedCarsSensor(step)
						<< flush;

			if ((*i).first == "total_stopped_cars_in_network_without_sources")
				*((*i).second.second)
						<< n -> totalStoppedCarsSensorWithoutSources(step)
						<< flush;
		}
}

#define OBLIGATORY 0
#define OPTIONAL 1

#define CHECK(element, mode) (check(element, #element, mode))
#define VALUE(element) (element->FirstChild()->Value())

bool Simulation::check(TiXmlElement *element_to_test, string element_name,
		int condition) {
	if (element_to_test == 0) {
		cout << "Tag '" << element_name << "' expected but not found!" << endl;
		exit(1);
	}

	if (element_to_test->FirstChild() == 0) {
		if (condition == OBLIGATORY) {
			cout << "Tag '" << element_name << "' must have a value!" << endl;
			exit(1);
		} else
			return false;
	} else {
		return true;
	}
}

void Simulation::parse_drivers(TiXmlElement *drivers) {

	TiXmlElement *driver;
	TiXmlElement *name;
	TiXmlElement *nick;
	TiXmlElement *number;
	TiXmlElement *state;
	TiXmlNode *option;
	TiXmlElement *options;
	TiXmlNode *route;
	TiXmlElement *routes;

	vector<pair<string, string> > ops;
	vector<Route> rts;

	driverLogger.open("drivers.txt", ofstream::out | ofstream::app);

	// log file header
	driverLogger << "T_GBL\t";
	driverLogger << "VEH_ID\t";
	driverLogger << "LEG_NR\t";
	driverLogger << "LINK_ID\t";
	driverLogger << "FROM_NODE_ID\t";
	driverLogger << "EVENT_FLAG\t";
	driverLogger << "DESCRIPTION";
	driverLogger << endl;

	for (driver = drivers->FirstChildElement("driver"); driver; driver
			= driver->NextSiblingElement("driver")) {
		ops.clear();
		rts.clear();
		state = driver->FirstChildElement("state");
		if ("ON" == static_cast<string> (VALUE(state))) {
			name = driver->FirstChildElement("name");
			nick = driver->FirstChildElement("nick");
			number = driver->FirstChildElement("number");
			options = driver->FirstChildElement("options");
			routes = driver->FirstChildElement("routes");

			if (CHECK(options, OPTIONAL)) {
				for (option = options->FirstChild(); option; option
						= option->NextSibling()) {
					string val = "";

					if (option->FirstChild() != NULL)
						val = VALUE(option);

					ops.push_back(make_pair(option->ToElement()->Value(), val));
				}
			}

			if (routes != 0) {
				for (route = routes->FirstChild(); route; route
						= route->NextSibling()) {
					Route r;
					r.laneset
							= atoi(VALUE(route->FirstChildElement("laneset")));
					rts.push_back(r);
				}
			}
			vet_drivers.push_back(
					new DriverLoader(n, static_cast<string> (VALUE(name)),
							static_cast<string> (VALUE(nick)),
							static_cast<int> (atoi(VALUE(number))), ops, rts,
							&driverLogger));
		}
	}
}

void Simulation::parse_sensors(TiXmlElement *sensors) {

	TiXmlElement *sensor;
	TiXmlElement *name;
	TiXmlElement *file;
	TiXmlElement *state;
	TiXmlElement *timetag;

	char timebuf[20];

	time_t myTime = time(0);
	strftime(timebuf, 20, "%H%M%S", localtime(&myTime));
	//cout<<"test time:"<<timebuf<<":"<<endl;
	string timestr("_");
	timestr.append(timebuf);

	for (sensor = sensors->FirstChildElement("sensor"); sensor; sensor
			= sensor->NextSiblingElement("sensor")) {
		state = sensor->FirstChildElement("state");

		timetag = sensor->FirstChildElement("time_tag");

		if ("ON" == static_cast<string> (VALUE(state))) {
			name = sensor->FirstChildElement("name");
			file = sensor->FirstChildElement("file");

			string filename = static_cast<string> (VALUE(file));

			if (timetag->FirstChild() != NULL) {
				if ("YES" == static_cast<string> (VALUE(timetag))) {
					size_t found;
					found = filename.find_last_of(".");
					filename.insert(found, timestr);
					cout << "filename: " << filename << endl;
				}
			} else {
				cout << "WARNING: time_tag has NULL value!" << endl;
			}

			//			vet_sensors.push_back(
			//					make_pair(static_cast<string> (VALUE(name)),
			//							make_pair(filename, new ofstream(filename.c_str()))));

			vet_sensors.push_back(
					make_pair(static_cast<string> (VALUE(name)),
							make_pair(filename, new ofstream())));

		}
	}

}

void Simulation::parse_config(TiXmlElement *config) {
	TiXmlElement *cs;
	TiXmlElement *cms;
	TiXmlElement *il;
	TiXmlElement *sensors;
	TiXmlElement *drivers;
	TiXmlElement *file_name;
	TiXmlElement *steps;
	TiXmlElement *dece;
	TiXmlElement *si;
	TiXmlElement *ai;
	TiXmlElement *odm_name;
	TiXmlElement *odm_size;
	TiXmlElement *xreplan;
	TiXmlElement *xcosts_view;
	TiXmlElement *xcommunication;
	TiXmlElement *xepisodes;
	TiXmlElement *xrounds;

	file_name = config->FirstChildElement("file");

	odm_name = config->FirstChildElement("odmatrix");

	odm_size = config->FirstChildElement("odsize");

	steps = config->FirstChildElement("steps");

	dece = config->FirstChildElement("default_deceleration");

	si = config->FirstChildElement("sensor_interval");

	ai = config->FirstChildElement("agent_interval");

	cs = config->FirstChildElement("cell_size");
	cms = config->FirstChildElement("car_max_speed");
	il = config->FirstChildElement("iteration_length");

	default_deceleration = atof(VALUE(dece));
	numberOfSteps = atoi(VALUE(steps));
	sensorInterval = atoi(VALUE(si));
	agentInterval = atoi(VALUE(ai));
	topology_file = string(VALUE(file_name));

	t->setCellSize(atof(VALUE(cs)));
	t->setIterationTime(atoi(VALUE(il)));
	t->setCarsMaximumSpeed(atoi(VALUE(cms)));

	load_xml(topology_file);

	sensors = config->FirstChildElement("sensors");
	if (CHECK(sensors, OBLIGATORY)) {
		parse_sensors(sensors);
	}

	xreplan = config->FirstChildElement("replan");
	xcosts_view = config->FirstChildElement("costs_view");
	xcommunication = config->FirstChildElement("communication");
	xrounds = config->FirstChildElement("rounds");
	xepisodes = config->FirstChildElement("episodes");

	if (!xrounds) {
		numberOfRounds = 1;
	} else if (atoi(VALUE(xrounds)) > 0) {
		numberOfRounds = atoi(VALUE(xrounds));
	} else {
		numberOfRounds = 1; // if not defined, one episode by default
	}

	if (!xepisodes) {
		numberOfEpisodes = 1;
	} else if (atoi(VALUE(xepisodes)) > 0) {
		numberOfEpisodes = atoi(VALUE(xepisodes));
	} else {
		numberOfEpisodes = 1; // if not defined, one episode by default
	}

	string costs_view;
	if (xcosts_view) {
		costs_view = VALUE(xcosts_view);
		global_view = static_cast<string> VALUE(xcosts_view) == "GLOBAL" ? 1 : 0;
	} else {
		costs_view = "GLOBAL";
		global_view = 1;
	}

	bool replan;
	bool communication;

	if (xreplan) {
		replan = (atoi(VALUE(xreplan)) != 0);
		global_replan = (atoi(VALUE(xreplan)) != 0);
	} else {
		replan = false;
		global_replan = 0;
	}

	if (xcommunication) {
		communication = (atoi(VALUE(xcommunication)) != 0);
		global_comm = (atoi(VALUE(xcommunication)) != 0);
	} else {
		communication = false;
		global_comm = 0;
	}

	cout << "Number of rounds: " << numberOfRounds << endl;
	cout << "Number of episodes: " << numberOfEpisodes << endl;
	cout << "Costs View: " << costs_view << endl;
	cout << "En-route replanning: " << (replan ? "YES" : "NO") << endl;
	cout << "Communication: " << (communication ? "YES" : "NO") << endl << endl;

	drivers = config->FirstChildElement("drivers");
	if (CHECK(drivers, OPTIONAL)) {
		parse_drivers(drivers);
	}
	//JB:: Load routed drivers from OD matrix
	if (odm_name != 0) // it works even without "odmatrix" tag
	{
		if (CHECK(odm_name, OPTIONAL)) {
			load_odmatrix(string(VALUE(odm_name)),
					atoi(string(VALUE(odm_size)).c_str()), n, costs_view,
					replan, communication);
		}
	}

}

float Simulation::getDeceleration() {
	return default_deceleration;
}

void Simulation::parse_config_file(string file) {
	TiXmlElement *config;

	// 	TiXmlBase::SetCondenseWhiteSpace( false );


	TiXmlDocument doc(file.c_str());

	bool loadOkay = doc.LoadFile();

	if (!loadOkay) {
		printf("Could not load file. Error='%s'.\nExiting.\n", doc.ErrorDesc());
		exit(1);
	}

	config = doc.FirstChildElement("config");
	// doc.Print( stdout );

	parse_config(config);

}

void Simulation::parse_od_drivers(TiXmlElement *od_drivers, int odsize,
		Network *n, string costs_view, bool replan, bool communication) {

	TiXmlElement *od_driver;
	TiXmlElement *od_driver_id;
	TiXmlElement *source;
	TiXmlElement *sink;

	Node* Src;
	Node* Dst;

	vector<pair<string, string> > ops;
	vector<Route> rts;
	DriverLoader * tDL;
	vector<Driver *> drivers;

	driverLogger.open("drivers.txt", ofstream::out | ofstream::app);

	// log file header
	driverLogger << "T_GBL\t";
	driverLogger << "VEH_ID\t";
	driverLogger << "LEG_NR\t";
	driverLogger << "LINK_ID\t";
	driverLogger << "FROM_NODE_ID\t";
	driverLogger << "EVENT_FLAG\t";
	driverLogger << "DESCRIPTION";
	driverLogger << endl;

	int numberOfVehicles = 0, loadedVehicles = 0;

	ops.clear();
	rts.clear();

	tDL = new DriverLoader(n, static_cast<string> ("Routed"),
			static_cast<string> (VALUE(od_driver_id)), odsize, //JB::verify! static_cast<int> (1), //number of car with this route (only one)
			ops, rts, &driverLogger);
	drivers = tDL->getWaitingDrivers();

	if (drivers.size() /* > 0 */) {
		(static_cast<RoutedDriver *> (drivers.front()))->setNetwork(n);

		od_driver = NULL;
		for (vector<Driver *>::iterator it = drivers.begin(); it
				!= drivers.end(); ++it) {
			// get one oddriver
			if (od_driver)
				od_driver = od_driver->NextSiblingElement("od_driver");
			else if (od_driver == NULL)
				od_driver = od_drivers->FirstChildElement("od_driver");

			od_driver_id = od_driver->FirstChildElement("od_driver_id");
			source = od_driver->FirstChildElement("source");
			sink = od_driver->FirstChildElement("sink");
			Src = n->getNodeById(VALUE(source));
			Dst = n->getNodeById(VALUE(sink));
			(*it)->setNick(static_cast<string> (VALUE(od_driver_id)));
			//GA::TODO add a config option for using local info or not
			//(static_cast<RoutedDriver *>(*it))->useLocalInfo(true);

			(static_cast<RoutedDriver *> (*it))->setViewCost(costs_view); // cost view: global or local
			(static_cast<RoutedDriver *> (*it))->setReplan(replan); // replan or not
			(static_cast<RoutedDriver *> (*it))->setCommunication(communication); // communicate or not

			(static_cast<RoutedDriver *> (*it))->set_source_destination(Src,
					Dst);
			(static_cast<RoutedDriver *> (*it))->FindShortestPath();
			(static_cast<RoutedDriver *> (*it))->setPathSize();

			//JB::TODO Check route before commit the driver;
			++loadedVehicles;
			++numberOfVehicles;
		}

		vet_drivers.push_back(tDL);
	} else
		delete tDL;

	cout << "Number of Routed Vehicles loaded: " << loadedVehicles << " of "
			<< numberOfVehicles << " tried." << endl;
}

void Simulation::load_odmatrix(string file, int odsize, Network *n,
		string costs_view, bool replan, bool communication) {

	TiXmlElement *od_drivers;
	char homePath[512];
	string hP;

	global_odfile = file;

	getcwd(homePath, 512);
	hP = homePath;

	hP.erase(hP.find("/src/basic"));

	if (file.find(hP, 0) == string::npos && file.substr(0, 1).find("/")
			== string::npos) //home path not found && first caracter "/" not found too
		file = "../../" + file;

	// TIAGO -- se o arquivo de od não exite, chamar o gerador de OD e criar o arquivo
	FILE* arq;
	if (!(arq = fopen(file.c_str(), "r"))) {

		geradorOD* gerador = new geradorOD(odsize);
		int source = 0;
		int sink = 0;
		matod mat;

		RoutedDriver *rd = new RoutedDriver("Routed");
		rd->setNetwork(n);

		//JB:: replace with new routeddriver
		//		DijkstraSSD* sp = new DijkstraSSD::DijkstraSSD(n);


		cout << "Gerando novo arquivo OD... " << endl;
		gerador->getNodos(n->getChildNodes());

		/************  LISTA OD - EQUIPROVÁVEL **********/
		for (unsigned int i = 0; i < gerador->getNumVehicles(); i++) {
			do {
				stringstream str;
				stringstream str2;
				source = gerador->getSource();
				sink = gerador->getSink(source);
				str << source;
				Node *srce = n->getNodeById(str.str());
				str2 << sink;
				Node *snk = n->getNodeById(str2.str());
				rd->set_source_destination(srce, snk);
				rd->FindShortestPath();
				rd->setPathSize();
			} while (rd->getlinksPathSize() <= 2); //nao criar rotas mais curtas que dois links
			gerador->addOdDriver(source, sink);
		}
		/************  LISTA OD - EQUIPROVÁVEL - END **********/

		/***************** MATRIZ OD ***************/
//		 gerador->loadMatrix("../../input/matrix.6x6.txt");          // __ <-- vai o caminho de onde tu coloca o txt
//		 for(unsigned int i=0;i<gerador->getNumVehicles();i++){
//		 do{
//		 stringstream str;
//		 stringstream str2;
//		 mat=gerador->getRandomOD();
//		 source=mat.source;
//		 sink=mat.sink;
//		 str<<source;
//		 Node *srce=n->getNodeById(str.str());
//		 str2<<sink;
//		 Node *snk=n->getNodeById(str2.str());
//		 rd->set_source_destination(srce, snk);
//		 rd->FindShortestPath();
//		 rd->setPathSize();
//		 }while(rd->getlinksPathSize() <= 2); //
//		 gerador->addOdDriver(source, sink);
//		 }
		 /***************** MATRIZ OD - END ***************/

		 // abaixo serve tanto para ODL quanto ODM

		 cout << "Arquivo OD Gerado com sucesso! Total de ODs: " << odsize
				<< endl;

		gerador->geraODM(file);

		/*
		 * Metodo para gerar o arquivo com o numero que cada par foi sortado.
		 * Funciona somente quando a OD eh gerada, ou seja,
		 * se o arquivo OD ja exite esse arquivo nao sera gerado.
		 */
		// gerador->medirPorcentagens("../../output/POART.txt");

	} else
		cout << "Arquivo OD já existe!" << endl;

	cout << "Abrir odm: " << file << endl;
	TiXmlDocument doc(file.c_str());
	bool loadOkay = doc.LoadFile();

	if (!loadOkay) {
		printf(
				"parse_matrix.c - Could not load file %s. Error='%s'.\nExiting.\n",
				file.c_str(), doc.ErrorDesc());
		exit(1);
	}

	od_drivers = doc.FirstChildElement("od_drivers");

	parse_od_drivers(od_drivers, odsize, n, costs_view, replan, communication);

	global_odsize = odsize;
	global_network = n->getName();

}

//void Simulation::updateGraphWeights(Car *car){
//
//	static_cast<FC *>(car)->dj->updateGraphWeights(graph);
//}
