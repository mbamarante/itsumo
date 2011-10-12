#!/usr/bin/python

#############################################################################
### CONFIGURACOES DA TOPOLOGIA PARA O PARADOXO DE BRAESS

# Velocidade Maxima
vel_max = 200

# OP/QD -> 2-4
# OQ/PD -> 4-6
# QP    -> 5-7

# Velocidades maximas em cada link
vel = [ 200,	# Entrada
	3, 	# OP  |
	5,	# OQ  -
	5, 	# PD  -
	3, 	# QD  |
	7	# QP  \
	]

# Ocorre o paradoxo ?  (1=sim /0=nao) 
paradox = 1


# Tamanho de cada Link 	---  QP eh automaticamente calculada
tamanhos = [ 	200,	# entrada
		1500,	# OP & QD  |
		1500,	# OQ & PD  -
		] 


# ok. acho uqe da pra deixar um com uma faixa e os outros com 2 (oq) e 3 (qp)
# Numero de lanes em cada link
num_lanes = [	5, 	# Entrada
		2,	# OP  |
		3,	# OQ  -
		3,	# PD  -
		2,	# QD  |
		5	# QP  \
		]

 
desal = [ 0, 	# Entrada
	  0.25,	# OP  |
	  0.25,	# OQ  - 
	  0.25,	# PD  -
	  0.25,	# QD  |
	  0.25	# QP  \
	  ] 



### FIM DAS CONFIGURACOES
##############################################################################


#                       X                            Y				
source_node = [ 100, 				tamanhos[1]+100 ]
o_node      = [ source_node[0] + tamanhos[0], 	source_node[1] ]
p_node      = [ o_node[0], 			100 ]
q_node      = [ o_node[0] + tamanhos[2], 	o_node[1] ]
d_node      = [ q_node[0], 			p_node[1] ]




def cria_lane(street, section, laneset, lane, vel, desal):
	lan = "\
								<lane>\n\
									<lane_id>" + str(street) + str(section) + str(laneset) + str(lane)  + "</lane_id>\n\
									<lane_position>" + str(lane) + "</lane_position>\n\
									<maximum_speed> %d " % vel + "</maximum_speed>\n\
									<deceleration_prob>" + str(desal) + "</deceleration_prob>\n\
								</lane>\
"
	return lan



def cria_lanes(street,section,laneset,num,vel, desal):
	lanes = 1
	s = ""
	while lanes <= num:
		s = s + cria_lane(street,section,laneset,lanes,vel,desal)
		lanes +=1
	return s+"\n"






#################################


head = "\n\
<simulation>\n\
	<network_id>1</network_id>\n\
	<network_name>Braess</network_name>\n\
	<settings>\n\
		<cell_size_in_meters>1</cell_size_in_meters>\n\
		<iteration_length_in_seconds>1</iteration_length_in_seconds>\n\
		<sensor_activation_frequency>1</sensor_activation_frequency>\n\
		<cars_maximum_speed> "+ "%d" % vel_max + " </cars_maximum_speed>\n\
		<trafficlight_agent_observation_frequency>720</trafficlight_agent_observation_frequency>\n\
	</settings>"

nodes = "\n\
	<nodes>\n\
		<node>\n\
			<node_id>11</node_id>\n\
			<node_name>Source</node_name>\n\
			<x_coord> %d " % source_node[0] + "</x_coord>\n\
			<y_coord> %d " % source_node[1] + "</y_coord>\n\
		</node>\n\
		<node>\n\
			<node_id>12</node_id>\n\
			<node_name>O</node_name>\n\
			<x_coord> %d " % o_node[0] + "</x_coord>\n\
			<y_coord> %d " % o_node[1] + "</y_coord>\n\
		</node>\n\
		<node>\n\
			<node_id>13</node_id>\n\
			<node_name>P</node_name>\n\
			<x_coord> %d " % p_node[0] + "</x_coord>\n\
			<y_coord> %d " % p_node[1] + "</y_coord>\n\
		</node>\n\
		<node>\n\
			<node_id>14</node_id>\n\
			<node_name>Q</node_name>\n\
			<x_coord> %d " % q_node[0] + "</x_coord>\n\
			<y_coord> %d " % q_node[1] + "</y_coord>\n\
		</node>\n\
		<node>\n\
			<node_id>15</node_id>\n\
			<node_name>D</node_name>\n\
			<x_coord> %d " % d_node[0] + "</x_coord>\n\
			<y_coord> %d " % d_node[1] + "</y_coord>\n\
		</node>\n\
	</nodes>"

