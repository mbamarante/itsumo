/**********************************************************************
 * File Name: Value.java
 * Author: Jay Modi (modi@isi.edu)
 * Date: Nov 11 2000
 *
 *
 **********************************************************************/
package adopt.problem;
import adopt.common.*;

import java.lang.*;
import java.io.*;
import java.util.*;

/** An abstract class for representing values of variables.
 */
public abstract class Value {
  
  public abstract boolean equal(Value v);
  public abstract String toString();
}
