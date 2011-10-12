#!/usr/bin/python

from xml.dom import minidom
from utils import *
import sys

def conditional_print(*what):
    if 0:
        for i in what:
            print i,
        print

def xml_out(file, level, *what):
    file.write("\t"*level)
    for i in what:
        file.write(i)
    file.write("\n")

def xml_tag(object, tag_name):
    escaped_str = str(object[tag_name]).replace('<', '&lt;').replace('>', '&gt;')
    return '<%s> %s </%s>' % (tag_name, escaped_str, tag_name)


def get_elements_by_name(tag_name, node):
    """Searches in 'node' for the element(s) whose name is 'tag_name'"""
    elements = [i for i in node if i.tagName == tag_name]
    if len(elements) == 0:
        return None
    if len(elements) == 1:
        return elements[0]
    return elements
    
def get_properties(node):
    """Returns all properties of 'node', ie., all elements under it in the XML tree"""
    return [i for i in node.childNodes if i.nodeType == i.ELEMENT_NODE]

def get_section(section_name, node):
    """Returns all elements of the specified section"""
    return get_properties(get_elements_by_name(section_name, node))

def get_value(tag_name, node, type):
    """Returns the value associated with the specified tag and coerces it to 'type'"""
    els = get_elements_by_name(tag_name, node)
    if els == None:
        return None
    if type == str:
        tmp = els.childNodes[0].data.encode('latin-1')
        tmp = tmp.strip()
        return tmp
    else:
        return type(els.childNodes[0].data)



# this is a flat version of the hierarquical tree. the function load_xml returns a
# tuple (flat_tree, tree), where flat_tree is a non-nested dictionary of the form
# {id: object}, for all objects that have been instantiated. flat_tree is used
# to speed up object searches. tree, on the contrary, is a hierarquical tree made up
# of nested dictionaries
# ex: {laneset_id: 10, lanes:{1: lane_1_object, 2:lane_2_object}}
flat_tree = {}

#-----------------------------------------------------------------------------------------

def parse_settings(ident, settings):
    conditional_print()
    cell_size = get_value('cell_size_in_meters', settings, float)
    iteration_length = get_value('iteration_length_in_seconds', settings, int)
    max_speed = get_value('cars_maximum_speed', settings, int)

#     tlight_freq = get_value('trafficlight_agent_observation_frequency', settings, int)
#     sensor_freq = get_value('sensor_activation_frequency', settings, int)

#     all_settings = { 'cell_size_in_meters': cell_size, 'iteration_length_in_seconds': iteration_length,
#                      'cars_maximum_speed': max_speed, 'trafficlight_agent_observation_frequency': tlight_freq,
#                      'sensor_activation_frequency': sensor_freq}

    all_settings = { 'cell_size_in_meters': cell_size, 'iteration_length_in_seconds': iteration_length,
                     'cars_maximum_speed': max_speed }

    conditional_print("\t"*ident, "cell_size:", cell_size)
    conditional_print("\t"*ident, "iteration_length:", iteration_length)
    conditional_print("\t"*ident, "max_speed:", max_speed)

#     conditional_print("\t"*ident, "tlight_freq:", tlight_freq)
#     conditional_print("\t"*ident, "sensor_freq:", sensor_freq)

    return all_settings


def parse_nodes(ident, nodes):
    all_nodes = {}
    for i in nodes:
        conditional_print()
        n = get_properties(i)
        node_id = get_value('node_id', n, int)
        node_name = get_value('node_name', n, str)
        x_coord = get_value('x_coord', n, float)
        y_coord = get_value('y_coord', n, float)

        node = {'node_id': node_id, 'node_name': node_name, 'x_coord': x_coord, 'y_coord': y_coord }
        all_nodes[node_id] = node
        flat_tree[node_id] = node
        
        conditional_print("\t"*ident, "node_id:", node_id)
        conditional_print("\t"*ident, "node_name:", node_name)
        conditional_print("\t"*ident, "x_coord:", x_coord)
        conditional_print("\t"*ident, "y_coord:", y_coord)
        
    return all_nodes
        

