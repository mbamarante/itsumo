#!/usr/bin/python

from math import *
import sys, os

error = 0.1

def radians(n):
    return (2.0*pi*n)/360.0

def slope(x, y):
    """calculates the slope, in degrees, of the [x y] vector against the origin"""

#     print "........... calculando inclinacao do vetor", x, y
    
    i = atan2(y, x) * 180.0 / pi
#     print "................... atan2=", i
    if i < 0:
#         print "......................... opa, eh menor que zero.. somando 360"
        i += 360
#     print "..-> resultado", i
    return i

class point:
    def __init__(self, x, y):
        self.x = x
        self.y = y
class segment:
    def __init__(self, start, end):
        self.start = start
        self.end = end

def intersection(s1, s2):
    """returns (a, b), where a=1 if there the two segments intersect, 0 otherwise;
                             b=1 if there was no intersection but the segments still need to be
                                 pushed away from each other"""
    x1=s1.start.x
    y1=s1.start.y
    x2=s1.end.x
    y2=s1.end.y
    x3=s2.start.x
    y3=s2.start.y
    x4=s2.end.x
    y4=s2.end.y
    
    denominator = (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1)
    
    # parallel
    if abs(denominator) <= error:
        # 	print "PARALLEL!"
        return (0, 1)
 
    ua = (x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3)
    ua /= denominator;

    ub= (x2 - x1)*(y1 - y3) - (y2 - y1)*(x1 - x3)
    ub/= denominator

    x = x1 + ua*(x2 - x1)
    y = y1 + ua*(y2 - y1)

    if 0.0 <= ua <= 1.0:
        if 0.0 <= ub <= 1.0:
            #             print "INSIDE BOTH"
            return (1, 0)
        else:
            #             print "INSIDE FIRST"
            return (1, 0)
    else:
        if 0.0 <= ub <= 1.0:
            #             print "INSIDE SECOND"
            return (1, 0)
        else:
            #             print "INSIDE NONE"
            return (0, 0)
        

class xyz:
    pass

class connected_section:
    def __init__(self, angle, width, section_ref):
        """a connected_section is an object which stores informations about a given section which
        are necessary for the correct drawing of the section. These informations are, for example,
        the angle of the section against the x-axis, its width and the reference to the actual section
        object"""
        self.angle = radians(angle)
        self.width = width
        self.section_ref = section_ref

    def calculate_section_coordinates(self, displacement):
        """if we want the starting point of a section to have a given displacement
        regarding the node of the crossing, then we use this function to calculate the displaced
        coordinates of the section's starting point"""
        self.x = cos(self.angle) * displacement
        self.y = sin(self.angle) * displacement

    def calculate_corners(self, displacement):
        """given that the starting point of a section has some displacement regarding the node
        of the crossing, we can calculate the points which determine its corners based on the
        section's width"""
        self.left_corner_x = self.x - self.width/2.0 * sin(self.angle)
        self.left_corner_y = self.y + self.width/2.0 * cos(self.angle)
        self.right_corner_x = self.x + self.width/2.0 * sin(self.angle)
        self.right_corner_y = self.y - self.width/2.0 * cos(self.angle)

       
    def update(self, displacement):
        """updates the coordinates of the starting point of the section, as well as the
        coordinates for its corners, given that the section should have a given displacement
        regarding the node position"""
        self.calculate_section_coordinates(displacement)
        self.calculate_corners(displacement)

    def __cmp__(self, other):
        return cmp(self.angle, other.angle)

def minimum_displacement(section1, section2):
    """calculates the minimum displacement that these sections must have regarding the
    origin (0,0) so that they do not intersect.
    
    restriction: angle(section1) > angle(section2)"""


    radius = 0.1
    step = 0.05

    if section1.angle <= section2.angle:
        print "MORRE", section1.angle, section2.angle
        return


    while 1:
        section1.update(radius)
        section2.update(radius)
        
        (intersect, correct) = intersection(segment(point(section1.left_corner_x, section1.left_corner_y),
                                                    point(section1.right_corner_x, section1.right_corner_y)),
                                            segment(point(section2.left_corner_x, section2.left_corner_y),
                                                    point(section2.right_corner_x, section2.right_corner_y)))
        if not intersect:
#             print "\n\n---------------------------------------------\n"
#             if corrige:
#                 print "corrigindo!"
#                 section1.x += 15
#                 section1.calcula_esquinas(15)
#                 section2.x += 15
#                 section2.calcula_esquinas(15)
                
            return radius
        radius += step
#         print ".",

if __name__ == "__main__":


    for i in range(0, 360, 5):
        for j in range(i+1, 360, 5):
            r0 = connected_section(i, 7, None)
            r1 = connected_section(j, 3, None)

            raio = minimum_displacement(r1, r0)
            
            fp = open('fixo', "w")
            fp.write("0\t0\n")
            fp.write("%s\t%s\n" % (r0.x, r0.y))
            fp.close()
            
            fp = open('esquina_esquerda_fixo', "w")
            fp.write("%s\t%s\n" % (r0.x, r0.y))
            fp.write("%s\t%s\n" % (r0.left_corner_x, r0.left_corner_y))
            fp.close()
            
            fp = open('esquina_direita_fixo', "w")
            fp.write("%s\t%s\n" % (r0.x, r0.y))
            fp.write("%s\t%s\n" % (r0.right_corner_x, r0.right_corner_y))
            fp.close()

            fp = open('ponto', "w")
            fp.write("0\t0\n")
            fp.write("%s\t%s\n" % (r1.x, r1.y))
            fp.close()
        
            fp = open('esquina_esquerda', "w")
            fp.write("%s\t%s\n" % (r1.x, r1.y))
            fp.write("%s\t%s\n" % (r1.left_corner_x, r1.left_corner_y))
            fp.close()
            
            fp = open('esquina_direita', "w")
            fp.write("%s\t%s\n" % (r1.x, r1.y))
            fp.write("%s\t%s\n" % (r1.right_corner_x, r1.right_corner_y))
            fp.close()
            
            cmd = 'echo \'set grid xtics ytics; set xtics 5; set ytics 5;'
            cmd += ' plot ' + '"' + 'ponto' + '" with linespoints ps 2;'
            cmd += ' replot ' + '"' + 'esquina_esquerda' + '" with linespoints lw 3 ps 2;'
            cmd += ' replot ' + '"' + 'esquina_direita' + '" with linespoints lw 3 ps 2;'
            cmd += ' replot ' + '"' + 'fixo' + '" with linespoints lw 3 ps 2;'
            cmd += ' replot ' + '"' + 'esquina_esquerda_fixo' + '" with linespoints lw 3 ps 2;'
            cmd += ' replot ' + '"' + 'esquina_direita_fixo' + '" with linespoints lw 3 ps 2\''
            
            cmd += ' | gnuplot -persist'
            print cmd
            
            os.system(cmd)
            x = raw_input("(%s)  %s, %s" % (raio, i, j))

            print "%s\t\t%s\t%s" % (raio, i, j)
