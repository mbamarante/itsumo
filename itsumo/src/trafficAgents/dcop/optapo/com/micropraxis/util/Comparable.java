// Comparable
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
 * This interface is implemented by classes that can compare themselves with
 * other objects of their class.  It is identical to the JDK 1.2 Comparable
 * interface to aid porting at a later date. 
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public interface Comparable
{
    /**
     * Compares this Object with the specified Object for order. 
     */
    public int compareTo( Object o );
}
