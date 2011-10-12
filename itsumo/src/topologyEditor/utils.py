import qt, qttable, qtcanvas, string
from tlights_drawing import *
import pprint, random, math
pretty_printer = pprint.PrettyPrinter()



pen_colors = [qt.QPen(qt.QColor(60,  70,  200), 3, qt.Qt.SolidLine), # azul escuro
              qt.QPen(qt.QColor(200, 70,  110), 3, qt.Qt.SolidLine), # roxo avermelhado
              qt.QPen(qt.QColor(125, 55,  125), 3, qt.Qt.SolidLine), # roxo escuro
              qt.QPen(qt.QColor(125, 120,  25), 3, qt.Qt.SolidLine), # mostarda
              qt.QPen(qt.QColor(150, 35,   35), 3, qt.Qt.SolidLine), # vermelho escuro
              qt.QPen(qt.QColor(220, 125 , 45), 3, qt.Qt.SolidLine), # laranja
              qt.QPen(qt.QColor(220, 210, 250), 3, qt.Qt.SolidLine), # cinza azulado
              qt.QPen(qt.QColor(55,  35,  120), 3, qt.Qt.SolidLine), # roxo azulado
              qt.QPen(qt.QColor(220, 130, 200), 3, qt.Qt.SolidLine), # rosa gay
              qt.QPen(qt.QColor(45,  30,   70), 3, qt.Qt.SolidLine), # azul escurao
	      qt.QPen(qt.QColor(20,  110,  20), 3, qt.Qt.SolidLine)  # verde escurao
              ]

# 43 73 166   azul tri

brush_colors = [qt.QBrush(qt.QColor(60,  70,  200)), # azul escuro
		qt.QBrush(qt.QColor(200, 70,  110)), # roxo avermelhado
		qt.QBrush(qt.QColor(125, 55,  125)), # roxo escuro
		qt.QBrush(qt.QColor(125, 120,  25)), # mostarda
		qt.QBrush(qt.QColor(150, 35,   35)), # vermelho escuro
		qt.QBrush(qt.QColor(220, 125 , 45)), # laranja
		qt.QBrush(qt.QColor(220, 210, 250)), # cinza azulado
		qt.QBrush(qt.QColor(55,  35,  120)), # roxo azulado
		qt.QBrush(qt.QColor(220, 130, 200)), # rosa gay
		qt.QBrush(qt.QColor(45,  30,   70)),  # azul escurao
		qt.QBrush(qt.QColor(20,  110,  20))  # verde escurao
		]



#-----------------------------------------------------------------------#
def rotate(x, y, angle):
	return (x * math.cos(angle) - y * math.sin(angle),
		y * math.cos(angle) + x * math.sin(angle))


#-----------------------------------------------------------------------#
class tree_item(qt.QListViewItem):
	"""tree_item is an extension of the QListViewItem designed so that
	each item of QT's Listview object can store additional information"""
	def __init__(self, parent, column1, _all_objects, _object_reference, _object_type):
		qt.QListViewItem.__init__(self, parent, column1)

		# a reference to the complete tree of network objects
		self.all_objects = _all_objects
		# a reference to the specific object associated with this item (ex: node 123)
		self.object_reference = _object_reference
		# the type of the object associated with this item (ex: 'lane', 'laneset', 'street', ...)
		self.object_type = _object_type



#-----------------------------------------------------------------------#
class my_qconnecting_line(qtcanvas.QCanvasLine):
	def __init__(self, parent):
		qtcanvas.QCanvasLine.__init__(self, parent)



#-----------------------------------------------------------------------#
class my_qcanvaspolygon(qtcanvas.QCanvasPolygon):
	def __init__(self, parent_canvas, obj_ref, connecting_x, connecting_y):
		qtcanvas.QCanvasPolygon.__init__(self, parent_canvas)
		self.obj_ref = obj_ref
		self.connecting_x = connecting_x
		self.connecting_y = connecting_y

	
	def class_and_ref(self):
		object = self.obj_ref
		try:
			id = object['section_id']
			ref = object
			class_name = 'section'
		except KeyError:
			try:
				id = object['laneset_id']
				ref = object
				class_name = 'laneset'
			except KeyError:
				id = object['lane_id']
				ref = object
				class_name = 'lane'

 		return (class_name, ref)





#-----------------------------------------------------------------------#
class my_canvasview(qtcanvas.QCanvasView):
	def __init__(self, parent, name):
		qtcanvas.QCanvasView.__init__(self, parent, name)

		self.current_signalplan = None

		# stores the points clicked by the user and then uses them
		# to draw a line and to determine which objects were selected
		self.__class__.start_point = (0, 0)
		self.__class__.end_point   = (0, 0)
		self.__class__.start_object = None
		self.__class__.end_object   = None


	def contentsMousePressEvent(self, e):
		i = self.canvas().collisions( e.pos() )
		i = [obj for obj in i if obj.__class__ == my_qcanvaspolygon]
		if len(i) > 0:
			if i[0].__class__ == my_qcanvaspolygon:
				pen = qt.QPen(qt.QColor(60, 70, 200), 5, qt.Qt.SolidLine)
				self.__class__.dragging_line = qtcanvas.QCanvasLine(self.canvas())
				self.__class__.dragging_line.setZ(1.0) # in order to appear above all the rest
				self.__class__.dragging_line.setPen(pen)
				
				self.__class__.start_point = (e.x(), e.y())
				(class_name, object_ref) = i[0].class_and_ref()
				self.__class__.start_object = object_ref
# 				print "clicou na %s #%d" % (class_name, object_ref['%s_id'%class_name])

			else:
				self.__class__.start_object = None
				self.__class__.end_object   = None

	def contentsMouseReleaseEvent(self, e):
		i = self.canvas().collisions( e.pos() )
		i = [obj for obj in i if obj.__class__ == my_qcanvaspolygon]
		if len(i) > 0:
			if i[0].__class__ == my_qcanvaspolygon:
				
				self.__class__.end_point = (e.x(), e.y())
				(class_name, object_ref) = i[0].class_and_ref()
				self.__class__.end_object = object_ref
# 				print "soltou na %s #%d" % (class_name, object_ref['%s_id'%class_name])

				available_phases = self.canvas().parent().child("available_phases")
				current_phase_id = int(str(available_phases.currentText()))

				update_directions(self.current_signalplan, current_phase_id,
						  self.__class__.start_object, self.__class__.end_object)

				editor = self.parent()
				while editor.parent() != None:
					editor = editor.parent()
				editor.paint_directions()
				

			else:
				self.__class__.start_object = None
				self.__class__.end_object   = None

		if (self.__class__.dragging_line):
			self.__class__.dragging_line.hide()
		self.canvas().update()


	def contentsMouseMoveEvent(self, e):
		if (self.__class__.start_object):
			start_x = self.__class__.start_point[0]
			start_y = self.__class__.start_point[1]
			end_x = e.x()
			end_y = e.y()
			
			self.__class__.dragging_line.setPoints(start_x, start_y, end_x, end_y)
			self.__class__.dragging_line.show()

			self.canvas().update()



