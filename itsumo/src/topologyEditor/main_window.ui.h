/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/





/*-----------------------------------------------------------------------*/
void editor::build_QT_tree( int form, int topology_elements )
{
"""Creates a QListView with the values of self.tree (the tree of network objects)"""

form = a0
topology_elements = a1

def compare_signalplans_by_id(a, b):
    if a['signalplan_id'] < b['signalplan_id']:
        return -1
    else:
        return 1
    

self.used_ids = []

self.QT_tree.clear()
self.QT_tree.setSorting( -1 )

network_id = topology_elements.keys()[0]
objects = topology_elements[network_id]

self.used_ids.append(network_id)

self.QT_tree.setColumnText(0, "Network %s" % objects['network_name'])

# constructor for tree_item: tree_item(parent_item, name, all_objects, object_reference, object_type)
network = tree_item(self.QT_tree, "network", objects, objects, 'network')
tlights = tree_item(network, "traffic lights", None, None, 'traffic_lights')
sinks = tree_item(network, "sinks", None, None, 'sinks')
sources = tree_item(network, "sources", None, None, 'sources')
streets = tree_item(network, "streets", None, None, 'streets')
nodes = tree_item(network, "nodes", None, None, 'nodes')

# TODO: conferir
# settings = tree_item(self.QT_tree, "settings", objects, objects['settings'], 'settings')
# self.QT_tree.setOpen(network, 1)
# self.QT_tree.setCurrentItem(settings)

for source_id, properties in objects['sources'].items():
	node_name = objects['nodes'][properties['located_at_node']]['node_name']
	new_source = tree_item(sources, "Located in node %s       (id %s)" % (node_name, source_id), objects, properties, 'source')
        self.used_ids.append(source_id)


for sink_id, properties in objects['sinks'].items():
	node_name = objects['nodes'][properties['located_at_node']]['node_name']
	new_sink = tree_item(sinks, "Located in node %s       (id %s)" % (node_name, sink_id), objects, properties, 'sink')
        self.used_ids.append(sink_id)


for node_id, properties in objects['nodes'].items():
	new_node = tree_item(nodes, "Node %s       (id %s)" % (properties['node_name'], node_id), 
			     objects, properties, 'node')
        self.used_ids.append(node_id)


for tlight_id, tlight_properties in objects['traffic_lights'].items():
	node_name = objects['nodes'][tlight_properties['located_at_node']]['node_name']
	new_tlight = tree_item(tlights, "Located in node %s       (id %s)" % (node_name, tlight_id),
			       objects, tlight_properties, 'tlight')
	self.used_ids.append(tlight_id)

        
        signalplans = [i for i in tlight_properties['signalplans'].values()]
        signalplans.sort(compare_signalplans_by_id)
	signalplans.reverse()

        for i in signalplans:
		new_sp = tree_item(new_tlight,
				   "Signalplan %s       (%s phases)" % (i['signalplan_id'], len(i['phases'])),
				   objects, i, 'signalplan')


for street_id, street_properties in objects['streets'].items():


	new_street = tree_item(streets, "%s" % (street_properties['street_name']),
			       objects, street_properties, 'street')
        self.used_ids.append(street_id)


	for section_id, section_properties in street_properties['sections'].items():
		new_section = tree_item(new_street, 
					"Section %s       (id %s)" % (section_properties['section_name'],
								   section_id),
					objects, section_properties, 'section')
		self.used_ids.append(section_id)



		for laneset_id, laneset_properties in section_properties['lanesets'].items():
                        start_node_name = get_element_by_id(self.flat_tree,
							     laneset_properties['start_node'])['node_name']
                        end_node_name = get_element_by_id(self.flat_tree,
							  laneset_properties['end_node'])['node_name']

                        
			laneset_caption = "Laneset (%s) -> (%s)       (id %s)" % (start_node_name,
									     end_node_name, laneset_id)
			new_laneset = tree_item(new_section, laneset_caption, objects,
						laneset_properties, 'laneset')
			self.used_ids.append(laneset_id)

			
			for lane_id, lane_properties in laneset_properties['lanes'].items():
				new_lane = tree_item(new_laneset, "Lane %s" % (lane_id), objects, 
						     lane_properties, 'lane')
				self.used_ids.append(lane_id)

self.used_ids.sort()

sources.sortChildItems(0, 1)
nodes.sortChildItems(0, 1)
sinks.sortChildItems(0, 1)
streets.sortChildItems(0, 1)
tlights.sortChildItems(0, 1)

				
}






/*-----------------------------------------------------------------------*/
void editor::fire_popup()
{
"""Fires a popup depending on which item was clicked"""
from types import NoneType
if type(self.QT_tree.selectedItem()) == NoneType:
	return

popup = getattr(self, self.QT_tree.selectedItem().object_type + "_popup")
if popup != None:
	popup.exec_loop(QCursor.pos())

}






/*-----------------------------------------------------------------------*/
void editor::fileNew()
{

# reads the empty network from the xml and returns a tree
(self.flat_tree, self.tree) = load_xml('empty.xml')
self.current_file = 'empty.xml'

# complements the tree with additional info (ie: the possible nodes for start/end of each laneset,
# the possible target/source lanesets for each source, etc)
self.tree = complement_tree(self.tree, self.flat_tree)

# build the QT ListView using the tree
self.build_QT_tree(self, self.tree)
# updates the form caption
self.setCaption(self.default_caption + "[" + self.current_file + "]")
                 
}




              
/*-----------------------------------------------------------------------*/           
void editor::fileOpen()
{

filename = qt.QFileDialog.getOpenFileName(".", "Topologies (*.xml)", self, "Open file dialog", "Choose a file" );                  


if filename == None:
	return

if (qt.QFile.exists( filename)):

	print "Opening", filename
	# reads the network from the xml and returns a tree
	(self.flat_tree, self.tree) = load_xml(filename)
	self.current_file = str(filename)
	
	# complements the tree with additional info (ie: the possible nodes for start/end of each laneset,
	# the possible target/source lanesets for each source, etc)
	self.tree = complement_tree(self.tree, self.flat_tree)
	
	# build the QT ListView using the tree
	self.build_QT_tree(self, self.tree)

	# re-selects the network tab and guarantees it remains opened and expanded
	self.QT_tree.setSelected(self.QT_tree.firstChild(), 1)
	selected_item = self.QT_tree.selectedItem()
	self.QT_tree.setOpen(selected_item, 1)
	self.QT_tree.ensureItemVisible(selected_item)

	# updates the form caption
	self.setCaption(self.default_caption + "[" + self.current_file + "]")

else:
	qt.QMessageBox.critical(self,
				"File does not exist", "File does not exist!",
				qt.QMessageBox.Ok)
	return

                  
}



              

