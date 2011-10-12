// ConcurrentModificationException
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
 * Thrown by the iterator for SimpleList in the case of concurrent
 * modification.  A duplicate of JDK1.2 functionality, provided here
 * in order to simplify porting at a later date.
 *
 * @author gajit@micropraxis.com
 * @version 0.2
 */
public class ConcurrentModificationException extends RuntimeException
{
    public ConcurrentModificationException()
    {
    }

    public ConcurrentModificationException( String message )
    {
        super( message );
    }
}