#-----------------------------------------------------------------------#
def cmp_ids(a, b):
	"""checks if two IDs are the same"""
	return int(a) == int(b)


#-----------------------------------------------------------------------#
def node_data_to_widget_text(node):
	"""converts the tree representation of a node to a text representation
	that can be used, for example, in QT comboboxes"""
	return '%s       (id: %s)' % (node['node_name'], node['node_id'])


#-----------------------------------------------------------------------#
def widget_text_to_node_id(text):
	"""the inverse of the above function (returns the node ID based on the
	node's text representation created by 'node_data_do_widget_text')"""
	return int(str(text[text.find(': ')+2:-1]))

#-----------------------------------------------------------------------#
def laneset_data_to_widget_text(laneset, flat_tree):
	"""converts the tree representation of a laneset to a text representation
	that can be used, for example, in QT comboboxes"""

	start_node_name = get_element_by_id(flat_tree, laneset[0]['start_node'])['node_name']
	end_node_name = get_element_by_id(flat_tree, laneset[0]['end_node'])['node_name']

	return '%s (\'%s -> %s\' in %s)' % (laneset[0]['laneset_id'], start_node_name, end_node_name, laneset[2])
				    
#-----------------------------------------------------------------------#
def widget_text_to_laneset_id(text):
	"""the inverse of the above function (returns the laneset ID based on the
	laneset's text representation created by 'laneset_data_to_widget_text')"""
	return int(str(text[0:text.find('(')-1]))


#-----------------------------------------------------------------------#
def possible_nodes_for_sources_and_sinks(tree, all_lanesets):
	"""calculates which are the possible nodes for use in sources and sinks.
	sources can only be placed in nodes which lead to some street. sinks can only
	be placed in nodes which are the end of some street"""

	network_id = tree.keys()[0]
	objects = tree[network_id]
	
	
	# finds out which nodes can be used as sources (ie: nodes that are in the beginning of at least one
	# street) and nodes that can be sinks (ie: nodes that are in the end of at least one street)
	source_nodes = {}
	sink_nodes = {}
	for n in objects['nodes'].values():
		for l in all_lanesets:
			if type(n['node_id']) != type(l[0]['start_node']):
				print "DEBUG EXPLOSION! possible_nodes_for_sources_and_sinks"
			if cmp_ids(n['node_id'], l[0]['start_node']):
				source_nodes[n['node_id']] = n
			if type(n['node_id']) != type(l[0]['end_node']):
				print "DEBUG EXPLOSION! possible_nodes_for_sources_and_sinks"
			if cmp_ids(n['node_id'], l[0]['end_node']):
				sink_nodes[n['node_id']] = n
	# list simular to [13, 15, ...]
	source_nodes_ids = [n for n in source_nodes.keys()]
	# list similar to ["nodo inicial (#13)", "nodo final (#15)", ...]
	source_nodes_verbose = [node_data_to_widget_text(n) for n in source_nodes.values()]
	# the same as 'source_nodes'
	sink_nodes_ids = [n for n in sink_nodes.keys()]
	# the same as 'source_nodes_verbose'
	sink_nodes_verbose = [node_data_to_widget_text(n) for n in sink_nodes.values()]

	return (source_nodes_ids, source_nodes_verbose,
		sink_nodes_ids, sink_nodes_verbose)



#-----------------------------------------------------------------------#
def possible_nodes_for_trafficlights(tree):
	"""calculates which are the possible nodes for use in traffig lights.
	trafficlights can only be placed in nodes which are in the middle of a street
	(that is, which have both an incoming and an outgoing laneset)"""

	network_id = tree.keys()[0]
	objects = tree[network_id]
	
	
	# finds out which nodes can be used as sources (ie: nodes that are in the beginning of at least one
	# street) and nodes that can be sinks (ie: nodes that are in the end of at least one street)
	tlight_nodes = {}
	for n in objects['nodes'].values():
		if len(n['__lanesets_in'].keys()) > 0 and len(n['__lanesets_out'].keys()) > 0:
			tlight_nodes[n['node_id']] = n
			
	# list simular to [13, 15, ...]
	tlight_nodes_ids = [n for n in tlight_nodes.keys()]
	# list similar to ["nodo inicial (#13)", "nodo final (#15)", ...]
	tlight_nodes_verbose = [node_data_to_widget_text(n) for n in tlight_nodes.values()]

	return (tlight_nodes_ids, tlight_nodes_verbose)


#-----------------------------------------------------------------------#
def all_nodes_and_all_lanesets(tree):
	"""returns lists with a text representation of all nodes and of all lanesets.
	these specially formatted text data will be used when filling values in QT widgets"""
	
	network_id = tree.keys()[0]
	objects = tree[network_id]


	# creates lists [all_nodes], [all sections,street_name] and [all_lanesets,section_name,street_name]
	all_nodes_ids = [n for n in objects['nodes'].keys()]
	all_nodes_verbose = [node_data_to_widget_text(n) for n in objects['nodes'].values()]
	all_lanesets_ids = []
	all_lanesets_verbose = []
	for street_properties in objects['streets'].values():
		for section_properties in street_properties['sections'].values():
			for laneset_properties in section_properties['lanesets'].values():
				all_lanesets_ids.append(laneset_properties['laneset_id'])
				all_lanesets_verbose.append([laneset_properties,
							     section_properties['section_name'],
							     street_properties['street_name']])
	return (all_nodes_ids, all_nodes_verbose,
		all_lanesets_ids, all_lanesets_verbose)



