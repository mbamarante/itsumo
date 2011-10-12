// XOverPtOp
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
 * A class that represents a binary genetic operator that performs an n
 * point crossover between two chromosomes. Gene boundaries are not
 * respected. 
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class XOverPtOp extends GenOp
{
    protected int [] xOverPts;

    public XOverPtOp( int numPts )
    {
        super( true );

        // Two extra elements to hold zero at the beginning and max length
        // at the end to provide fixed endpoints to the crossovers.

        xOverPts = new int[((numPts >= 1) ? numPts : 1) + 2];
        xOverPts[0] = 0;
    }

    public Chrom apply( Chrom chrom1, Chrom chrom2 )
    {
        // Compute minimum length

        int minLen = chrom1.size();

        if (chrom2.size() < minLen)
            minLen = chrom2.size();

        xOverPts[xOverPts.length-1] = minLen - 1;

        // Generate crossover points. The chromosomes is divided more or
        // less regularly

        int divLen = minLen / ( xOverPts.length - 1);

        for (int i = 1; i < xOverPts.length - 1; ++i)
        {
            // To avoid making 2 points the same

            xOverPts[i] = xOverPts[i-1] + 1 +
                RandomInt.next( (i+1)*divLen - 1 - xOverPts[i-1] );
        }

        // Do xOver

        for (int i = 1; i <= xOverPts.length - 1; i += 2)
        {
            if (xOverPts[i] < minLen)
                chrom1.swapSubSet( xOverPts[i-1], 
                    xOverPts[i] - xOverPts[i-1], chrom2 );
        }

        return chrom1;
    }

    public Chrom applyAt( int index, Chrom chrom1, Chrom chrom2 )
    {
        return apply( chrom1, chrom2 );
    }
}
