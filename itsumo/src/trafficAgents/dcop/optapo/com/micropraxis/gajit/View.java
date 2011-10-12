// View
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

import java.io.Serializable;

/**
 * Classes derived from View are used to impose some interpretation on a
 * chromosome.  The default view (as provided by this base class), simply
 * treats the chromosome as a string of single bit genes, each encoding a
 * boolean.  More useful interpretations can be encoded by subclasses.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class View implements Serializable
{
    public View()
    {
    }

    /**
     * Get the value of gene <code>index</code>.  The default implementation
     * simply regards the chromosome as n single bit genes and returns the
     * bit value at the given offset encoded as a Boolean.  It is intended
     * to be overridden by subclasses.
     */
    public Object getGene( Chrom chrom, int index )
    {
        return new Boolean( chrom.get( index ) );
    }

    /**
     * Return the number of genes.  The default is the same as the number of
     * bits.
     */
    public int size( Chrom chrom ) 
    { 
        return chrom.size();
    }

    /**
     * Returns a stringified representation of a chromosome as viewed
     * through this view.
     */
    public String toString( Chrom chrom )
    {
        return chrom.toString();
    }
}