#-----------------------------------------------------------------------#
def complement_sources(tree, flat_tree):
	"""complements all sources with additional information, such as the possible nodes
	where it can be placed. Additionally, this function detects if the
	source's node changed and automatically selects a new value for its
	'target_laneset' property (the process of selecting a new value is called
	'fallback')."""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	(all_nodes_ids, all_nodes_verbose,
	 all_lanesets_ids, all_lanesets_verbose) = all_nodes_and_all_lanesets(tree)

	(source_nodes_ids,
	 source_nodes_verbose,
	 sink_nodes_ids,
	 sink_nodes_verbose) = possible_nodes_for_sources_and_sinks(tree, all_lanesets_verbose)


	need_rebuild = 0
	
	# complements the sources with the list of all nodes and the list of possible target lanesets
	sources = objects['sources']
	for s in sources.values():
 		s['__possible_nodes'] = source_nodes_verbose
		possible_lanesets_ids = [l[0]['laneset_id'] for l in all_lanesets_verbose \
					 if cmp_ids(l[0]['start_node'], s['located_at_node'])]
		possible_lanesets_verbose = [laneset_data_to_widget_text(l, flat_tree) \
					    for l in all_lanesets_verbose \
					    if cmp_ids(l[0]['start_node'], s['located_at_node'])]

		s['__possible_lanesets'] = possible_lanesets_verbose

		# oops, the source's node is not valid!
		if s['located_at_node'] not in source_nodes_ids:
			err = "OPA, source nao tem nenhum nodo! isso nao podia ter acontecido"
			return (1, err, 0, 0)
			
		# oops, the source's targetlaneset if not valid!
		if s['target_laneset'] not in possible_lanesets_ids:
			# return error if there is no possible substitute
			if len(possible_lanesets_ids) == 0:
				err = "OPA, source nao tem nenhuma laneset! isso nao podia ter acontecido"

				return (1, err, 0, 0)
			# otherwise fallsback to a valid laneset
			print "falling back LANESET no source", s['source_id'], "de", \
			      s['target_laneset'], "para", possible_lanesets_ids[0]
			s['target_laneset'] = possible_lanesets_ids[0]
			need_rebuild = 1
	return (0, 0, need_rebuild, tree)



#-----------------------------------------------------------------------#
def complement_sinks(tree, flat_tree):
	"""complements all sinks with additional information, such as the possible nodes
	where it can be placed. Additionally, this function detects if the
	sinks's node changed and automatically selects a new value for its
	'source_laneset' property (the process of selecting a new value is called
	'fallback')."""


	network_id = tree.keys()[0]
	objects = tree[network_id]

	(all_nodes_ids, all_nodes_verbose,
	 all_lanesets_ids, all_lanesets_verbose) = all_nodes_and_all_lanesets(tree)

	(source_nodes_ids,
	 source_nodes_verbose,
	 sink_nodes_ids,
	 sink_nodes_verbose) = possible_nodes_for_sources_and_sinks(tree, all_lanesets_verbose)

	need_rebuild = 0

	# complements the sinks with the list of all nodes and the list of possible source lanesets
	sinks = objects['sinks']
	for s in sinks.values():
 		s['__possible_nodes'] = sink_nodes_verbose

		possible_lanesets_ids = [l[0]['laneset_id'] \
					for l in all_lanesets_verbose \
					if cmp_ids(l[0]['end_node'], s['located_at_node'])]
		possible_lanesets_verbose = [laneset_data_to_widget_text(l, flat_tree) \
					    for l in all_lanesets_verbose \
					    if cmp_ids(l[0]['end_node'], s['located_at_node'])]
		
		s['__possible_lanesets'] = possible_lanesets_verbose

		# oops, the sink's node is not valid!
		if s['located_at_node'] not in sink_nodes_ids:
			err = "OPA, sink nao tem nenhum nodo! isso nao podia ter acontecido"
			return (1, err, 0, 0)

		# oops, the sink's targetlaneset is not valid!
		if s['source_laneset'] not in possible_lanesets_ids:
			# return error if there is no possible substitute
			if len(sink_nodes_ids) == 0:
				err = "OPA, sink nao tem nenhuma laneset! isso nao podia ter acontecido"
				return (1, err, 0, 0)

			# otherwise fallsback to a valid laneset
			print "falling back LANESET no sink", s['sink_id'], "de", \
			      s['source_laneset'], "para", possible_lanesets_ids[0]
			s['source_laneset'] = possible_lanesets_ids[0]
			need_rebuild = 1
	return (0, 0, need_rebuild, tree)



#-----------------------------------------------------------------------#
def complement_tlights(tree, flat_tree):
	"""complements the traffic lights with the list of possibles nodes where
	they can be placed"""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	(tlight_nodes_ids, tlight_nodes_verbose) = possible_nodes_for_trafficlights(tree)

	for i in objects['traffic_lights'].values():
		i['__possible_nodes'] = tlight_nodes_verbose

	return tree



#-----------------------------------------------------------------------#
def complement_sections_and_lanesets_and_nodes(tree, flat_tree):
	"""complements all sections with the possible delimiting nodes it can use;
	complements all lanesets with the possibles values for 'start_node'
	and 'end_node', depending on its parent's delimiting nodes;
	
	complements all nodes with additional information regarding the
	lanesets which start/end on it;
	
	Additionally, this function detects if the sections's delimiting nodes changed and
	automatically selects a new value for its lanesets 'start_node' and 'end_node'
	properties (the process of selecting a new value is called 'fallback')."""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	(all_nodes_ids, all_nodes_verbose,
	 all_lanesets_ids, all_lanesets_verbose) = all_nodes_and_all_lanesets(tree)

	(source_nodes_ids,
	 source_nodes_verbose,
	 sink_nodes_ids,
	 sink_nodes_verbose) = possible_nodes_for_sources_and_sinks(tree, all_lanesets_verbose)

	need_rebuild = 0

	# for all nodes, clear the list of incoming/outcoming lanesets
	for i in objects['nodes'].values():
		i['__lanesets_in'] = {}
		i['__lanesets_out'] = {}


	# complements the sections and the lanesets with the list of possible nodes
	for street in objects['streets'].values():
		for section in street['sections'].values():
			section['__possible_nodes'] = all_nodes_verbose

			# makes sure the end starts/ends in valid nodes; otherwise,
			# selects a new valid value
			if section['delimiting_node_1'] not in all_nodes_ids:
				print "falling back NODE 1 na section", section['section_id'], "de", \
				      section['delimiting_node_1'], "para", all_nodes_ids[0]

				section['delimiting_node_1'] = all_nodes_ids[0]
				need_rebuild = 1

			if section['delimiting_node_2'] not in all_nodes_ids:
				print "falling back NODE 2 na section", section['section_id'], "de", \
				      section['delimiting_node_2'], "para", all_nodes_ids[1]

				section['delimiting_node_2'] = all_nodes_ids[1]
				need_rebuild = 1

			# finds out the delimiting nodes
			sec_n1 = get_element_by_id(flat_tree, section['delimiting_node_1'])
			sec_n2 = get_element_by_id(flat_tree, section['delimiting_node_2'])

			if section['section_name'] == '':
				section['section_name'] = "(%s) <-> (%s)" % (sec_n1['node_name'], sec_n2['node_name'])

			for laneset in section['lanesets'].values():

				# fills the possible nodes for the laneset
				laneset['__possible_nodes'] = [node_data_to_widget_text(sec_n1),
							       node_data_to_widget_text(sec_n2)]

				# makes sure the laneset starts/ends in valid nodes; otherwise,
				# selects a new valid value
				if laneset['start_node'] not in [section['delimiting_node_1'],
								 section['delimiting_node_2']]:
					print "falling back NODE na laneset", laneset['laneset_id'], "de", \
					      laneset['start_node'], "para", section['delimiting_node_1']
					laneset['start_node'] = section['delimiting_node_1']
					need_rebuild = 1
				if laneset['end_node'] not in [section['delimiting_node_1'], 
							       section['delimiting_node_2']]:
					print "falling back NODE na laneset", laneset['laneset_id'], "de", \
					      laneset['end_node'], "para", section['delimiting_node_2']
					laneset['end_node'] = section['delimiting_node_2']
					need_rebuild = 1

				# complements the nodes with the information of which lanesets
				# start/end on it
				n_start = get_element_by_id(flat_tree, laneset['start_node'])
				n_end = get_element_by_id(flat_tree, laneset['end_node'])

				n_start['__lanesets_out'][laneset['laneset_id']] = laneset
				n_end['__lanesets_in'][laneset['laneset_id']] = laneset
				
				
	return (need_rebuild, tree)