/*-----------------------------------------------------------------------*/            
void editor::fileSave()
{

if self.current_file == 'empty.xml':
	self.saving_dialog_open = 1
	self.fileSaveAs()
	self.saving_dialog_open = 0

if self.current_file == 'empty.xml':
	return

dump_xml(self, self.current_file)

# updates the form caption
self.setCaption(self.default_caption + "[" + self.current_file + "]")

print "Saving", self.current_file

                 
}
              





/*-----------------------------------------------------------------------*/    
void editor::fileSaveAs()
{

filename = qt.QFileDialog.getSaveFileName(".", "Topologies (*.xml)", self, "Save file dialog", "Choose a file" );
filename = filename + ".xml"
if (qt.QFile.exists( filename)) and (qt.QMessageBox.question( self, "Save File",
							      "%s already exists. Do you want to override it?" % filename,
							      "&Yes", "&No") == 1 ):
	return

if filename == None:
	return

self.current_file = str(filename)
# updates the form caption
self.setCaption(self.default_caption + "[" + self.current_file + "]")

if not self.saving_dialog_open:
	self.fileSave()


}
              


              

/*-----------------------------------------------------------------------*/             
void editor::fileExit()
{

pass

}


              
         
/*-----------------------------------------------------------------------*/              
void editor::helpAbout()
{

qt.QMessageBox.critical(self, "About",
			"ITSUMO network editor by Bruno (brunocs@gmail.com)\n" + \
			"For more details of the project, please contact bazzan@inf.ufrgs.br",
			qt.QMessageBox.Ok)

                  
}
              




/*-----------------------------------------------------------------------*/              
void editor::QT_tree_selection()
{
"""When the user selects an item on the QT tree, this function
   puts the properties of the select item on the correct widgets"""

# during this function, the tree SHOULD NOT be updated because the value of the widgets is not
# yet filled! (trust me)
self.do_not_update = 1

from types import NoneType
widgets = {'source': 0, 'sink': 1, 'settings': 2, 'street': 3, 'laneset': 4, 'lane': 5, 'section': 6,
           'tlight': 7, 'node': 8, 'network': 10, 'streets': 11, 'nodes': 12, 'sources': 13, 'sinks': 14,
	   'traffic_lights': 15, 'signalplan': 16}

selected_item = self.QT_tree.selectedItem()

# if the users clicks the "+" button to expand the list, the selected object is NoneType. If so, do nothing
if type(selected_item) == NoneType:
	return

# raise the frame containing the properties for the specified object 
# ('lane edit' frame, 'section edit' frame, etc)
self.stack.raiseWidget(widgets[str(selected_item.object_type)])

if selected_item.object_type == 'signalplan':

    # since the signal plan requires a complex routine to show its attributes
    # (eg: drawing the plans, directions, etc), we'll use a different function
    parent_trafficlight = self.QT_tree.selectedItem().parent().object_reference
    drawing_frame = self.stack.visibleWidget().child("drawing_frame")
    
    fill_signalplan(self.stack.visibleWidget(), self.tree, self.flat_tree,
                    parent_trafficlight, selected_item.object_reference, drawing_frame)


elif selected_item.object_type == 'source':
    fill_source(self, self.stack.visibleWidget().child('source'),
                selected_item.object_reference)

else:

    # all other objects are more or less the same, so we can use these generic
    # calls for filling automatically the widgets, giving the name of the attribute
    if selected_item.object_type not in ('streets', 'nodes', 'sources', 'sinks', 'traffic_lights'):
        for widget in self.stack.visibleWidget().child(selected_item.object_type).children():
            widget_name = widget.name()[widget.name().find('_')+1:]
            if selected_item.object_reference.has_key(widget_name):
                fill_widget(self, widget, 
                            selected_item.object_reference[widget_name], 
                            selected_item.object_reference)

# ok, now the tree can be updated if there is any change in the value of a widget
self.do_not_update = 0

}






/*-----------------------------------------------------------------------*/
void editor::print_error_and_rollback(int error_message, int valid_tree)
{
"""If an error was found, print some message and rollback to a valid state"""
error_message = a0
valid_tree = a1

qt.QMessageBox.critical(self,
                        "Inconsistent topology", error_message,
                        qt.QMessageBox.Ok)
# rollsback in case update_tree_and_QT_tree failed
# print "rolling back!"

# returns to the previous state of the tree and restores the original object_reference of the widget
self.tree = copy.deepcopy(valid_tree)
# selected_item.object_reference = object_copy

# re-selects the widget and re-fills the widgets with the original values
self.QT_tree_selection()

}





/*-----------------------------------------------------------------------*/
void editor::update_tree()
{
"""This function is called when it is necessary to update the tree of objects
   to reflect an update in the value of some widget"""

if self.do_not_update:
    return

selected_item = self.QT_tree.selectedItem()

object_ref = selected_item.object_reference
object_type = selected_item.object_type

# makes a copy of the tree, in case the upcoming updates fail!
valid_tree = copy.deepcopy(self.tree)

widget_name = self.sender().name()[self.sender().name().find('_')+1:]
if object_ref.has_key(widget_name) or widget_name == "source_variable":
    print "updating --", widget_name
    (error, error_message) = update_tree_from_widget(self.sender(), widget_name, object_ref, self.tree)
    if error == 1:
        self.print_error_and_rollback(error_message, valid_tree)
        return


self.update_tree_and_QT_tree({'object_reference': object_ref,
                              'object_type': object_type})

    
}




/*-----------------------------------------------------------------------*/
void editor::add_lane_popup()
{
"""function that will be called by the popup menus. All it
does is to call 'add_lane' with a lot of None parameters"""

self.add_lane(None, None)
}