def parse_sources(ident, sources):
    all_sources = {}
    for i in sources:
        conditional_print()
        s = get_properties(i)
        source_id = get_value('source_id', s, int)
        first_activation = get_value('first_activation_at', s, int)
        located_at_node = get_value('located_at_node', s, int)
        target_laneset = get_value('target_laneset', s, int)
        source_freq = get_value('source_activation_frequency', s, int)
        source_type = get_value('source_type', s, str)

        if source_type == 'CONSTANT_PROB':
            param = get_value('probability', s, float)
        elif source_type == 'CONSTANT_FLOW':
            param = get_value('flow', s, int)
        elif source_type == 'VARIABLE_FLOW':
            flows = get_section('flows', s)
            param = []
            for var_flow in flows:
                flow_properties = get_properties(var_flow)
                number_of_iterations = get_value('number_of_iterations', flow_properties, int)
                flow = get_value('flow', flow_properties, int)
                param.append([number_of_iterations, flow])
        elif source_type == 'VARIABLE_PROB':
            probs = get_section('probabilities', s)
            param = []
            for var_prob in probs:
                prob_properties = get_properties(var_prob)
                number_of_iterations = get_value('number_of_iterations', prob_properties, int)
                prob = get_value('probability', prob_properties, float)
                param.append([number_of_iterations, prob])
        else:
            print "Oops. Reading xml but source", get_value('source_id', s, int), "has invalid type=", source_type
            
        source = {'source_id': source_id, 'first_activation_at': first_activation,
                  'located_at_node': located_at_node, 'target_laneset': target_laneset,
                  'source_activation_frequency': source_freq, 'source_type': source_type,
                  'param': param}
        
        all_sources[source_id] = source
        flat_tree[source_id] = source
        
        conditional_print("\t"*ident, "source_id:", source_id)
        conditional_print("\t"*ident, "first_activation:", first_activation)
        conditional_print("\t"*ident, "located_at:", located_at_node)
        conditional_print("\t"*ident, "target_laneset:", target_laneset)
        conditional_print("\t"*ident, "source_freq:", source_freq)
        conditional_print("\t"*ident, "source_type:", source_type)
        conditional_print("\t"*(ident+1), "param:", param)
                          
    return all_sources
        
def parse_sinks(ident, sinks):
    all_sinks = {}
    for i in sinks:
        conditional_print()
        s = get_properties(i)
        sink_id = get_value('sink_id', s, int)
        first_activation = get_value('first_activation_at', s, int)
        located_at_node = get_value('located_at_node', s, int)
        source_laneset = get_value('source_laneset', s, int)
        prob = get_value('removal_probability', s, float)

        sink = {'sink_id': sink_id, 'first_activation_at': first_activation,
                'located_at_node': located_at_node,
                'source_laneset': source_laneset, 'removal_probability': prob }
        all_sinks[sink_id] = sink
        flat_tree[sink_id] = sink
        
        conditional_print("\t"*ident, "sink_id:", sink_id)
        conditional_print("\t"*ident, "first_activation:", first_activation)
        conditional_print("\t"*ident, "located_at:", located_at_node)
        conditional_print("\t"*ident, "source_laneset:", source_laneset)
        conditional_print("\t"*ident, "removal_prob:", prob)
                          
    return all_sinks

def parse_streets(ident, streets):
    all_streets = {}
    for i in streets:
        conditional_print()
        s = get_properties(i)
        street_id = get_value('street_id', s, int)
        street_name = get_value('street_name', s, str)
        sections = get_section('sections', s)
        conditional_print("\t"*ident, "street_id:", street_id)
        conditional_print("\t"*ident, "street_name:", street_name)
        st_sections = parse_sections(ident+1, sections)

        street = {'street_id': street_id, 'street_name': street_name, 'sections': st_sections}
        all_streets[street_id] = street
        flat_tree[street_id] = street
        
    return all_streets


def parse_sections(ident, sections):
    all_sections = {}
    for i in sections:
        conditional_print()
        s = get_properties(i)
        section_id = get_value('section_id', s, int)
        section_name = get_value('section_name', s, str)
        preferencial = get_value('is_preferencial', s, str)

        delimiting_nodes = get_elements_by_name('delimiting_node', s)
        n1 = int(delimiting_nodes[0].childNodes[0].data)
        n2 = int(delimiting_nodes[1].childNodes[0].data)
        
        lanesets = get_section('lanesets', s)
        conditional_print("\t"*ident, "section_id:", section_id)
        conditional_print("\t"*ident, "section_name:", section_name)
        conditional_print("\t"*ident, "is_preferencial:", preferencial)
        conditional_print("\t"*ident, "nodes:", n1, ",", n2)
        sec_lanesets = parse_lanesets(ident+1, lanesets)

        section = {'section_id': section_id, 'section_name': section_name, 'is_preferencial': preferencial,
                   'delimiting_node_1': n1, 'delimiting_node_2': n2, 'lanesets': sec_lanesets}
        all_sections[section_id] = section
        flat_tree[section_id] = section
        
    return all_sections
        