#-----------------------------------------------------------------------#
def complement_tree(tree, flat_tree):
	"""complements the tree of objects with additional information (ex: the possible
	nodes where a source can be inserted, etc)"""

	# complements the sources
	(error, error_message, need_rebuild, new_tree) = complement_sources(tree, flat_tree)
	if error == 1:
		print "ERRO complement_source", error_message
		return
	tree = new_tree
	# fucking cascade updates!
	if need_rebuild == 1:
		tree = complement_tree(tree, flat_tree)
		
			


	# complements the sinks
	(error, error_message, need_rebuild, new_tree) = complement_sinks(tree, flat_tree)
	if error == 1:
		print "ERRO complement_sink", error_message
		return
	tree = new_tree
	# fucking cascade updates!
	if need_rebuild == 1:
		tree = complement_tree(tree, flat_tree)
		


	# complements the sections, lanesets and nodes
	(need_rebuild, new_tree) = complement_sections_and_lanesets_and_nodes(tree, flat_tree)
	tree = new_tree
	if need_rebuild == 1:
		tree = complement_tree(tree, flat_tree)

	tree = complement_tlights(tree, flat_tree)

	# if it didn't catch errors anywhere before this line, exit with success
	return (tree)




#-----------------------------------------------------------------------#
def update_tree_from_widget(widget, attribute_name, object, tree):
	"""updates an object, as kept in memory, according to the new value of a QT widget.
	this function calls the apropriate methods to update the tree depending
	on the type of the widget which was modified (combobox, qtable, etc)"""

	if widget.__class__ == qt.QComboBox:
		(error, error_message) = update_object_from_combobox(widget, attribute_name, object, tree)
		if error == 1:
			return (error, error_message)
		

	elif widget.__class__ == qttable.QTable:
		widget_name = widget.name()[widget.name().find('_')+1:]
		if widget_name == "turning_probabilities":
			for i in range(widget.numRows()):
				dest_laneset_id = int(widget_text_to_laneset_id(widget.text(i, 0)))
				new_prob = float(str(widget.text(i, 1)))
				object['turning_probabilities'][dest_laneset_id] = new_prob
		elif widget_name == "source_variable":
			param = []
			for i in range(widget.numRows()):
				number_of_iter = int(str(widget.text(i, 0)))
				if object['source_type'] == "VARIABLE_FLOW":
					v = int(str(widget.text(i, 1)))
				else:
					v = float(str(widget.text(i, 1)))
				param.append([number_of_iter, v])
			object['param'] = param

		else:
			print "oops, update table nao implementada p/", widget_name
			

		
	elif widget.__class__ == qt.QCheckBox:
		if widget.isChecked():
			object[attribute_name] = 'true'
		else:
			object[attribute_name] = 'false'
			

	else:
		if attribute_name.find('id') != -1:
			# ids are int
			object[attribute_name] = int(str(widget.text()))
		else:
			# everything else can be a string, i hope
			object[attribute_name] = str(widget.text())

	# no errors
	return (0, 0)



#-----------------------------------------------------------------------#
def can_update_delimiting_node(object, which_node, tree):
	"""when the user tries to modify a delimiting node for a section,
	this function verifies if this modification won't affect
	any other objects"""

	lanesets = object['lanesets']
	for i in lanesets.values():
		# we're going to check, for each laneset of the current section, if it is
		# going to be affected by the change
		if object[which_node] == i['start_node']:
			(err, err_msg) = can_update_start_node(i, tree)
			if err == 1:
				return (err, err_msg)
		
		if object[which_node] == i['end_node']:
			(err, err_msg) = can_update_end_node(i, tree)
			if err == 1:
				return (err, err_msg)
		
	return (0, 0)



#-----------------------------------------------------------------------#
def can_update_start_node(object, tree):
	"""when the user tries to modify the starting node of a laneset,
	   this function verifies if this modification won't affect
	   any other objects"""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	for i in objects['sources'].values():
		# the laneset being modified cannot be the target laneset for any sources
		if object['laneset_id'] == i['target_laneset']:
			error = "This change cannot be commited because it affects the <font color=red><b>laneset %d</b>"
			error += "</font> and this laneset is being used by the <font color=red><b>source #%d</b></font>."
			return (1, error % (object['laneset_id'], i['source_id']))
	return (0, 0)
	
		

#-----------------------------------------------------------------------#
def can_update_end_node(object, tree):
	"""when the user tries to modify the ending node of a laneset,
	this function verifies if this modification won't affect
	any other objects"""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	for i in objects['sinks'].values():
		# the laneset being modified cannot be the target laneset for any sinks
		if object['laneset_id'] == i['source_laneset']:
			error = "This change cannot be commited because it affects the <font color=red><b>laneset %d</b>"
			error += "</font> and this laneset is being used by the <font color=red><b>sink #%d</b></font>."
			return (1, error % (object['laneset_id'], i['sink_id']))


	return (0, 0)




#-----------------------------------------------------------------------#
def can_delete_laneset(laneset, tree):
	"""when the user tries to delete a laneset, this function verifies if it not
	being used in sources or sinks"""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	laneset_id = laneset['laneset_id']

	for i in objects['sources'].values():
		if i['target_laneset'] == laneset_id:
			err = "This laneset is being used in the <font color=red>source #%s!</font>" % i['source_id']
			return (1, err)
	

	for i in objects['sinks'].values():
		if i['source_laneset'] == laneset_id:
			err = "This laneset is being used in the <font color=red>sink #%s!</font>" % i['sink_id']
			return (1, err)

	return (0, 0)




#-----------------------------------------------------------------------#
def can_delete_section(section, tree):
	"""when the user tries to delete a section, this function verifies if its lanesets
	are not being used in sources or sinks"""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	section_id = section['section_id']

	for i in section['lanesets'].values():
		(error, error_message) = can_delete_laneset(i, tree)
		if error == 1:
			err = "The <font color=red>laneset #%s</font> of this section cannot be " % i['laneset_id']
			err += "deleted. (" + error_message + ")"
			return (1, err)

	return (0, 0)