/*-----------------------------------------------------------------------*/
// note: how to add slots with only one parameter??  DO NOT specify it as "void". God knows why. 
// Otherwise fucking pyuic won't translate it correctly to python. I'm using "int" for everything and
// it works
void editor::add_lane( int parent_laneset, int lanes_max_speed )
{
"""Adds a lane"""
parent_laneset = a0
lanes_max_speed = a1

must_update = 0

if parent_laneset == None:
	must_update = 1
	parent_laneset = self.QT_tree.selectedItem().object_reference

max_used_position = 0
max_speed_for_other_lanes = -1
for other_lane in parent_laneset['lanes'].values():
    if other_lane['lane_position'] > max_used_position:
        max_used_position = int(other_lane['lane_position'])
        max_speed_for_other_lanes = int(other_lane['maximum_speed'])

if lanes_max_speed == None:
	network_id = self.tree.keys()[0]
	objects = self.tree[network_id]

	# TODO: deixar de alguma maneira mais decente
	lanes_max_speed = 3
	    
# 	lanes_max_speed = objects['settings']['cars_maximum_speed']
	if max_speed_for_other_lanes != -1:
		lanes_max_speed = max_speed_for_other_lanes
        
        
new_id = self.get_free_id()

new_lane = {'lane_id': new_id, 'maximum_speed': lanes_max_speed, 'deceleration_prob': 0,
                    'lane_position': max_used_position+1}

parent_laneset['lanes'][new_id] = new_lane
self.flat_tree[new_id] = new_lane

if must_update:
	self.update_tree_and_QT_tree({'object_reference': new_lane, 'object_type': 'lane'})
}




/*-----------------------------------------------------------------------*/
void editor::add_laneset_popup()
{
"""function that will be called by the popup menus. All it
does is to call 'add_laneset' with a lot of None parameters"""

self.add_laneset(None, None, None, None, None)
}




/*-----------------------------------------------------------------------*/
void editor::add_laneset( int parent_section, int star, int end, int nlanes, int lanes_max_speed )
{
"""Adds a laneset"""
parent_section = a0
start = a1
end = a2
nlanes = a3
lanes_max_speed = a4

must_update = 0

if parent_section == None:
	must_update = 1
	parent_section = self.QT_tree.selectedItem().object_reference
if start == None:
	start = -1
if end == None:
	end = -1
if nlanes == None:
    nlanes = 1

max_used_position = 0
for other_laneset in parent_section['lanesets'].values():
    if other_laneset['laneset_position'] > max_used_position:
        max_used_position = int(other_laneset['laneset_position'])
        
new_id = self.get_free_id()

new_laneset = {'laneset_id': new_id, 'laneset_position': max_used_position+1, 'lanes': {},
	       'turning_probabilities': {}, 'start_node': start, 'end_node': end, '__possible_nodes': [] }

for i in range(nlanes):
	self.add_lane(new_laneset, lanes_max_speed)

parent_section['lanesets'][new_id] = new_laneset
self.flat_tree[new_id] = new_laneset

if must_update:
	self.update_tree_and_QT_tree({'object_reference': new_laneset, 'object_type': 'laneset'})


}




/*-----------------------------------------------------------------------*/
void editor::add_section_popup()
{
"""function that will be called by the popup menus. All it
does is to call 'add_section' with a lot of None parameters"""

self.add_section(None, None, None, None, None, None)
}




/*-----------------------------------------------------------------------*/
void editor::add_section( int parent_street, int n1, int n2, int nways, int nlanes, int lanes_max_speed )
{
"""Adds a section"""
parent_street = a0
n1 = a1
n2 = a2
nways = a3
nlanes = a4
lanes_max_speed = a5

must_update = 0

if parent_street == None:
	must_update = 1
	parent_street = self.QT_tree.selectedItem().object_reference
if nways == None:
	nways = 'one_way'
if nlanes == None:
	nlanes = 1
if n1 == None:
	n1 = -1
if n2 == None:
	n2 = -1


new_id = self.get_free_id()

new_section = {'section_id': new_id, 'section_name': '', 'lanesets': {}, 'is_preferencial': 'false',
               'delimiting_node_1': n1, 'delimiting_node_2': n2}

if nways == 'one_way':
	self.add_laneset(new_section, n1, n2, nlanes, lanes_max_speed)
else:
	self.add_laneset(new_section, n1, n2, nlanes, lanes_max_speed)
	self.add_laneset(new_section, n2, n1, nlanes, lanes_max_speed)

parent_street['sections'][new_id] = new_section
self.flat_tree[new_id] = new_section

if must_update:
	self.update_tree_and_QT_tree({'object_reference': new_section, 'object_type': 'section'})

}



/*-----------------------------------------------------------------------*/
void editor::add_var_source_rule()
{

source_var_table = self.sender().parent().child('source_source_variable')
cur_row = source_var_table.currentRow()

if cur_row == -1:
	cur_row = int(source_var_table.numRows())

source_var_table.insertRows(cur_row)
source_var_table.setText(cur_row, 0, str("1"))
source_var_table.setText(cur_row, 1, str("1"))
source_var_table.selectRow(cur_row)

update_tree_from_widget(source_var_table, 'source_variable',
			self.QT_tree.selectedItem().object_reference, self.tree)

}



/*-----------------------------------------------------------------------*/
void editor::delete_var_source_rule()
{

source_var_table = self.sender().parent().child('source_source_variable')
cur_row = source_var_table.currentRow()

if cur_row == -1:
	return

source_var_table.removeRow(cur_row)

update_tree_from_widget(source_var_table, 'source_variable',
			self.QT_tree.selectedItem().object_reference, self.tree)

}



/*-----------------------------------------------------------------------*/
void editor::add_street()
{
"""Adds a street"""

parent_item = self.QT_tree.selectedItem()

network_id = self.tree.keys()[0]
parent_network = self.tree[network_id]

selected_nodes = self.stack.child("selected_nodes")

if selected_nodes.count() < 2:
	qt.QMessageBox.critical(self,
				"Insuficient number of nodes!",
				"The street must have at least two nodes!",
				qt.QMessageBox.Ok)
	return


new_id = self.get_free_id()

suggested_name = str(self.stack.child("street_name").text())
if suggested_name.strip() == "":
	suggested_name = 'street %s' % (new_id)
	
number_of_ways = self.stack.child("button_group").selected().name()
number_of_lanes = self.stack.child("number_of_lanes").value()
lanes_max_speed = int(str(self.stack.child("lanes_max_speed").text()))

# TODO:       forcar pelo menos 1 source e os sinks minimos
# TODO:       garantir sections/lanesets com nodos distintos

new_street = {'street_id': new_id, 'street_name': suggested_name, 'sections': {}}

for i in range(selected_nodes.count()):
	if i == selected_nodes.count() - 1:
		break
	self.add_section(new_street, widget_text_to_node_id(selected_nodes.item(i).text()),
			 widget_text_to_node_id(selected_nodes.item(i+1).text()),
			 number_of_ways, number_of_lanes, lanes_max_speed)


parent_network['streets'][new_id] = new_street
self.flat_tree[new_id] = new_street
              
# selects the parent item on the QT tree and makes sure the tree remains expanded
self.QT_tree.setSelected(parent_item, 1)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)


