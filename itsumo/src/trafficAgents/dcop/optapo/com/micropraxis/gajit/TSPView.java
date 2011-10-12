// TSPView
// Copyright (c) 1998-2003 Matthew Faupel
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library Public License for more details.
//
// You should have received a copy of the GNU Library Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// This Java class is inspired in part by the GAGS C++ library which I
// presume to be copyright J. J. Merelo <jmerelo@kal-el.ugr.es>.

package com.micropraxis.gajit;

/**
 * Travelling Salesperson View.  A view that interprets chromosomes as a
 * string of unique values in the range 0 to n-1.  The gene size is dictated
 * by the number of bits required to represent n-1; the chromosome is
 * assumed to be of size n * genesize.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class TSPView extends View 
{
    /**
     * Number of cities
     */
    protected int numCities;

    /**
     * Number of bits needed to represent a city id.
     */
    private int bitsRep;

    public TSPView( int cityCount )
    {
        numCities = cityCount;
        if (cityCount > 1)
        {
            --cityCount;
            for (bitsRep = 31; (cityCount <<= 1) > 0; --bitsRep)
                continue;
        }
    }

    /**
     * Get the value of gene <code>index</code>.  The result is a Long
     * holding the long value encoded by that gene, or null if the index is
     * out of range.
     */
    public Object getGene( Chrom chrom, int index )
    {
	if (index > 0 && index < numCities)
	    return new Long( 
                chrom.getLongAt( bitsRep * index, bitsRep ) % numCities );
        else
	    return null;
    }

    /**
     * Return the number of genes.
     */
    public int size( Chrom chrom )
    {
        return numCities; 
    }

    /**
     * Returns a stringified representation of a chromosome as viewed
     * through this view.
     */
    public String toString( Chrom chrom )
    {
        StringBuffer buf = new StringBuffer();

        buf.append( getGene( chrom, 0 ) );
        for (int i = 1; i < size( chrom ); ++i)
        {
            buf.append( "," );
            buf.append( getGene( chrom, i ) );
        }
        return buf.toString();
    }
}