#-----------------------------------------------------------------------#
def can_delete_street(street, tree):
	"""when the user tries to delete a street, this function verifies if its sections and lanesets
	are not being used in any way"""

	network_id = tree.keys()[0]
	objects = tree[network_id]

	street_id = street['street_id']

	for i in street['sections'].values():
		(error, error_message) = can_delete_section(i, tree)
		if error == 1:
			err = "The <font color=red>section #%s</font> of this street cannot be " % i['section_id']
			err += "deleted. Try to delete this laneset first in order to verify what is going on."
			return (1, err)

	return (0, 0)

	


#-----------------------------------------------------------------------#
def update_object_from_combobox(widget, attribute_name, object, tree):
	"""updates the object, as kept in memory, according to the value of a QT combobox.
	before changing the actual value of the object, this function verifies if the
	modification won't destroy the integrity of other objects"""

	# updates a node, in the tree, according to the value in the combobox
	if widget.name().find('node') != -1:

		current_value = object[attribute_name]
		new_value = widget_text_to_node_id(widget.currentText())

		# do some integrity validation
		if attribute_name.startswith('delimiting_node'):
			if current_value != new_value:
				# trying to update a section delimiting node
				(error, error_message) = can_update_delimiting_node(object, attribute_name, tree)
				if error == 1:
					return(1, error_message)

		elif attribute_name == 'start_node':
			# trying to update a laneset start node
			if current_value != new_value:
				(error, error_message) = can_update_start_node(object, tree)
				if error == 1:
					return(1, error_message)
				
		elif attribute_name == 'end_node':
			if current_value != new_value:
				# trying to update a laneset end node
				(error, error_message) = can_update_end_node(object, tree)
				if error == 1:
					return(1, error_message)

			
		object[attribute_name] = widget_text_to_node_id(widget.currentText())
			
	# updates a laneset, in the tree, according to the value in the combobox
	elif widget.name().find('laneset') != -1:
		object[attribute_name] = widget_text_to_laneset_id(widget.currentText())

	elif widget.name().find('source') != -1:

		if widget.currentItem() == 0:
			object['source_type'] = 'CONSTANT_PROB'
			object['param'] = 1.0
		elif widget.currentItem() == 1:
			object['source_type'] = 'CONSTANT_FLOW'
			object['param'] = 1
		elif widget.currentItem() == 2:
			object['source_type'] = 'VARIABLE_PROB'
			object['param'] = [[1, 1.0]]
		else:
			object['source_type'] = 'VARIABLE_FLOW'
			object['param'] = [[1, 1]]

		editor = widget.parent()
		while editor.parent() != None:
			editor = editor.parent()
		fill_source(editor, widget.parent(), object)
		
	else:

		print "Oops. Trying to update an unknown combobox!", widget.name()

	return (0, 0)




#-----------------------------------------------------------------------#
def fill_combobox(widget, value, object):
	"""fills a QT combobox with the value of an object"""

	widget.clear()
	# inserts the possible nodes into the combobox
	if widget.name().find('node') != -1:
		widget.insertStrList(object['__possible_nodes'])

		index = -1

		# now searches, in the combobox, for the index of the specified node
		for i in range(widget.count()):
			node_id = widget_text_to_node_id(widget.text(i))
			if cmp_ids(node_id, value):
				index = i
				break

		# if the item specified on the tree was not found, choose the first one (ex: if
		# the user changed the delimiting nodes of a section, the nodes of the child
		# lanesets are no longer valid
		if index == -1:
			pass  # probably it was not found because it is being initialized
			
		# and sets the specified node as the current item in the combobox
		widget.setCurrentItem(index)
			
	# inserts the possible lanesets into the combobox
	elif widget.name().find('laneset') != -1:
		widget.insertStrList(object['__possible_lanesets'])

		index = -1
	
		# now searches, in the combobox, for the index of the specified laneset
		for i in range(widget.count()):
			laneset_id = widget_text_to_laneset_id(widget.text(i))
			if cmp_ids(laneset_id, value):
				index = i
				break

		# if the item specified on the tree was not found, choose the first one (this can happen,
		# for ex., if the user changed the node of a source and then the target laneset is
		# not valid anymore
		if index == -1:
			pass  # probably it was not found because it is being initialized

		# and sets the specified laneset as the current item in the combobox
		widget.setCurrentItem(index)
	else:
		print "widget", widget.name(), "not supported!"
			

#-----------------------------------------------------------------------#
def fill_turning_probabilities(editor, widget, turnings, cur_laneset):

	cur_laneset_id = cur_laneset['laneset_id']
	end_node = editor.flat_tree[cur_laneset['end_node']]
	outgoing_lanesets = end_node['__lanesets_out'].keys()

	widget.setNumCols(2)
	widget.setNumRows(len(outgoing_lanesets))

        widget.horizontalHeader().setLabel(0, "Destination")
        widget.horizontalHeader().setLabel(1, "Probability")
	widget.setColumnWidth(0, 250)

	# column "destination" is readonly
	widget.setColumnReadOnly(0, 1)

	(all_nodes_ids, all_nodes_verbose,
	 all_lanesets_ids, all_lanesets_verbose) = all_nodes_and_all_lanesets(editor.tree)

	known_turnings = turnings.keys()

# 	print "len(outgoing)=", len(outgoing_lanesets), outgoing_lanesets, "len(known)=", \
# 	      len(known_turnings), known_turnings

	# check for invalid turnings
	for i in known_turnings:
		if i not in outgoing_lanesets:
			print "Invalid turning!", i, "->", turnings[i], "na laneset", cur_laneset['laneset_id']

	# if there are outgoing lanesets but there is no turning probability, we fill
	# the table with equal probability turnings
	if len(outgoing_lanesets) > 0 and len(known_turnings) == 0:
# 		print "preenchendo equal"
		cur_row = 0
		for l in all_lanesets_verbose:
			laneset_id = l[0]['laneset_id']
			if laneset_id in outgoing_lanesets:
				# fills the laneset name
				widget.setText(cur_row, 0, laneset_data_to_widget_text(l, editor.flat_tree))
				widget.setText(cur_row, 1, str(float(100.0/len(outgoing_lanesets))))
				cur_row = cur_row + 1
	else:
		cur_row = 0
		for l in all_lanesets_verbose:
			laneset_id = l[0]['laneset_id']
			if laneset_id in outgoing_lanesets:
				# fills the laneset name
				widget.setText(cur_row, 0, laneset_data_to_widget_text(l, editor.flat_tree))
				
				# fills the turning probability
				if laneset_id in known_turnings:
					widget.setText(cur_row, 1, str(turnings[laneset_id]))
				else:
					widget.setText(cur_row, 1, str(0.0))
					
				cur_row = cur_row + 1
					