self.update_tree_and_QT_tree({})

}




/*-----------------------------------------------------------------------*/
void editor::prepare_for_trafficlight_creation( QWidget * )
{
"""fills the widgets in the trafficlight creation window with the default values:
a list of all nodes in the listbox of available nodes and a suggestion of ID"""
widget = a0

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

(possible_nodes_ids, possible_nodes_verbose) = possible_nodes_for_trafficlights(self.tree)

located_at_node = widget.child("creation_trafficlight_located_at_node")
located_at_node.clear()
located_at_node.insertStrList(possible_nodes_verbose)

# # ok, let's suggest an id
# new_id = self.get_free_id()
# # the name is based on the next available id, but we don't want to mark it as used yet
# self.used_ids.remove(new_id)
# widget.child("trafficlight_id").setText("%s" % (new_id))
}





/*-----------------------------------------------------------------------*/
// note: how to connect signals and slots with parameters? create, in QT design,
// a slot with a signature of parameters equal to the signature of the signal.
// Apparently you should not name the parameters, just specify the types. Maybe
// it is some bug in sip, who knows?

void editor::prepare_for_object_creation( QWidget * )
{
widget = a0

if widget.name() == 'street_creation_page':
	self.prepare_for_street_creation(widget)
elif widget.name() == 'trafficlight_creation_page':
	self.prepare_for_trafficlight_creation(widget)
					 
}




/*-----------------------------------------------------------------------*/
void editor::add_trafficlight()
{

parent_item = self.QT_tree.selectedItem()

network_id = self.tree.keys()[0]
parent_network = self.tree[network_id]


# all_nodes = [n for n in parent_network['nodes'].keys()]
# located_at_node = all_nodes[0]
# print "-->", self.stack.child("creation_trafficlight_located_at_node").currentText()
located_at_node = widget_text_to_node_id(self.stack.child("creation_trafficlight_located_at_node").currentText())

tlight_id = self.get_free_id()

# new_tlight = {'traffic_light_id': tlight_id, 'located_at_node': located_at_node, 'signalplans': {}}
# new_tlight['signalplans'] = {1: {'phases': {}, 'signalplan_id': 1}}
# new_tlight['signalplans'][1]['phases'] = {0: {'directions': [[]], 'iteration_end': 0, 'iteration_start': 0, 'phase_id': 0}}

new_tlight = {'traffic_light_id': tlight_id, 'located_at_node': located_at_node}
new_tlight['signalplans'] =  {1: {'phases': {1: {'directions': [],
						 'iteration_end': 29,
						 'iteration_start': 0,
						 'phase_id': 1} },
				  'additional_info': '', 
				  'signalplan_id': 1}
}


# pretty_printer.pprint(new_tlight)

parent_network['traffic_lights'][tlight_id] = new_tlight
self.flat_tree[tlight_id] = new_tlight

# selects the parent item on the QT tree and makes sure the tree remains expanded
self.QT_tree.setSelected(parent_item, 1)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)



self.update_tree_and_QT_tree({})


}





/*-----------------------------------------------------------------------*/
void editor::add_signalplan()
{

# print "adicionando novo signalplan!"

parent_item = self.QT_tree.selectedItem()
parent_tlight = self.QT_tree.selectedItem().object_reference

used_ids = parent_tlight['signalplans'].keys()
used_ids.sort()

new_id = used_ids[-1] + 1

signalplans = parent_tlight['signalplans']

new_phase_id = -1
for sp in signalplans.values():
        phases = sp['phases'].values()
        for ph in phases:
                if ph['phase_id'] > new_phase_id:
                        new_phase_id = ph['phase_id']


new_phase_id += 1


# print "used ids=", used_ids

# print "vai botar no tlight", parent_tlight
# print "--> vai criar com id=", new_id

parent_tlight['signalplans'][new_id] = {'phases': {new_phase_id: {'directions': [],
						       'iteration_end': 0,
						       'iteration_start': 0,
						       'phase_id': new_phase_id} },
					'additional_info': '', 
					'signalplan_id': new_id}


# selects the parent item on the QT tree and makes sure the tree remains expanded
self.QT_tree.setSelected(parent_item, 1)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)

self.update_tree_and_QT_tree({})


}





/*-----------------------------------------------------------------------*/
void editor::delete_signalplan()
{

# print "removendo signalplan!"

parent_item = self.QT_tree.selectedItem()
selected_signalplan = self.QT_tree.selectedItem().object_reference
signalplan_id = selected_signalplan['signalplan_id']

# print "--> vai apagar a id=", signalplan_id

parent_tlight = self.QT_tree.selectedItem().parent().object_reference

if len(parent_tlight['signalplans']) < 2:
	qt.QMessageBox.critical(self, "Error",
				"The trafficlight must have at least one signalplan!",
				qt.QMessageBox.Ok)
else:
	del(parent_tlight['signalplans'][signalplan_id])

	# selects the parent item on the QT tree and makes sure the tree remains expanded
	self.QT_tree.setSelected(parent_item, 1)
	selected_item = self.QT_tree.selectedItem()
	self.QT_tree.setOpen(selected_item, 1)
	self.QT_tree.ensureItemVisible(selected_item)

	self.update_tree_and_QT_tree({})


}


/*-----------------------------------------------------------------------*/
void editor::update_signalplan_additional_info()
{

selected_signalplan = self.QT_tree.selectedItem().object_reference
additional_info_widget = self.stack.visibleWidget().child("signalplan_additional_info")

selected_signalplan['additional_info'] = str(additional_info_widget.text())

}



