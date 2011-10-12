// FixView
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
 * A view that interprets chromosomes as a (possibly variable length) string
 * of fixed size genes, encoding floating point values in a given range.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class FixView extends View
{
    protected int geneSize;
    protected double start;
    protected double scale;

    /**
     * Define the view, indicating the size of each gene.  The value
     * returned from the gene will be a Double in the range
     * <code>start</code> to <code>start + range</code>.
     */
    public FixView( int geneSize, double start, double range )
    {
        this.geneSize = geneSize;
        this.start = start;

        // Compute max value for that gene size and from that derive a
        // scaling factor that will ensure that all returned values fall
        // within the required range.

        this.scale = range / (double) ((1L << geneSize) - 1);
    }

    /**
     * Get the value of gene <code>index</code>.  The result is a Double
     * holding the value encoded by that gene interpreted to fall within the
     * required range.
     */
    public Object getGene( Chrom chrom, int index )
    {
	return new Double( chrom.getLongAt( geneSize * index, geneSize ) *
            scale + start );
    }

    /**
     * Return the number of genes.
     */
    public int size( Chrom chrom )
    { 
        return chrom.size() / geneSize;
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