#-----------------------------------------------------------------------#
def fill_source(editor, widget, source):


	fill_widget(editor, widget.child('source_source_id'), source['source_id'], source)
	fill_widget(editor, widget.child('source_first_activation_at'), source['first_activation_at'], source)
	fill_combobox(widget.child('source_located_at_node'), source['located_at_node'], source)
	fill_combobox(widget.child('source_target_laneset'), source['target_laneset'], source)
	fill_widget(editor, widget.child('source_source_activation_frequency'),
		    source['source_activation_frequency'], source)

	widget.child('source_source_variable').setColumnWidth(0, 150)
	
	if source['source_type'] == 'CONSTANT_PROB':
		widget.child('source_source_type').setCurrentItem(0)
		widget.child('param_caption').show()
		widget.child('param_caption').setText("Probability")
		widget.child('source_param').show()
		fill_widget(editor, widget.child('source_param'), source['param'], source)
		widget.child('source_source_variable').hide()
		widget.child('add_source_rule_btn').hide()
		widget.child('del_source_rule_btn').hide()
		
	elif source['source_type'] == 'CONSTANT_FLOW':
		widget.child('source_source_type').setCurrentItem(1)
		widget.child('param_caption').show()
		widget.child('param_caption').setText("Flow")
		widget.child('source_param').show()
		fill_widget(editor, widget.child('source_param'), source['param'], source)
		widget.child('source_source_variable').hide()
		widget.child('add_source_rule_btn').hide()
		widget.child('del_source_rule_btn').hide()
	
	elif source['source_type'] == 'VARIABLE_PROB':
		widget.child('source_source_type').setCurrentItem(2)
		widget.child('param_caption').hide()
		widget.child('source_param').hide()
		widget.child('source_source_variable').show()
		widget.child('source_source_variable').horizontalHeader().setLabel(1, "Probability")
		widget.child('add_source_rule_btn').show()
		widget.child('del_source_rule_btn').show()

		for i in range(widget.child('source_source_variable').numRows()):
			widget.child('source_source_variable').removeRow(0)
		widget.child('source_source_variable').setNumRows(len(source['param']))

		for i in range(len(source['param'])):
			widget.child('source_source_variable').setText(i, 0, str(source['param'][i][0]))
			widget.child('source_source_variable').setText(i, 1, str(source['param'][i][1]))

	elif source['source_type'] == 'VARIABLE_FLOW':
		widget.child('source_source_type').setCurrentItem(3)
		widget.child('param_caption').hide()
		widget.child('source_param').hide()
		widget.child('source_source_variable').show()
		widget.child('source_source_variable').horizontalHeader().setLabel(1, "Flow")
		widget.child('add_source_rule_btn').show()
		widget.child('del_source_rule_btn').show()

		for i in range(widget.child('source_source_variable').numRows()):
			widget.child('source_source_variable').removeRow(0)
		widget.child('source_source_variable').setNumRows(len(source['param']))

		for i in range(len(source['param'])):
			widget.child('source_source_variable').setText(i, 0, str(source['param'][i][0]))
			widget.child('source_source_variable').setText(i, 1, str(source['param'][i][1]))

	else:
		print "Oops. Unknown source type for source", source['source_id']




#-----------------------------------------------------------------------#
def fill_widget(editor, widget, value, object):
	"""fills a QT widget (ie: a LineEdit, or a ComboBox) with the specified value of an object"""


	# if the widget is a combobox, we must fill it with the list of nodes
	if widget.__class__ == qt.QComboBox:
		fill_combobox(widget, value, object)
	

	elif widget.__class__ == qttable.QTable:
		widget_name = widget.name()[widget.name().find('_')+1:]
		if widget_name == "turning_probabilities":
			fill_turning_probabilities(editor, widget, value, object)
			update_tree_from_widget(widget, 'turning_probabilities', object, editor.tree)
		else:
			print "oops, table nao implementada p/", widget_name
		
	elif widget.__class__ == qt.QCheckBox:
		if string.lower(value).find('true') != -1:
			widget.setChecked(1)
		else:
			widget.setChecked(0)
			

	else:
		widget.setText(str(value))


#-----------------------------------------------------------------------#
def system2world(coord, world_width, system_max, system_min, scale):
	"""transforms a coordinate in some arbitrary system of coordinates to a 
	'world' system of coordinates. The parameters are: the coordinate to
	be transformed, the width of the world system (=difference between its maximum
	coordinate and its minimum coordinate), the max and min coordinates of the arbitrary
	system and the scale (=ratio of the world 'area' we wish to use)"""

	return (world_width * scale/2.0) + \
	       (coord - system_min)/(system_max-system_min)*(world_width*scale)



#-----------------------------------------------------------------------#
def other_end(x, y, angle):
	"""calculates the other end of a line, given a starting point and an angle"""
	distance = 1000

	other_x = math.cos(angle) * distance
	other_y = math.sin(angle) * distance
	
	return (x + other_x, y - other_y)


#-----------------------------------------------------------------------#
def fill_signalplan(widget, tree, flat_tree, parent_tlight, signalplan, drawing_frame):
	"""fills the widgets for a given signalplan. This envolvs drawing
	the streets so that the user can specify the phases, etc."""

	network_id = tree.keys()[0]
	objects = tree[network_id]
	parents_of_lanesets = {} # a mapping of laneset -> parent section
	
	for street_properties in objects['streets'].values():
		for section_properties in street_properties['sections'].values():
			for laneset_properties in section_properties['lanesets'].values():
				parents_of_lanesets[laneset_properties['laneset_id']] = section_properties

	node = get_element_by_id(flat_tree, parent_tlight['located_at_node'])


	sections = []
	connected_sections = []
	
	for i in node['__lanesets_in']:
		if parents_of_lanesets[i] not in sections:
			sections.append(parents_of_lanesets[i])
	for i in node['__lanesets_out']:
		if parents_of_lanesets[i] not in sections:
			sections.append(parents_of_lanesets[i])
	for i in sections:
		n1 = get_element_by_id(flat_tree, i['delimiting_node_1'])
		n2 = get_element_by_id(flat_tree, i['delimiting_node_2'])
		if n1 == node:
			other_node = n2
		else:
			other_node = n1
	
		normalized_coordinates = [float(other_node['x_coord']) - float(node['x_coord']),
					  float(other_node['y_coord']) - float(node['y_coord'])]
# 		print "->", float(other_node['x_coord']), float(node['y_coord']), normalized_coordinates

		# the width of the section is the total number of lanes in it
		section_width = 0
		for p in i['lanesets'].values():
			section_width += len(p['lanes'].keys())

		# angle, width, section_ref
		conn_section = connected_section(slope(normalized_coordinates[0],
						       normalized_coordinates[1]),
						 section_width, i)
		connected_sections.append(conn_section)


	# sorts the sections that are connected to a node according to their angles
	connected_sections.sort()