/*-----------------------------------------------------------------------*/
void editor::delete_trafficlight()
{
"""Deletes a traffic light"""

selected_tlight = self.QT_tree.selectedItem().object_reference
tlight_id = selected_tlight['traffic_light_id']
parent_item = self.QT_tree.selectedItem().parent()

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

del objects['traffic_lights'][tlight_id]
del self.flat_tree[tlight_id]

# selects the parent item on the QT tree and makes sure the tree remains expanded
self.QT_tree.setSelected(parent_item, 1)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)


self.update_tree_and_QT_tree({})


}



/*-----------------------------------------------------------------------*/
void editor::prepare_for_street_creation( QWidget * )
{
"""fills the widgets in the street creation window with the default values:
a list of all nodes in the listbox of available nodes, a suggestion for street name and
the default values for direction and number of lanes"""
widget = a0

available_nodes = widget.child("available_nodes")
selected_nodes = widget.child("selected_nodes")
	

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

all_nodes = [node_data_to_widget_text(n) for n in objects['nodes'].values()]

# prepares the listboxes of available and selected nodes
available_nodes.clear()
selected_nodes.clear()

available_nodes.insertStrList(all_nodes)
available_nodes.sort()

# ok, let's suggest a name too
new_id = self.get_free_id()
# the name is based on the next available id, but we don't want to mark it as used yet
self.used_ids.remove(new_id)
widget.child("street_name").setText("street %s" % (new_id))

# default direction = one way
widget.child("one_way").setChecked(1)

# default number of lanes = 1
widget.child("number_of_lanes").setValue(1)

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

# default_speed = objects['settings']['cars_maximum_speed']
# TODO: maneira mais decente
default_speed = 3

# default speed for the lanes
widget.child("lanes_max_speed").setText(str(default_speed))


}




/*-----------------------------------------------------------------------*/
void editor::add_node_to_street()
{
"""when the user clicks the add-arrow in the street creation
widget, this function is called in order to move the selected
item from the list of available nodes to the list of selected nodes"""

available_nodes = self.stack.child("available_nodes")
selected_nodes = self.stack.child("selected_nodes")

# is there anything selected?
if available_nodes.currentItem() != -1:
	selected_nodes.insertItem(available_nodes.currentText())
	available_nodes.removeItem(available_nodes.currentItem())

}




/*-----------------------------------------------------------------------*/
void editor::remove_node_from_street()
{
"""when the user clicks the remove-arrow in the street creation
widget, this function is called in order to move the selected
item from the list of selected nodes to the list of available nodes"""

available_nodes = self.stack.child("available_nodes")
selected_nodes = self.stack.child("selected_nodes")

# is there anything selected?
if selected_nodes.currentItem() != -1:
	available_nodes.insertItem(selected_nodes.currentText())
	selected_nodes.removeItem(selected_nodes.currentItem())

	available_nodes.sort()

}





/*-----------------------------------------------------------------------*/
void editor::move_node_up()
{
"""changes the order of the selected nodes for a given
street, in the street creation widget"""

selected_nodes = self.stack.child("selected_nodes")

current = selected_nodes.currentItem()
if current == 0:
	return

temp = selected_nodes.text(current - 1)
selected_nodes.changeItem(selected_nodes.text(current), current - 1)
selected_nodes.changeItem(temp, current)
selected_nodes.setSelected(current - 1, 1)


}





/*-----------------------------------------------------------------------*/
void editor::move_node_down()
{
"""changes the order of the selected nodes for a given
street, in the street creation widget"""

selected_nodes = self.stack.child("selected_nodes")

current = selected_nodes.currentItem()
if current == selected_nodes.count() - 1:
	return

temp = selected_nodes.text(current);
selected_nodes.changeItem(selected_nodes.text(current + 1), current)
selected_nodes.changeItem(temp, current + 1)
selected_nodes.setSelected(current + 1, 1)


}




/*-----------------------------------------------------------------------*/
void editor::add_source()
{
"""Adds a source"""

network_id = self.tree.keys()[0]
parent_network = self.tree[network_id]

# chooses the first available node for the initial value of 'located_at_node'
for street_properties in parent_network['streets'].values():
	for section_properties in street_properties['sections'].values():
		for laneset_properties in section_properties['lanesets'].values():
			located_at_node = laneset_properties['start_node']
			break
		
new_id = self.get_free_id()
new_source = {'source_id': new_id, 'first_activation_at': 0, 'located_at_node': located_at_node,
              'target_laneset': -1, 'source_activation_frequency': 1,
              'source_type': 'CONSTANT_FLOW', 'param': 1}

parent_network['sources'][new_id] = new_source
self.flat_tree[new_id] = new_source

self.update_tree_and_QT_tree({'object_reference': new_source, 'object_type': 'source'})

}





/*-----------------------------------------------------------------------*/
void editor::add_sink()
{
"""Adds a sink"""

network_id = self.tree.keys()[0]
parent_network = self.tree[network_id]

# chooses the first available node for the starting value of 'located_at_node' (could be any node)
# chooses the first available node for the initial value of 'located_at_node'
for street_properties in parent_network['streets'].values():
	for section_properties in street_properties['sections'].values():
		for laneset_properties in section_properties['lanesets'].values():
			located_at_node = laneset_properties['end_node']
			break

new_id = self.get_free_id()
new_sink = {'sink_id': new_id, 'first_activation_at': 0, 'located_at_node': located_at_node,
            'source_laneset': -1, 'removal_probability': 1.0 }

parent_network['sinks'][new_id] = new_sink
self.flat_tree[new_id] = new_sink
              
self.update_tree_and_QT_tree({'object_reference': new_sink, 'object_type': 'sink'})

}



/*-----------------------------------------------------------------------*/
void editor::add_node()
{
"""Adds a node"""

network_id = self.tree.keys()[0]
parent_network = self.tree[network_id]

new_id = self.get_free_id()
new_node = {'node_id': new_id, 'node_name': 'n%s' % (new_id), 'x_coord': 0, 'y_coord': 0 }

parent_network['nodes'][new_id] = new_node
self.flat_tree[new_id] = new_node

self.update_tree_and_QT_tree({'object_reference': new_node, 'object_type': 'node'})

}