def parse_lanesets(ident, lanesets):
    all_lanesets = {}
    for i in lanesets:
        conditional_print()
        l = get_properties(i)
        laneset_id = get_value('laneset_id', l, int)
        laneset_pos = get_value('laneset_position', l, int)
        start_node = get_value('start_node', l, int)
        end_node = get_value('end_node', l, int)
        conditional_print("\t"*ident, "laneset_id:", laneset_id)
        conditional_print("\t"*ident, "laneset_pos:", laneset_pos)
        conditional_print("\t"*ident, "start_node:", start_node)
        conditional_print("\t"*ident, "end_node:", end_node)
        
        turning = get_section('turning_probabilities', l)
        ls_turnings = parse_turning_probs(ident+1, turning)

        lanes = get_section('lanes', l)
        ls_lanes = parse_lanes(ident+1, lanes)

        laneset = {'laneset_id': laneset_id, 'laneset_position': laneset_pos, 'start_node': start_node, 'end_node': end_node,
                   'turning_probabilities': ls_turnings, 'lanes': ls_lanes}
        all_lanesets[laneset_id] = laneset
        flat_tree[laneset_id] = laneset
        
    return all_lanesets

def parse_lanes(ident, lanes):
    all_lanes = {}
    for i in lanes:
        conditional_print()
        l = get_properties(i)
        lane_id = get_value('lane_id', l, int)
        lane_pos = get_value('lane_position', l, int)
        max_speed = get_value('maximum_speed', l, int)
        decel_prob = get_value('deceleration_prob', l, float)
        
        lane = {'lane_id': lane_id, 'lane_position': lane_pos, 'maximum_speed': max_speed, 'deceleration_prob': decel_prob}
        all_lanes[lane_id] = lane
        flat_tree[lane_id] = lane
        
        conditional_print("\t"*ident, "lane_id:", lane_id)
        conditional_print("\t"*ident, "lane_pos:", lane_pos)
        conditional_print("\t"*ident, "max_speed:", max_speed)
        conditional_print("\t"*ident, "decel_prob:", decel_prob)
    return all_lanes


def parse_turning_probs(ident, turnings):
    all_turnings = {}
    for i in turnings:
        conditional_print()
        d = get_properties(i)
        laneset = get_value('destination_laneset', d, int)
        prob = get_value('probability', d, float)
        all_turnings[laneset] = prob
        
        conditional_print("\t"*ident, "destination_laneset:", laneset)
        conditional_print("\t"*ident, "probability:", prob)

    return all_turnings
        


def parse_traffic_lights(ident, tlights):
    all_tlights = {}
    for i in tlights:
        conditional_print()
        t = get_properties(i)
        tlight_id = get_value('traffic_light_id', t, int)
        node = get_value('located_at_node', t, int)
        conditional_print("\t"*ident, "tlight_id:", tlight_id)
        conditional_print("\t"*ident, "located_at_node:", node)

        signalplans = get_section('signalplans', t)
        tl_signalplans = parse_signalplans(ident+1, signalplans)

        tlight = {'traffic_light_id': tlight_id, 'located_at_node': node, 'signalplans': tl_signalplans}
        all_tlights[tlight_id] = tlight
        flat_tree[tlight_id] = tlight
        
    return all_tlights

def parse_signalplans(ident, signalplans):
    all_signalplans = {}
    for i in signalplans:
        conditional_print()
        s = get_properties(i)
        splan_id = get_value('signalplan_id', s, int)
        conditional_print("\t"*ident, "signalplan_id:", splan_id)
        
        additional_info = get_value('additional_info', s, str)
        if additional_info == None:
            additional_info = ""

        phases = get_section('phases', s)
        sp_phases = parse_phases(ident+1, phases)

        signalplan  = {'signalplan_id': splan_id, 'phases': sp_phases, 'additional_info': additional_info}
        all_signalplans[splan_id] = signalplan

        
    return all_signalplans