sources_sinks = "\n\
	<sources>\n\
		<source>\n\
			<source_id>31</source_id>\n\
			<first_activation_at>0</first_activation_at>\n\
			<located_at_node>11</located_at_node>\n\
			<target_laneset>2111</target_laneset>\n\
			<source_activation_frequency>1</source_activation_frequency>\n\
			<source_type>CONSTANT_PROB</source_type>\n\
			<probability>0</probability>\n\
		</source>\n\
	</sources>\n\
	<sinks>\n\
		<sink>\n\
			<sink_id>41</sink_id>\n\
			<first_activation_at>0</first_activation_at>\n\
			<located_at_node>15</located_at_node>\n\
			<source_laneset>2411</source_laneset>\n\
			<removal_probability>1</removal_probability>\n\
		</sink>\n\
		<sink>\n\
			<sink_id>42</sink_id>\n\
			<first_activation_at>0</first_activation_at>\n\
			<located_at_node>15</located_at_node>\n\
			<source_laneset>2511</source_laneset>\n\
			<removal_probability>1</removal_probability>\n\
		</sink>\n\
	</sinks>"



s1 = head + nodes + sources_sinks





s1 = s1 + "\n\
	<streets>\n\
		<street>\n\
			<street_id>21</street_id>\n\
			<street_name>Entrada</street_name>\n\
			<sections>\n\
				<section>\n\
					<section_id>211</section_id>\n\
					<section_name>section-inutil</section_name>\n\
					<is_preferencial>false</is_preferencial>\n\
					<delimiting_node>11</delimiting_node>\n\
					<delimiting_node>12</delimiting_node>\n\
					<lanesets>\n\
						<laneset>\n\
							<laneset_id>2111</laneset_id>\n\
							<laneset_position>0</laneset_position>\n\
							<start_node>11</start_node>\n\
							<end_node>12</end_node>\n\
							<turning_probabilities>"
if paradox == 0:
	s1 = s1 + "\n\
							<direction>\n\
								<destination_laneset>2211</destination_laneset>\n\
								<probability>50</probability>\n\
							</direction>\n\
							<direction>\n\
								<destination_laneset>2311</destination_laneset>\n\
								<probability>50</probability>\n\
							</direction>"
else:
	s1 = s1 + "\n\
							<direction>\n\
								<destination_laneset>2211</destination_laneset>\n\
								<probability>33</probability>\n\
							</direction>\n\
							<direction>\n\
								<destination_laneset>2311</destination_laneset>\n\
								<probability>66</probability>\n\
							</direction>"
s1 = s1 + "\n\
							</turning_probabilities>\n\
							<lanes>\n"

s1 += cria_lanes(21,1,1,num_lanes[0],vel[0],desal[0])

s1 += "\
							</lanes>\n\
						</laneset>\n\
					</lanesets>\n\
				</section>\n\
			</sections>\n\
		</street>\n\
		<street>\n\
			<street_id>22</street_id>\n\
			<street_name>OP</street_name>\n\
			<sections>\n\
				<section>\n\
					<section_id>221</section_id>\n\
					<section_name>op</section_name>\n\
					<is_preferencial>false</is_preferencial>\n\
					<delimiting_node>12</delimiting_node>\n\
					<delimiting_node>13</delimiting_node>\n\
					<lanesets>\n\
						<laneset>\n\
							<laneset_id>2211</laneset_id>\n\
							<laneset_position>0</laneset_position>\n\
							<start_node>12</start_node>\n\
							<end_node>13</end_node>\n\
							<turning_probabilities>\n\
							<direction>\n\
								<destination_laneset>2411</destination_laneset>\n\
								<probability>100</probability>\n\
							</direction>\n\
							</turning_probabilities>\n\
							<lanes>\n"


s1 += cria_lanes(22,1,1,num_lanes[1],vel[1],desal[1]) 


s1 += "\
                                                         </lanes>\n\
						</laneset>\n\
					</lanesets>\n\
				</section>\n\
			</sections>\n\
		</street>\n\
		<street>\n\
			<street_id>23</street_id>\n\
			<street_name>OQ</street_name>\n\
			<sections>\n\
				<section>\n\
					<section_id>231</section_id>\n\
					<section_name>oq</section_name>\n\
					<is_preferencial>false</is_preferencial>\n\
					<delimiting_node>12</delimiting_node>\n\
					<delimiting_node>14</delimiting_node>\n\
					<lanesets>\n\
						<laneset>\n\
							<laneset_id>2311</laneset_id>\n\
							<laneset_position>0</laneset_position>\n\
							<start_node>12</start_node>\n\
							<end_node>14</end_node>\n\
							<turning_probabilities>"

