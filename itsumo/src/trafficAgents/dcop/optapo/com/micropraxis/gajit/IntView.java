// IntView
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
 * of fixed size genes, encoding integer numeric values.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class IntView extends View
{
    protected int geneSize;
    protected boolean signed;
    protected long signMask;
    protected long extendMask;

    /**
     * Define the view, indicating the size of each gene and whether or not
     * the values encoded in the genes should be interpreted as signed.
     */
    public IntView( int geneSize, boolean signed )
    {
        if (geneSize > 64)
            geneSize = 64;
        else if (geneSize < 1)
            geneSize = 1;

        this.geneSize = geneSize;
        this.signed = signed;

        if (signed)
        {
            signMask = 1 << (geneSize-1);
            extendMask = 0x8000000000000000L >> (64-geneSize);
        }
    }

    /**
     * Get the value of gene <code>index</code>.  The result is a Long
     * holding the value encoded by that gene.
     */
    public Object getGene( Chrom chrom, int index )
    {
        long retValue = chrom.getLongAt( geneSize * index, geneSize );

        if (signed && (retValue & signMask) != 0)
            retValue |= extendMask;

	return new Long( retValue );
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