/*-----------------------------------------------------------------------*/
void editor::delete_lane()
{
"""Deletes a lane"""

selected_lane = self.QT_tree.selectedItem().object_reference
lane_id = selected_lane['lane_id']

parent_laneset = self.QT_tree.selectedItem().parent().object_reference


if len(parent_laneset['lanes'].keys()) < 2:
	qt.QMessageBox.critical(self,
				"Insuficient number of lanes!",
				"The laneset must have at one lane!",
				qt.QMessageBox.Ok)
	return


del parent_laneset['lanes'][lane_id]
del self.flat_tree[lane_id]

self.update_tree_and_QT_tree({'object_reference': parent_laneset, 'object_type': 'laneset'})

}




/*-----------------------------------------------------------------------*/
void editor::delete_laneset()
{
"""Deletes a laneset"""

selected_laneset = self.QT_tree.selectedItem().object_reference
laneset_id = selected_laneset['laneset_id']

parent_section = self.QT_tree.selectedItem().parent().object_reference


if len(parent_section['lanesets'].keys()) < 2:
	qt.QMessageBox.critical(self,
				"Insuficient number of lanesets!",
				"The section must have at one laneset!",
				qt.QMessageBox.Ok)
	return


(error, error_message) = can_delete_laneset(selected_laneset, self.tree)
if error == 1:
	qt.QMessageBox.critical(self, "Laneset in use!", error_message, qt.QMessageBox.Ok)
	return

# deletes all lanes too
for i in selected_laneset['lanes'].keys():
	del self.flat_tree[i]

del parent_section['lanesets'][laneset_id]
del self.flat_tree[laneset_id]

self.update_tree_and_QT_tree({'object_reference': parent_section, 'object_type': 'section'})

}





/*-----------------------------------------------------------------------*/
void editor::delete_section()
{
"""Deletes a section"""

selected_section = self.QT_tree.selectedItem().object_reference
section_id = selected_section['section_id']

parent_street = self.QT_tree.selectedItem().parent().object_reference


if len(parent_street['sections'].keys()) < 2:
	qt.QMessageBox.critical(self,
				"Insuficient number of sections!",
				"The street must have at one section!",
				qt.QMessageBox.Ok)
	return


(error, error_message) = can_delete_section(selected_section, self.tree)
if error == 1:
	qt.QMessageBox.critical(self, "Section in use!", error_message, qt.QMessageBox.Ok)
	return

# deletes all lanesets and lanes too
for laneset_id, laneset_properties in selected_section['lanesets'].items():
	for lane_id in laneset_properties['lanes'].keys():
		del self.flat_tree[lane_id]
	del self.flat_tree[laneset_id]

del parent_street['sections'][section_id]
del self.flat_tree[section_id]

self.update_tree_and_QT_tree({'object_reference': parent_street, 'object_type': 'street'})

}





/*-----------------------------------------------------------------------*/
void editor::delete_street()
{
"""Deletes a street"""

selected_street = self.QT_tree.selectedItem().object_reference
street_id = selected_street['street_id']
parent_item = self.QT_tree.selectedItem().parent()

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

(error, error_message) = can_delete_street(selected_street, self.tree)
if error == 1:
	qt.QMessageBox.critical(self, "Street in use!", error_message, qt.QMessageBox.Ok)
	return

# deletes all sections, lanesets and lanes too
for section_id, section_properties in selected_street['sections'].items():
	for laneset_id, laneset_properties in section_properties['lanesets'].items():
		for lane_id in laneset_properties['lanes'].keys():
			del self.flat_tree[lane_id]
		del self.flat_tree[laneset_id]
	del self.flat_tree[section_id]

del objects['streets'][street_id]
del self.flat_tree[street_id]

# selects the parent item on the QT tree and makes sure the tree remains expanded
self.QT_tree.setSelected(parent_item, 1)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)


self.update_tree_and_QT_tree({})

}





/*-----------------------------------------------------------------------*/
void editor::delete_node()
{
"""Deletes a node"""

selected_node = self.QT_tree.selectedItem().object_reference
node_id = selected_node['node_id']
parent_item = self.QT_tree.selectedItem().parent()

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

used_by_laneset = -1
if len(selected_node['__lanesets_in']) != 0:
	used_by_laneset = selected_node['__lanesets_in'].keys()[0]

if len(selected_node['__lanesets_out']) != 0:
	used_by_laneset = selected_node['__lanesets_out'].keys()[0]
	

if used_by_laneset != -1:
	err = "This node is being used in a street (in <font color=red>laneset #%s</font>, " % used_by_laneset
	err += "for ex., maybe in others)!"
	qt.QMessageBox.critical(self, "Node in use!", err, qt.QMessageBox.Ok)
	return

for i in objects['sources'].values():
	if i['located_at_node'] == node_id:
		err = "This node is being used in the <font color=red>source #%s!</font>" % i['source_id']
		qt.QMessageBox.critical(self, "Node in use!", err, qt.QMessageBox.Ok)
		return

for i in objects['sinks'].values():
	if i['located_at_node'] == node_id:
		err = "This node is being used in the <font color=red>sink #%s!</font>" % i['sink_id']
		qt.QMessageBox.critical(self, "Node in use!", err, qt.QMessageBox.Ok)
		return

del objects['nodes'][node_id]
del self.flat_tree[node_id]

# selects the parent item on the QT tree and makes sure the tree remains expanded
self.QT_tree.setSelected(parent_item, 1)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)


self.update_tree_and_QT_tree({})


}





/*-----------------------------------------------------------------------*/
void editor::delete_source()
{
"""Deletes a source"""

selected_source = self.QT_tree.selectedItem().object_reference
source_id = selected_source['source_id']
parent_item = self.QT_tree.selectedItem().parent()

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

del objects['sources'][source_id]
del self.flat_tree[source_id]

# selects the parent item on the QT tree and makes sure the tree remains expanded
self.QT_tree.setSelected(parent_item, 1)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)


self.update_tree_and_QT_tree({})

}





/*-----------------------------------------------------------------------*/
void editor::delete_sink()
{
"""Deletes a sink"""

selected_sink = self.QT_tree.selectedItem().object_reference
sink_id = selected_sink['sink_id']
parent_item = self.QT_tree.selectedItem().parent()

network_id = self.tree.keys()[0]
objects = self.tree[network_id]

del objects['sinks'][sink_id]
del self.flat_tree[sink_id]

# selects the parent item on the QT tree and makes sure the tree remains expanded
#self.QT_tree.setSelected(parent_item, 1)
self.QT_tree.setCurrentItem(parent_item)
selected_item = self.QT_tree.selectedItem()
self.QT_tree.setOpen(selected_item, 1)
self.QT_tree.ensureItemVisible(selected_item)


self.update_tree_and_QT_tree({})

}