# 	for i in connected_sections:
# 		print "connected da:", i.section_ref['section_id'], "=", i.angle

	# calculate the minimum distance that the sections must be pushed far away
	# from the node in order to avoid overlapping
	displacements = []
	for i in range(len(connected_sections)-1):
		displacements.append(minimum_displacement(connected_sections[i+1], connected_sections[i]))
	displacements.append(minimum_displacement(connected_sections[-1], connected_sections[0]))
	needed_displacement = max(displacements)
# 	print "needed displacement=", needed_displacement

	x_coords = []
	y_coords = []
	# adapt all sections so they are pushed far away from the node by the minimum required
	# distance to avoid them from overlapping
	for i in connected_sections:
		i.update(needed_displacement)
		x_coords.extend([i.left_corner_x, i.right_corner_x])
		y_coords.extend([i.left_corner_y, i.right_corner_y])
	min_x = min(x_coords)
	max_x = max(x_coords)
	min_y = min(y_coords)
	max_y = max(y_coords)

	ratio_of_canvas = 0.5  # portion of the canvas' area that will be used for drawing (< 1.0!)
	# transforms all coordinates to the "world" system of coordinates (the canvas system)
	for i in connected_sections:
		i.left_corner_x = system2world(i.left_corner_x, drawing_frame.width(), max_x, min_x, ratio_of_canvas)
		i.right_corner_x = system2world(i.right_corner_x, drawing_frame.width(), max_x, min_x, ratio_of_canvas)
		# para inverter (flip) vertical, trocar o max por min e vice-versa
		i.left_corner_y = system2world(i.left_corner_y, drawing_frame.height(), min_y, max_y, ratio_of_canvas)
		i.right_corner_y = system2world(i.right_corner_y, drawing_frame.height(), min_y, max_y, ratio_of_canvas)
		i.x = (i.right_corner_x+i.left_corner_x)/2
		i.y = (i.right_corner_y+i.left_corner_y)/2
		

	drawing_frame.parent().canvas = qtcanvas.QCanvas(drawing_frame.parent())
	drawing_frame.parent().canvas.resize(drawing_frame.width(), drawing_frame.height())

	drawing_frame.current_signalplan = signalplan

	drawing_frame.setCanvas(drawing_frame.parent().canvas)
	drawing_frame.canvas().setBackgroundColor(qt.QColor(219, 255, 235))
	drawing_frame.setVScrollBarMode(qtcanvas.QCanvasView.AlwaysOff)
	drawing_frame.setHScrollBarMode(qtcanvas.QCanvasView.AlwaysOff)

	paint_streets_at_node(connected_sections, drawing_frame, node)


	# fills the widgets

	phases_box = widget.child("available_phases")
	phases_box.clear()
	phases = [i for i in signalplan['phases'].keys()]
	phases.sort()
	phases_box.insertStrList([str(i) for i in phases])
	phases_box.setSelected(0, 1)
	widget.child("signalplan_signalplan_id").setText(str(signalplan['signalplan_id']))

	widget.child("signalplan_additional_info").setText(str(signalplan['additional_info']))


#-----------------------------------------------------------------------#
def paint_streets_at_node(connected_sections, drawing_frame, node):
	
	# resets the scale to 100% (normal view)
	scale_matrix = qt.QWMatrix()
	scale_matrix = scale_matrix.scale(1.0, 1.0)
	drawing_frame.setWorldMatrix(scale_matrix)

	pen = qt.QPen(qt.QColor(0, 108, 130), 5, qt.Qt.SolidLine)
	brush_crossing = qt.QBrush(qt.QColor(0, 108, 13))
	brush_crossing.setStyle(qt.Qt.Dense5Pattern)

	crossing_edges = qt.QPointArray(len(connected_sections)*2)

	count = 0
	for i in connected_sections:
		paint_lanesets(i, node, drawing_frame)

		# tooltips on canvas!
		# http://poa.berlios.de/gcover/canvasview.cpp.html
	
		# adds another edge to the crossing polygon
		if (count %2) == 1:
			crossing_edges.setPoint(count, i.left_corner_x, i.left_corner_y)
			count += 1
			crossing_edges.setPoint(count, i.right_corner_x, i.right_corner_y)
			count += 1
		else:
			crossing_edges.setPoint(count, i.right_corner_x, i.right_corner_y)
			count += 1
			crossing_edges.setPoint(count, i.left_corner_x, i.left_corner_y)
			count += 1

		l = qtcanvas.QCanvasLine(drawing_frame.canvas())
		l.setPoints(i.left_corner_x, i.left_corner_y, i.right_corner_x, i.right_corner_y)
		l.setPen(pen)
		l.show()



	crossing = qtcanvas.QCanvasPolygon (drawing_frame.canvas())
	crossing.setPoints(crossing_edges)
	crossing.setPen(pen)
	crossing.setBrush(brush_crossing)
	crossing.show()

	drawing_frame.canvas().update()



#-----------------------------------------------------------------------#
def paint_lanesets(section, node, drawing_frame):
	def compare_lanesets(a, b):
		if a['laneset_position'] < b['laneset_position']:
			return -1
		else:
			return 1

	def compare_lanesets_rev(a, b):
		if a['laneset_position'] < b['laneset_position']:
			return 1
		else:
			return -1

	brush_laneset = qt.QBrush(qt.QColor(125, 125, 125))
	
	
# 	print "pintando lanesets da section", section.section_ref['section_id'], "ligada no nodo", node['node_id']
# 	print "coordenadas que delimitam a section: left=(%f, %f) right=(%f, %f)" % \
# 	      (section.left_corner_x, section.left_corner_y, section.right_corner_x, section.right_corner_y)

	number_of_lanesets = len(section.section_ref['lanesets'].keys())
	total_number_of_lanes = section.width # the section width is the total number of lanes

	# each lane has lane_inc_x pixels width and lane_inc_y height
	lane_inc_x = (section.right_corner_x - section.left_corner_x) / total_number_of_lanes
	lane_inc_y = (section.right_corner_y - section.left_corner_y) / total_number_of_lanes
	
# 	print "   ela possui", number_of_lanesets, "lanesets! (#lanes total", total_number_of_lanes, ")"
# 	print "   lane_inc_x=", lane_inc_x, "lane_inc_y", lane_inc_y

	lanesets = [i for i in section.section_ref['lanesets'].values()]

	for ls in lanesets:
		if ( (ls['end_node'] > node['node_id']) or (ls['start_node'] > node['node_id']) ):
			lanesets.sort(compare_lanesets)
		else:
			lanesets.sort(compare_lanesets_rev)

	x_start = section.left_corner_x
	y_start = section.left_corner_y


	for ls in lanesets:
		print_divisory_line(x_start, y_start, section.angle, drawing_frame)
		
		brush = 0

		#print "\tstrt_nd: ",ls['start_node']

		if ls['end_node'] == node['node_id']:
			#print "\t>>>>node_id: ",node['node_id'], "\tstrt_nd: ", ls['start_node'], "\tend_nd:", ls['end_node'] ,"(", ls['laneset_position'], ")"
  			#print "\tlaneset entrando:", ls['laneset_id'], "(ordem", ls['laneset_position'], ")"

			for l in ls['lanes'].values():
