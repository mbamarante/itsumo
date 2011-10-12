// VarView
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
 * A view that interprets chromosomes as a certain number of variable sized
 * genes, each of which encodes (a possibly truncated) long value.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class VarView extends View
{
    protected int [] geneBounds;

    public VarView()
    {
    }

    /**
     * Initialise a view that interprets chromosomes as a certain number of
     * variable sized genes, each of which encodes (a possibly truncated)
     * long value.  geneBoundaries encodes the start positions of each gene,
     * sorted in ascending order, with the last element being the end offset
     * of the last gene + 1, i.e. an array of size n configures n-1 genes.
     */
    public VarView( int [] geneBoundaries )
    {
        geneBounds = new int[geneBoundaries.length];

        System.arraycopy( geneBoundaries, 0, geneBounds, 0,
            geneBoundaries.length );
    }

    /**
     * Get the value of gene <code>index</code>.  The result is a Long
     * holding the long value encoded by that gene, or null if the index is
     * out of range.
     */
    public Object getGene( Chrom chrom, int index )
    {
	if ( index >= 0 && index < geneBounds.length - 1)
	    return new Long( chrom.getLongAt( geneBounds[index],
                geneBounds[index+1] - geneBounds[index] ) );
        else
	    return null;
    }

    /**
     * Return the number of genes.
     */
    public int size( Chrom chrom )
    { 
        return geneBounds.length - 1;
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
