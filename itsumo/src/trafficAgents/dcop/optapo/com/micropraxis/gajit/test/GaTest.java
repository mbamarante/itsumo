// GaTest
// Copyright (c) 1998-2003 Matthew Faupel
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// This Java class is inspired in part by the GAGS C++ library which I
// presume to be copyright J. J. Merelo <jmerelo@kal-el.ugr.es>.

package com.micropraxis.gajit.test;

import com.micropraxis.gajit.Chrom;
import com.micropraxis.gajit.ChromItem;
import com.micropraxis.gajit.DupOp;
import com.micropraxis.gajit.FixView;
import com.micropraxis.gajit.IntView;
import com.micropraxis.gajit.Population;
import com.micropraxis.gajit.View;
import com.micropraxis.util.Iterator;
import com.micropraxis.util.ListIterator;
import com.micropraxis.util.SortedList;

/**
 * A basic genetic algorithm implementation to test the GAJIT package.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class GaTest
{
    static final int NUMGENES = 10;
    static final int SIZEGENES = 3;
    static final int GENERATIONS = 100;
    static final int NUMCHROMS = 200;

    public static void main( String []  args )
    {
        Population people = new Population( NUMCHROMS, SIZEGENES, NUMGENES,
            NUMGENES, 1.0/(SIZEGENES*NUMGENES) );
        View vista = new FixView( SIZEGENES, 0.0, 7.0 );
        DupOp adder = new DupOp( SIZEGENES );

        people.setEliteRate( 0.05 );
        people.setCullRate( 0.4 );
        people.addOp( adder, 0.1 );

        /*for (Iterator censo = people.iterator(); censo.hasNext(); )
            System.out.println( censo.next() );*/

        for (int i = 0; i < GENERATIONS; ++i)
        {
            Iterator censo;

           /* if (i % 10 == 0)
            {
                censo = people.iteratorAll();
                System.out.println( "Resetting fitness values" );
            }
            else*/
                censo = people.iteratorNewOnly();
	
            while (censo.hasNext())
            {
                double val = 0.0;
                ChromItem tmp = (ChromItem) censo.next();
                Chrom chrom = tmp.getChrom();
                int geneCount = vista.size( chrom );                
                //System.out.println( tmp );
                for (int j = 0; j < NUMGENES; ++j)
                    val -= ((Double) vista.getGene( chrom, j )).
                        doubleValue();

                tmp.setFitness( val );
            }
            if (i < GENERATIONS - 1)
                people.newGeneration();
        }

        SortedList results = new SortedList( people );
        ListIterator iter = results.listIterator( results.size() );

        //while (iter.hasPrevious())
        //{
            ChromItem item = (ChromItem) iter.previous();
            Chrom chrom = item.getChrom();

            for (int i = 0; i < NUMGENES; ++i)
                System.out.print(vista.getGene( chrom, i ) + " " );

            System.out.println( " = " + item.getDoubleFitness() );
       // }
    }
}