def parse_phases(ident, phases):
    all_phases = {}
    for i in phases:
        conditional_print()
        p = get_properties(i)
        phase_id = get_value('phase_id', p, int)
        iter_start = get_value('iteration_start', p, int)
        iter_end = get_value('iteration_end', p, int)
        conditional_print("\t"*ident, "phase_id:", phase_id)
        conditional_print("\t"*ident, "iter_start:", iter_start)
        conditional_print("\t"*ident, "iter_end:", iter_end)

        directions = get_section('directions', p)
        ph_directions = []
        for j in directions:
            conditional_print()
            d = get_properties(j)

            from_lane = get_value('from_lane', d, int)
            to_laneset = get_value('to_laneset', d, int)
            ph_directions.append([from_lane, to_laneset])
            conditional_print("\t"*(ident+1), "from_lane:", from_lane)
            conditional_print("\t"*(ident+1), "to_laneset:", to_laneset)
        phase = {'phase_id': phase_id, 'iteration_start': iter_start, 'iteration_end': iter_end, 'directions': ph_directions}
        all_phases[phase_id] = phase
    return all_phases
            
        

def load_xml(filename):
	
    flat_tree.clear()

    xml = minidom.parse(str(filename))
    simulation = [i for i in xml.getElementsByTagName('simulation')[0].childNodes if i.nodeType == i.ELEMENT_NODE]
    
    network_id = get_value('network_id', simulation, int)
    network_name = get_value('network_name', simulation, str)
    
    # TODO: conferir
#     xml_settings = get_section('settings', simulation)
#     current_settings = parse_settings(1, xml_settings)
    
    xml_nodes = get_section('nodes', simulation)
    current_nodes = parse_nodes(2, xml_nodes)
    
    xml_sources = get_section('sources', simulation)
    current_sources = parse_sources(2, xml_sources)
    
    xml_sinks = get_section('sinks', simulation)
    current_sinks = parse_sinks(2, xml_sinks)
    
    xml_streets = get_section('streets', simulation)
    current_streets = parse_streets(2, xml_streets)
    
    xml_tlights = get_section('traffic_lights', simulation)
    current_tlights = parse_traffic_lights(2, xml_tlights)
    
    current_network = { 'network_id': network_id, 'network_name': network_name,
                        'nodes': current_nodes,
                        'sources': current_sources, 'sinks': current_sinks,
                        'streets': current_streets, 'traffic_lights': current_tlights}

    # TODO: conferir acima
#     current_network = { 'network_id': network_id, 'network_name': network_name,
#                         'settings': current_settings, 'nodes': current_nodes,
#                         'sources': current_sources, 'sinks': current_sinks,
#                         'streets': current_streets, 'traffic_lights': current_tlights}


    tree = {network_id: current_network}
    flat_tree[network_id] = current_network
    
#     import pprint
#     printer = pprint.PrettyPrinter()
    # printer.pprint(tree)

    return (flat_tree, tree)


def check_if_turnings_have_phase(flat_tree, laneset, turnings, tlights):

    from copy import copy

    laneset_id = laneset['laneset_id']
    end_node = flat_tree[laneset['end_node']]

    needed_directions = []
    origin_lanes = laneset['lanes'].keys()
    for from_lane in origin_lanes:
        for dest_laneset in turnings.keys():
            if turnings[dest_laneset] > 0.0:
                # the tlight must have directions to all turnings with probability > 0
                needed_directions.append([from_lane, dest_laneset])

    
    for i in tlights.values():
        if i['located_at_node'] == end_node['node_id']:
            for signalplan in i['signalplans'].values():
                # this signalplan must have all needed_directions

                needed_directions_tmp = copy(needed_directions)
                for phase in signalplan['phases'].values():
                    for direction in phase['directions']:
                        if direction in needed_directions_tmp:
                            needed_directions_tmp.remove(direction)
                            
                if len(needed_directions_tmp) > 0:
                    error = "There is a turning from <font color=red>laneset %d to %d</font>"
                    error += " but there is no phase allowing this turn in <font color=red>signalplan %d"
                    error += " of trafficlight %d</font>. The file will be saved but simulation errors"
                    error += " WILL occur :-)"
                    return (1,  error  % (laneset_id,
                                          needed_directions_tmp[0][1],
                                          signalplan['signalplan_id'],
                                          i['traffic_light_id']))
    return (0, None)
    