# 				print "\t\tlane %d (%f, %f)->(%f, %f)" % (l['lane_id'], x_start, y_start,
# 									  x_start+lane_inc_x, y_start+lane_inc_y)
				print_lane(x_start, y_start, x_start+lane_inc_x, y_start+lane_inc_y,
					   section.angle, l, drawing_frame, brush)
				x_start += lane_inc_x
				y_start += lane_inc_y
				brush ^= 1
			
		else:
 			#print "\tlaneset saindo:", ls['laneset_id'], "(ordem", ls['laneset_position'], ")"
			lanes_in_laneset = len(ls['lanes'].keys())

			(other_left_x, other_left_y) = other_end(x_start, y_start, section.angle)
			(other_right_x, other_right_y) = other_end(x_start + lane_inc_x*lanes_in_laneset,
								   y_start + lane_inc_y*lanes_in_laneset,
								   section.angle)
			
			laneset_edges = qt.QPointArray(5)
			laneset_edges.setPoint(0, other_left_x, other_left_y)
			laneset_edges.setPoint(1, x_start, y_start)
			laneset_edges.setPoint(2, x_start + lane_inc_x*lanes_in_laneset,
					       y_start + lane_inc_y*lanes_in_laneset)
			laneset_edges.setPoint(3, other_right_x, other_right_y)		
			laneset_edges.setPoint(4, other_left_x, other_left_y)

			laneset_draw = my_qcanvaspolygon (drawing_frame.canvas(), ls,
							  x_start+(lane_inc_x*lanes_in_laneset)/2,
							  y_start+(lane_inc_y*lanes_in_laneset)/2)
			laneset_draw.setPoints(laneset_edges)
			laneset_draw.setBrush(brush_laneset)
			laneset_draw.show()

			x_start += lane_inc_x*lanes_in_laneset
			y_start += lane_inc_y*lanes_in_laneset
		
		#x_start += lane_inc_x*lanes_in_laneset
		#y_start += lane_inc_y*lanes_in_laneset
			
	print_divisory_line(x_start, y_start, section.angle, drawing_frame)



#-----------------------------------------------------------------------#
def print_divisory_line(x, y, angle, drawing_frame):
	pen = qt.QPen(qt.QColor(220, 250, 120), 4, qt.Qt.DashLine)
	divisory_line = qtcanvas.QCanvasLine(drawing_frame.canvas())

	(end_x, end_y) = other_end(x, y, angle)
	divisory_line.setPoints(x, y, end_x, end_y)

	divisory_line.setZ(1.0) # in order to appear above all the rest
	divisory_line.setPen(pen)
	divisory_line.show()
	


#-----------------------------------------------------------------------#
def print_lane(left_corner_x, left_corner_y, right_corner_x, right_corner_y,
		lane_angle, lane_ref, drawing_frame, brush):

	if brush == 0:
		brush_lane = qt.QBrush(qt.QColor(100, 150, 125))
	else:
		brush_lane = qt.QBrush(qt.QColor(80, 115, 110))


# 	print "\t\t\tpintando lane (%f, %f)->(%f, %f) #%d" % (left_corner_x, left_corner_y,
# 							       right_corner_x, right_corner_y, lane_ref['lane_id'])

	(other_left_x, other_left_y) = other_end(left_corner_x, left_corner_y, lane_angle)
	(other_right_x, other_right_y) = other_end(right_corner_x, right_corner_y, lane_angle)
	
	lane_edges = qt.QPointArray(5)
	lane_edges.setPoint(0, other_left_x, other_left_y)
	lane_edges.setPoint(1, left_corner_x, left_corner_y)
	lane_edges.setPoint(2, right_corner_x, right_corner_y)		
	lane_edges.setPoint(3, other_right_x, other_right_y)		
	lane_edges.setPoint(4, other_left_x, other_left_y)

	lane_draw = my_qcanvaspolygon (drawing_frame.canvas(), lane_ref,
				       left_corner_x+(right_corner_x-left_corner_x)/2,
				       left_corner_y+(right_corner_y-left_corner_y)/2)
	lane_draw.setPoints(lane_edges)
	lane_draw.setBrush(brush_lane)
	lane_draw.show()



#-----------------------------------------------------------------------#
def update_directions(signalplan, current_phase, start_obj, end_obj):
	if not start_obj.has_key('lane_id') or not end_obj.has_key('laneset_id'):
		return
	from_lane = start_obj['lane_id']
	to_laneset = end_obj['laneset_id']
	
# 	print "atualizando direcao de", from_lane, "para", to_laneset, "(phase=", current_phase, ")"

	directions = signalplan['phases'][current_phase]['directions']

	procura = [from_lane, to_laneset]
# 	pretty_printer.pprint(directions)

	try:
		directions.index(procura)
	except ValueError:
# 		print "-- nao encontrou.. adicionando", procura
		directions.append(procura)
	else:
# 		print "-- achou.. removendo", procura
		directions.remove(procura)
	


#-----------------------------------------------------------------------#
# given a line, calculate the polygon for an arrow which will be placed in the end of the line
def calculate_arrow(start_x, start_y, end_x, end_y, arrow_side_length, canvas):


	arrow_points = qt.QPointArray(4)
	
	line_length = math.hypot( (end_y - start_y), (end_x - start_x) )
	angle = math.atan2(float(end_y-start_y), float(end_x-start_x))
		
	arrow_points.setPoint(0, 0, 0)

	(x,y) = rotate(-arrow_side_length, -arrow_side_length/1.5, angle)
	arrow_points.setPoint(1, x, y)

	(x,y) = rotate(-arrow_side_length, +arrow_side_length/1.5, angle)
	arrow_points.setPoint(2, x, y)

	(x,y) = rotate(-arrow_side_length, +arrow_side_length/1.5, angle)
	arrow_points.setPoint(3, x, y)
	
	arrow_points.translate(start_x+(end_x-start_x)*2.0/3.0, start_y+(end_y-start_y)*2.0/3.0)
	
	arrow_polygon = qtcanvas.QCanvasPolygon(canvas)
	arrow_polygon.setPoints(arrow_points)
	arrow_polygon.setZ(1.5)

	return arrow_polygon
	
	

#-----------------------------------------------------------------------#
def get_element_by_id(flat_tree, id):
	return flat_tree.get(id, None)

