// RandomInt
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

package com.micropraxis.util;

/**
 * A utility class purely to provide a random integer generator
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class RandomInt
{
    /**
     * Returns a random integer evenly selected from the range 0 to
     * rangeSize-1. 
     *
     * @param rangeSize the number of possible answers wanted
     * @return a random integer in the required range
     */
    public static int next( int rangeSize )
    {
        int random = (int) (Math.random() * rangeSize);

        // Cope with the 1 in 2^64-ish chance that random returns 1.0

        if (random == rangeSize)
            --random;

        return random;
    }
}