def dump_xml(editor, file):

    flat_tree = editor.flat_tree
    tree = editor.tree

    fp = open(str(file), "w")

    cur_level = 0

    network_id = tree.keys()[0]
    objects = tree[network_id]
    
    xml_out(fp, cur_level, '<?xml version="1.0" encoding="ISO-8859-1"?>')
    xml_out(fp, cur_level, '<simulation>')

    cur_level += 1

    xml_out(fp, cur_level, '<network_id> %s </network_id>' % network_id)
    xml_out(fp, cur_level, xml_tag(objects, 'network_name'))

    # TODO: conferir
    #--------------------- settings
#     xml_out(fp, cur_level, '<settings>')
#     settings = objects['settings']
#     cur_level += 1
#     xml_out(fp, cur_level, xml_tag(settings, 'cell_size_in_meters'))
#     xml_out(fp, cur_level, xml_tag(settings, 'iteration_length_in_seconds'))
# #     xml_out(fp, cur_level, xml_tag(settings, 'sensor_activation_frequency'))
# #     xml_out(fp, cur_level, xml_tag(settings, 'trafficlight_agent_observation_frequency'))
#     xml_out(fp, cur_level, xml_tag(settings, 'cars_maximum_speed'))
#     cur_level -= 1
#     xml_out(fp, cur_level, '</settings>')

    #--------------------- nodes
    xml_out(fp, cur_level, '<nodes>')
    nodes = objects['nodes']
    cur_level += 1
    for i in nodes.values():
        xml_out(fp, cur_level, '<node>')
        cur_level += 1
        xml_out(fp, cur_level, xml_tag(i, 'node_id'))
        xml_out(fp, cur_level, xml_tag(i, 'node_name'))
        xml_out(fp, cur_level, xml_tag(i, 'x_coord'))
        xml_out(fp, cur_level, xml_tag(i, 'y_coord'))
        cur_level -= 1
        xml_out(fp, cur_level, '</node>')
    cur_level -= 1
    xml_out(fp, cur_level, '</nodes>')


    # TODO: deixar o cara modificar o tipo do source durante a edicao
    #       botoes pra adicionar/remover linhas da tabela

    #--------------------- sources
    xml_out(fp, cur_level, '<sources>')
    sources = objects['sources']
    cur_level += 1
    for i in sources.values():
        xml_out(fp, cur_level, '<source>')
        cur_level += 1
        xml_out(fp, cur_level, xml_tag(i, 'source_id'))
        xml_out(fp, cur_level, xml_tag(i, 'first_activation_at'))
        xml_out(fp, cur_level, xml_tag(i, 'located_at_node'))
        xml_out(fp, cur_level, xml_tag(i, 'target_laneset'))
        xml_out(fp, cur_level, xml_tag(i, 'source_activation_frequency'))
        source_type = i['source_type']
        xml_out(fp, cur_level, '<source_type> %s </source_type>' % source_type)
        if source_type == 'CONSTANT_PROB':
            xml_out(fp, cur_level+1, '<probability> %s </probability>' % i['param'])
        elif source_type == 'CONSTANT_FLOW':
            xml_out(fp, cur_level+1, '<flow> %s </flow>' % i['param'])
        elif source_type == 'VARIABLE_PROB':
            xml_out(fp, cur_level, '<probabilities>')
            cur_level += 1
            for (number_of_iter, prob) in i['param']:
                xml_out(fp, cur_level, '<var_prob>')
                xml_out(fp, cur_level+1, '<number_of_iterations> %s </number_of_iterations>' % str(number_of_iter))
                xml_out(fp, cur_level+1, '<probability> %s </probability>' % str(prob))
                xml_out(fp, cur_level, '</var_prob>')
            cur_level -= 1
            xml_out(fp, cur_level, '</probabilities>')
        elif source_type == 'VARIABLE_FLOW':
            xml_out(fp, cur_level, '<flows>')
            cur_level += 1
            for (number_of_iter, flow) in i['param']:
                xml_out(fp, cur_level, '<var_flow>')
                xml_out(fp, cur_level+1, '<number_of_iterations> %s </number_of_iterations>' % str(number_of_iter))
                xml_out(fp, cur_level+1, '<flow> %s </flow>' % str(flow))
                xml_out(fp, cur_level, '</var_flow>')
            cur_level -= 1
            xml_out(fp, cur_level, '</flows>')
        else:
            print "Oops. Saving xml but source", i['source_id'], "has invalid type=", source_type
        cur_level -= 1
        xml_out(fp, cur_level, '</source>')
    cur_level -= 1
    xml_out(fp, cur_level, '</sources>')


    #--------------------- sinks
    xml_out(fp, cur_level, '<sinks>')
    sinks = objects['sinks']
    cur_level += 1
    for i in sinks.values():
        xml_out(fp, cur_level, '<sink>')
        cur_level += 1
        xml_out(fp, cur_level, xml_tag(i, 'sink_id'))
        xml_out(fp, cur_level, xml_tag(i, 'first_activation_at'))
        xml_out(fp, cur_level, xml_tag(i, 'located_at_node'))
        xml_out(fp, cur_level, xml_tag(i, 'source_laneset'))
        xml_out(fp, cur_level, xml_tag(i, 'removal_probability'))
        cur_level -= 1
        xml_out(fp, cur_level, '</sink>')
    cur_level -= 1
    xml_out(fp, cur_level, '</sinks>')

    
    #--------------------- traffic lights
    xml_out(fp, cur_level, '<traffic_lights>')
    tlights = objects['traffic_lights']
    cur_level += 1
    for i in tlights.values():
        xml_out(fp, cur_level, '<traffic_light>')
        cur_level += 1
        xml_out(fp, cur_level, xml_tag(i, 'traffic_light_id'))
        xml_out(fp, cur_level, xml_tag(i, 'located_at_node'))
        xml_out(fp, cur_level, '<signalplans>')
        cur_level += 1
        for j in i['signalplans'].values():
            xml_out(fp, cur_level, '<signalplan>')
            cur_level += 1
            xml_out(fp, cur_level, xml_tag(j, 'signalplan_id'))
            xml_out(fp, cur_level, xml_tag(j, 'additional_info'))
            
            xml_out(fp, cur_level, '<phases>')
            cur_level += 1
            for k in j['phases'].values():
                xml_out(fp, cur_level, '<phase>')
                cur_level += 1
                xml_out(fp, cur_level, xml_tag(k, 'phase_id'))
                xml_out(fp, cur_level, xml_tag(k, 'iteration_start'))
                xml_out(fp, cur_level, xml_tag(k, 'iteration_end'))
                xml_out(fp, cur_level, '<directions>')
                cur_level += 1
                for l in k['directions']:
                    xml_out(fp, cur_level, '<direction>')
                    xml_out(fp, cur_level+1, '<from_lane> %s </from_lane>' % l[0])
                    xml_out(fp, cur_level+1, '<to_laneset> %s </to_laneset>' % l[1])
                    xml_out(fp, cur_level, '</direction>')
                cur_level -= 1
                xml_out(fp, cur_level, '</directions>')
                cur_level -= 1
                xml_out(fp, cur_level, '</phase>')
            cur_level -= 1
            xml_out(fp, cur_level, '</phases>')
            cur_level -= 1
            xml_out(fp, cur_level, '</signalplan>')
        cur_level -= 1
        xml_out(fp, cur_level, '</signalplans>')
        cur_level -= 1
        xml_out(fp, cur_level, '</traffic_light>')
    cur_level -= 1
    xml_out(fp, cur_level, '</traffic_lights>')
            

    #--------------------- streets
    xml_out(fp, cur_level, '<streets>')
    streets = objects['streets']
    cur_level += 1
    for i in streets.values():
        xml_out(fp, cur_level, '<street>')
        cur_level += 1
        xml_out(fp, cur_level, xml_tag(i, 'street_id'))
        xml_out(fp, cur_level, xml_tag(i, 'street_name'))
        xml_out(fp, cur_level, '<sections>')
        cur_level += 1
        for j in i['sections'].values():
            xml_out(fp, cur_level, '<section>')
            cur_level += 1
            xml_out(fp, cur_level, xml_tag(j, 'section_id'))
            xml_out(fp, cur_level, xml_tag(j, 'section_name'))
            xml_out(fp, cur_level, xml_tag(j, 'is_preferencial'))
            xml_out(fp, cur_level, '<delimiting_node> %s </delimiting_node>' % j['delimiting_node_1'])
            xml_out(fp, cur_level, '<delimiting_node> %s </delimiting_node>' % j['delimiting_node_2'])
            xml_out(fp, cur_level, '<lanesets>')
            cur_level += 1

            for k in j['lanesets'].values():
                xml_out(fp, cur_level, '<laneset>')
                cur_level += 1
                xml_out(fp, cur_level, xml_tag(k, 'laneset_id'))
                xml_out(fp, cur_level, xml_tag(k, 'laneset_position'))
                xml_out(fp, cur_level, xml_tag(k, 'start_node'))
                xml_out(fp, cur_level, xml_tag(k, 'end_node'))
                xml_out(fp, cur_level, '<turning_probabilities>')

                (error, error_msg) = check_if_turnings_have_phase(flat_tree, k,
                                                                  k['turning_probabilities'], tlights)
                if (error):
                    qt.QMessageBox.critical(editor,
                                            "Error", error_msg,
                                            qt.QMessageBox.Ok)


                known_turnings = k['turning_probabilities'].keys()
                outgoing_lanesets = flat_tree[k['end_node']]['__lanesets_out'].keys()
                
                # if there are outgoing lanesets but there is no turning probability, we fill
                # the table with equal probability turnings
                if len(outgoing_lanesets) > 0 and len(known_turnings) == 0:
                    for l in outgoing_lanesets:
                        xml_out(fp, cur_level, '<direction>')
                        xml_out(fp, cur_level+1, '<destination_laneset> %s </destination_laneset>' % l)
                        xml_out(fp, cur_level+1, '<probability> %s </probability>' % (100.0/len(outgoing_lanesets)))
                        xml_out(fp, cur_level, '</direction>')
                else:
                    for (dest_laneset, prob) in k['turning_probabilities'].items():
                        xml_out(fp, cur_level, '<direction>')
                        xml_out(fp, cur_level+1, '<destination_laneset> %s </destination_laneset>' % dest_laneset)
                        xml_out(fp, cur_level+1, '<probability> %s </probability>' % prob)
                        xml_out(fp, cur_level, '</direction>')
                xml_out(fp, cur_level, '</turning_probabilities>')

                xml_out(fp, cur_level, '<lanes>')
                cur_level += 1
                for l in k['lanes'].values():
                    xml_out(fp, cur_level, '<lane>')
                    cur_level += 1
                    xml_out(fp, cur_level, xml_tag(l, 'lane_id'))
                    xml_out(fp, cur_level, xml_tag(l, 'lane_position'))
                    xml_out(fp, cur_level, xml_tag(l, 'maximum_speed'))
                    xml_out(fp, cur_level, xml_tag(l, 'deceleration_prob'))
                    cur_level -= 1
                    xml_out(fp, cur_level, '</lane>')
                cur_level -= 1
                xml_out(fp, cur_level, '</lanes>')

                cur_level -= 1
                xml_out(fp, cur_level, '</laneset>')
            cur_level -= 1
            xml_out(fp, cur_level, '</lanesets>')
            cur_level -= 1
            xml_out(fp, cur_level, '</section>')
        cur_level -= 1
        xml_out(fp, cur_level, '</sections>')
        cur_level -= 1
        xml_out(fp, cur_level, '</street>')
    cur_level -= 1
    xml_out(fp, cur_level, '</streets>')            
    
    cur_level -= 1

    xml_out(fp, cur_level, '</simulation>')


        
if __name__ == "__main__":

    xml = minidom.parse(sys.argv[1])
    simulation = [i for i in xml.getElementsByTagName('simulation')[0].childNodes if i.nodeType == i.ELEMENT_NODE]

    network_id = get_value('network_id', simulation, int)
    network_name = get_value('network_name', simulation, str)
    conditional_print("network_id:", network_id)
    conditional_print("network_name:", network_name)

    settings = get_section('settings', simulation)
    teste=parse_settings(1, settings)

    nodes = get_section('nodes', simulation)
    parse_nodes(2, nodes)

    sources = get_section('sources', simulation)
    parse_sources(2, sources)

    sinks = get_section('sinks', simulation)
    parse_sinks(2, sinks)

    streets = get_section('streets', simulation)
    parse_streets(2, streets)

    tlights = get_section('traffic_lights', simulation)
    parse_traffic_lights(2, tlights)
