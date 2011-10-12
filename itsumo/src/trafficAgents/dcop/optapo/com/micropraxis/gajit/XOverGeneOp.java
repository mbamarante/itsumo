// XOverGeneOp
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
 * A class that represents a binary genetic operator that performs a single
 * gene crossover between two chromosomes.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class XOverGeneOp extends GenOp
{
    /**
     * Length of a gene.
     */
    protected int len;

    public XOverGeneOp( int geneLength )
    {   
        super( true );
        len = geneLength;
    }

    public Chrom apply( Chrom chrom1, Chrom chrom2 )
    {
        Chrom shorter;
        
        if (chrom1.size() < chrom2.size())
            shorter = chrom1;
        else
            shorter = chrom2;

        return applyAt( selectGene( shorter, len ), chrom1, chrom2 );
    }

    public Chrom applyAt( int index, Chrom chrom1, Chrom chrom2 )
    {
        index -= index % len;

        return (Chrom) chrom1.swapSubSet( index, len, chrom2 );
    }
}
