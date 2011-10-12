// GenOp
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

import com.micropraxis.util.RandomInt;

/**
 * A class that represents a genetic operator that can manipulate a
 * chromosome.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class GenOp
{
    protected boolean binary;

    public GenOp( boolean isBinaryOp )
    {
        binary = isBinaryOp;
    }

    public Chrom apply( Chrom chrom1 )
    {
        return chrom1;
    }
    
    public Chrom apply( Chrom chrom1, Chrom chrom2 )
    {
        return apply( chrom1 );
    }

    public Chrom applyAt( int index, Chrom chrom1 )
    {
        return chrom1;
    }

    public Chrom applyAt( int index, Chrom chrom1, Chrom chrom2 )
    {
        return applyAt( index, chrom1 );
    }

    /**
     * Whether or not the operator is binary
     */
    public boolean isBinaryOp()
    {
        return binary;
    }

    protected static int compNumGenes( Chrom chrom, int geneLength )
    {
        return (chrom.size() - 1) / geneLength + 1;
    }

    protected static int selectGene( Chrom chrom, int geneLength )
    {
        int numGenes = compNumGenes( chrom, geneLength );
        int selected = RandomInt.next( numGenes );

        return selected * geneLength;
    }
}
