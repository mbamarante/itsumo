#!/usr/bin/python

import sys, os

map = {'0': 'QSizePolicy.Fixed,',
       '1': 'QSizePolicy.Minimum,',
       '2': 'QSizePolicy.Ignored,',
       '3': 'QSizePolicy.MinimumExpanding,',
       '4': 'QSizePolicy.Maximum,',
       '5': 'QSizePolicy.Preferred,',
       '6': 'SEI-LAH!,',
       '7': 'QSizePolicy.Expanding,' }

ui = sys.argv[1]
name_in_file = ui.replace('ui', 'py')
name_out_file = '%s.tmp' % name_in_file

os.system('pyuic %s -o %s' % (ui, name_in_file))

in_file = open(name_in_file, 'r')
out_file = open(name_out_file, 'w')

lines = in_file.readlines()

for line in lines:
    first_spot_start = line.find('QSizePolicy(')
    if first_spot_start != -1:
        first_spot_start += 12
        first_spot_end = line.find(',', first_spot_start)
        len_first_spot = first_spot_end - first_spot_start

	print "ae1", line[first_spot_start:first_spot_end]
	print "ae2", map[line[first_spot_start:first_spot_end]]
        line = line[:first_spot_start] + map[line[first_spot_start:first_spot_end]] + line[first_spot_end+len_first_spot:]
#         print "1) (%d, %d, %d) >%s<" %  (first_spot_start, first_spot_end, len_first_spot, line)

        second_spot_start = line.find(',', first_spot_end+len_first_spot)
        if second_spot_start != -1:
            second_spot_start += 1
            second_spot_end = line.find(',', second_spot_start)
            len_second_spot = second_spot_end - second_spot_start
            
            line = line[:second_spot_start] + map[line[second_spot_start:second_spot_end]] + line[second_spot_end+len_second_spot:]
#             print "2) (%d, %d, %d) >%s<" %  (second_spot_start, second_spot_end, len_second_spot, line)
            
    out_file.write(line)

in_file.close()
out_file.close()

os.remove(name_in_file)
os.rename(name_out_file, name_in_file)