/*-----------------------------------------------------------------------*/
void editor::create_popups()
{
"""Creates all popups and fills them with the apropriate options (ex: the popup which will be
   fired when a laneset is clicked contains the options (delete laneset, add new lane)"""

self.lane_popup = QPopupMenu(self.QT_tree)
self.lane_popup.insertItem("Delete this lane", self.delete_lane)

self.laneset_popup = QPopupMenu(self.QT_tree)
self.laneset_popup.insertItem("Add new lane", self.add_lane_popup)
self.laneset_popup.insertSeparator()
self.laneset_popup.insertItem("Delete this laneset", self.delete_laneset)

self.section_popup = QPopupMenu(self.QT_tree)
self.section_popup.insertItem("Add new laneset", self.add_laneset_popup)
self.section_popup.insertSeparator()
self.section_popup.insertItem("Delete this section", self.delete_section)


self.street_popup = QPopupMenu(self.QT_tree)
self.street_popup.insertItem("Add new section", self.add_section_popup)
self.street_popup.insertSeparator()
self.street_popup.insertItem("Delete this street", self.delete_street)


self.node_popup = QPopupMenu(self.QT_tree)
self.node_popup.insertItem("Delete this node", self.delete_node)

self.source_popup = QPopupMenu(self.QT_tree)
self.source_popup.insertItem("Delete this source", self.delete_source)

self.sink_popup = QPopupMenu(self.QT_tree)
self.sink_popup.insertItem("Delete this sink", self.delete_sink)

self.streets_popup = None

self.traffic_lights_popup = None
self.tlight_popup = QPopupMenu(self.QT_tree)
self.tlight_popup.insertItem("Add new signalplan", self.add_signalplan)
self.tlight_popup.insertSeparator()
self.tlight_popup.insertItem("Delete this traffic light", self.delete_trafficlight)

self.signalplan_popup = QPopupMenu(self.QT_tree)
self.signalplan_popup.insertItem("Delete this signalplan", self.delete_signalplan)

self.nodes_popup = QPopupMenu(self.QT_tree)
self.nodes_popup.insertItem("Add new node", self.add_node)

self.sources_popup = QPopupMenu(self.QT_tree)
self.sources_popup.insertItem("Add new source", self.add_source)

self.sinks_popup = QPopupMenu(self.QT_tree)
self.sinks_popup.insertItem("Add new sink", self.add_sink)

self.network_popup = None
self.settings_popup = None


}





/*-----------------------------------------------------------------------*/
void editor::get_free_id()
{
"""Returns and object ID which was not yet used"""

i = 1
while 1:
	if i not in self.used_ids:
		self.used_ids.append(i)
		self.used_ids.sort()
		return i
	i+=1

}




/*-----------------------------------------------------------------------*/
void editor::update_tree_and_QT_tree( int item )
{
"""Updates the tree (ie: calls complement_tree) and rebuilds the QT tree to
   reflect these updates. Receives as parameter the item which should be
   selected on the QT tree after its reconstruction"""

item = a0

group_items = ('streets', 'nodes', 'sources', 'sinks', 'traffic_lights')

if len(item) == 0:
    selected_item = self.QT_tree.selectedItem()
    selected_item_text = selected_item.text(0)
    selected_item_type = selected_item_text
else:
    selected_item_type = item['object_type']
    selected_item_object = item['object_reference']

# selected_item = self.QT_tree.selectedItem()
# previous_item_text = selected_item.text(0)

self.tree = complement_tree(self.tree, self.flat_tree)
self.build_QT_tree(self, self.tree)

# previous_item = self.QT_tree.findItem(previous_item_text, 0)
# self.QT_tree.setSelected(previous_item, 1)
# selected_item = self.QT_tree.selectedItem()

# print "aparentemente pode ser", previous_item
# print "caralho", selected_item.text(0)

# self.QT_tree.setOpen(selected_item, 1)
# self.QT_tree.ensureItemVisible(selected_item)


# finds the previously selected item. if it was a group_item, search it by the text; otherwise, by the id
if selected_item_type in group_items:
    previous_item = self.QT_tree.findItem(selected_item_text, 0)
elif selected_item_type == 'settings':
    settings = self.QT_tree.findItem("settings", 0)
    previous_item = settings
else:
    network = self.QT_tree.findItem("network", 0)
    iterator = QListViewItemIterator(network);
    while (iterator.current()):
        previous_item = iterator.current()
        if (previous_item.object_type not in group_items) and (previous_item.object_type == selected_item_type):
            id_attribute = "%s_id" % selected_item_type
            if cmp_ids(previous_item.object_reference[id_attribute], selected_item_object[id_attribute]):
                break
        iterator += 1


# if modifying the network tab, explicity makes sure it will remain opened and expanded
if previous_item.object_type == 'network':
	self.QT_tree.setSelected(self.QT_tree.firstChild(), 1)
	selected_item = self.QT_tree.selectedItem()
	self.QT_tree.setOpen(selected_item, 1)
	self.QT_tree.ensureItemVisible(selected_item)
else:
	# re-selects the previously selected item
	self.QT_tree.setCurrentItem(previous_item)
	self.QT_tree.setOpen(previous_item, 1)
	self.QT_tree.ensureItemVisible(previous_item)

}





/*-----------------------------------------------------------------------*/
void editor::update_phases_duration()
{

if self.do_not_update_phases:
	return


# print "updating!!!"
selected_signalplan = self.QT_tree.selectedItem().object_reference

available_phases_widget = self.stack.visibleWidget().child("available_phases")
current_phase_id = int(str(available_phases_widget.currentText()))


phase_duration_widget = self.stack.visibleWidget().child("signalplan_phase_duration")
new_duration = phase_duration_widget.value()

phases_to_update = [i for i in selected_signalplan['phases'].keys()
		    if i > current_phase_id]

# print "phases to update:", phases_to_update

current_phase_start = selected_signalplan['phases'][current_phase_id]['iteration_start']
selected_signalplan['phases'][current_phase_id]['iteration_end'] = current_phase_start + new_duration - 1
current_phase_end = selected_signalplan['phases'][current_phase_id]['iteration_end']

for i in phases_to_update:
	phase_length = selected_signalplan['phases'][i]['iteration_end'] - selected_signalplan['phases'][i]['iteration_start'] + 1
	selected_signalplan['phases'][i]['iteration_start'] = current_phase_end + 1
	selected_signalplan['phases'][i]['iteration_end'] = current_phase_end + phase_length
	current_phase_end = selected_signalplan['phases'][i]['iteration_end']

# print "*-"*65
# pretty_printer.pprint(selected_signalplan)
# print "*-"*65

}