if paradox == 0:
	s1 = s1 + "\n\
							<direction>\n\
								<destination_laneset>2511</destination_laneset>\n\
								<probability>100</probability>\n\
							</direction>"
else:
	s1 = s1 + "\n\
							<direction>\n\
								<destination_laneset>2511</destination_laneset>\n\
								<probability>50</probability>\n\
							</direction>\n\
							<direction>\n\
								<destination_laneset>2611</destination_laneset>\n\
								<probability>50</probability>\n\
							</direction>"

s1 = s1 + "\n\
							</turning_probabilities>\n\
							<lanes>\n"

s1 += cria_lanes(23,1,1,num_lanes[2],vel[2],desal[2]) 

s1 += "\
							</lanes>\n\
						</laneset>\n\
					</lanesets>\n\
				</section>\n\
			</sections>\n\
		</street>\n\
		<street>\n\
			<street_id>24</street_id>\n\
			<street_name>PD</street_name>\n\
			<sections>\n\
				<section>\n\
					<section_id>241</section_id>\n\
					<section_name>pd</section_name>\n\
					<is_preferencial>false</is_preferencial>\n\
					<delimiting_node>13</delimiting_node>\n\
					<delimiting_node>15</delimiting_node>\n\
					<lanesets>\n\
						<laneset>\n\
							<laneset_id>2411</laneset_id>\n\
							<laneset_position>0</laneset_position>\n\
							<start_node>13</start_node>\n\
							<end_node>15</end_node>\n\
							<turning_probabilities>\n\
							<direction>\n\
								<destination_laneset>2111</destination_laneset>\n\
								<probability>100</probability>\n\
							</direction>\n\
							</turning_probabilities>\n\
							<lanes>\n"

s1 += cria_lanes(24,1,1,num_lanes[3],vel[3],desal[3])

s1 += "\
							</lanes>\n\
						</laneset>\n\
					</lanesets>\n\
				</section>\n\
			</sections>\n\
		</street>\n\
		<street>\n\
			<street_id>25</street_id>\n\
			<street_name>QD</street_name>\n\
			<sections>\n\
				<section>\n\
					<section_id>251</section_id>\n\
					<section_name>qd</section_name>\n\
					<is_preferencial>false</is_preferencial>\n\
					<delimiting_node>14</delimiting_node>\n\
					<delimiting_node>15</delimiting_node>\n\
					<lanesets>\n\
						<laneset>\n\
							<laneset_id>2511</laneset_id>\n\
							<laneset_position>0</laneset_position>\n\
							<start_node>14</start_node>\n\
							<end_node>15</end_node>\n\
							<turning_probabilities>\n\
							<direction>\n\
								<destination_laneset>2111</destination_laneset>\n\
								<probability>100</probability>\n\
							</direction>\n\
							</turning_probabilities>\n\
							<lanes>\n"

s1 += cria_lanes(25,1,1,num_lanes[4],vel[4],desal[4])

s1 += "\
							</lanes>\n\
						</laneset>\n\
					</lanesets>\n\
				</section>\n\
			</sections>\n\
		</street>"

if paradox == 1:
	s1 = s1 + "\n\
		<street>\n\
			<street_id>26</street_id>\n\
			<street_name>QP</street_name>\n\
			<sections>\n\
				<section>\n\
					<section_id>261</section_id>\n\
					<section_name>qp</section_name>\n\
					<is_preferencial>false</is_preferencial>\n\
					<delimiting_node>14</delimiting_node>\n\
					<delimiting_node>13</delimiting_node>\n\
					<lanesets>\n\
						<laneset>\n\
							<laneset_id>2611</laneset_id>\n\
							<laneset_position>0</laneset_position>\n\
							<start_node>14</start_node>\n\
							<end_node>13</end_node>\n\
							<turning_probabilities>\n\
							<direction>\n\
								<destination_laneset>2411</destination_laneset>\n\
								<probability>100</probability>\n\
							</direction>\n\
							</turning_probabilities>\n\
							<lanes>\n"

s1 += cria_lanes(26,1,1,num_lanes[5],vel[5],desal[5])

s1 += "\
							</lanes>\n\
						</laneset>\n\
					</lanesets>\n\
				</section>\n\
			</sections>\n\
		</street>\n\
	</streets>\n\
	<traffic_lights>\n\
	</traffic_lights>\n\
</simulation>\n\
"


print s1