/*-----------------------------------------------------------------------*/
void editor::add_phase()
{

selected_signalplan = self.QT_tree.selectedItem().object_reference

phases_ids = selected_signalplan['phases'].keys()
phases_ids.sort()

end_of_last_phase = selected_signalplan['phases'][phases_ids[-1]]['iteration_end']
# print "---------->", phases_ids, phases_ids[-1]


parent_trafficlight = self.QT_tree.selectedItem().parent().object_reference
signalplans = parent_trafficlight['signalplans']

new_phase_id = -1
for sp in signalplans.values():
	phases = sp['phases'].values()
	for ph in phases:
		if ph['phase_id'] > new_phase_id:
			new_phase_id = ph['phase_id']


new_phase_id += 1


selected_signalplan['phases'][new_phase_id] = {'directions': [],
					       'iteration_end': end_of_last_phase + 30,
					       'iteration_start': end_of_last_phase + 1,
					       'phase_id': new_phase_id}


available_phases_widget = self.stack.visibleWidget().child("available_phases")
available_phases_widget.insertItem(str(new_phase_id))
available_phases_widget.setSelected(new_phase_id, 1)
available_phases_widget.ensureCurrentVisible()

# print "*-"*65
# pretty_printer.pprint(selected_signalplan)
# print "*-"*65

}





/*-----------------------------------------------------------------------*/
void editor::delete_phase()
{

# print "deleting!!"

selected_signalplan = self.QT_tree.selectedItem().object_reference
available_phases_widget = self.stack.visibleWidget().child("available_phases")

if len(selected_signalplan['phases'].keys()) < 2:
	qt.QMessageBox.critical(self,
				"Insuficient number of phases!",
				"The signalplan must have at one phase!",
				qt.QMessageBox.Ok)
	return


current_phase_id = int(str(available_phases_widget.currentText()))

del(selected_signalplan['phases'][current_phase_id])

phases_to_update = [i for i in selected_signalplan['phases'].keys()
		    if i > current_phase_id]


# update the ids of the phases that follow the one we just deleted
for i in phases_to_update:
	selected_signalplan['phases'][i-1] = selected_signalplan['phases'][i]
	selected_signalplan['phases'][i-1]['phase_id'] = i-1
	del(selected_signalplan['phases'][i])


available_phases_widget.clear()
all_phases = [i for i in selected_signalplan['phases'].keys()]
all_phases.sort()
available_phases_widget.insertStrList([str(i) for i in all_phases])
available_phases_widget.setSelected(0, 1)
available_phases_widget.ensureCurrentVisible()

self.update_phases_duration()

}





/*-----------------------------------------------------------------------*/
void editor::paint_directions()
{

# print "\n\n\n\n-::::::::::::::::::::::::::::::"

selected_signalplan = self.QT_tree.selectedItem().object_reference
signalplan_id = selected_signalplan['signalplan_id']

available_phases_widget = self.stack.visibleWidget().child("available_phases")
current_phase_id = int(str(available_phases_widget.currentText()))
current_phase_iter_start = selected_signalplan['phases'][current_phase_id]['iteration_start']
current_phase_iter_end = selected_signalplan['phases'][current_phase_id]['iteration_end']

phase_duration_widget = self.stack.visibleWidget().child("signalplan_phase_duration")

# set a flag to avoid the calling of 'update_phases_duration()'
self.do_not_update_phases = 1
phase_duration_widget.setValue(current_phase_iter_end-current_phase_iter_start+1)
self.do_not_update_phases = 0

# pretty_printer.pprint(selected_signalplan['phases'])
# print "direcoes da fase atual:"
directions = selected_signalplan['phases'][current_phase_id]['directions']
# pretty_printer.pprint(directions)

canvas = self.stack.visibleWidget().child("drawing_frame").canvas()
polygons = [i for i in canvas.allItems() if i.__class__ == my_qcanvaspolygon]

# remove the previously drawn connecting lines from the canvas
connecting_lines = [i for i in canvas.allItems() if i.__class__ == my_qconnecting_line]
for i in connecting_lines:
	i.setCanvas(None)

# remove the previously drawn arrows from the canvas
arrows = [i for i in canvas.allItems() if i.__class__ == qtcanvas.QCanvasPolygon]
for i in arrows:
        if i.points().size() == 4:
            i.setCanvas(None)

connecting_point  = {}  # a dictionary which associated the id of a {lane,laneset} with the
                        # (x,y) coordinates of its connecting point with other objects

for i in polygons:
    (class_name, object_ref) = i.class_and_ref()
    connecting_point[object_ref['%s_id'%class_name]] = (i.connecting_x, i.connecting_y)


pen = qt.QPen(qt.QColor(60, 70, 200), 5, qt.Qt.SolidLine)

          
cur_color=10
for d in directions:
    start_x = connecting_point[d[0]][0]
    start_y = connecting_point[d[0]][1]
    end_x   = connecting_point[d[1]][0]
    end_y   = connecting_point[d[1]][1]
    
    connecting_line = my_qconnecting_line(canvas)
    connecting_line.setPoints(start_x, start_y, end_x, end_y)
    connecting_line.setZ(1.0) # in order to appear above all the rest
    connecting_line.setPen(pen_colors[cur_color % len(pen_colors)])
    connecting_line.show()

    arrow_side_length = 15
    arrow_polygon = calculate_arrow(start_x, start_y, end_x, end_y, arrow_side_length, canvas)
    arrow_polygon.setPen(pen_colors[cur_color % len(pen_colors)])
    arrow_polygon.setBrush(brush_colors[cur_color % len(pen_colors)])

    arrow_polygon.show()

    cur_color += 0

canvas.update()

}





