/**********************************************************************
 * File Name: NominalValue.java
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

/** Class for representing values of nominal variables (variables
 *  whose domains are finite. */
public class NominalValue extends Value{

  /** Value is represented as a string. */
  String value;
  
  /** Create a new NominalValue.
   */
  public NominalValue(String v ){
    value = v;
  }

  public NominalValue(int v ){
    value = v + "";
  }


  /** Is the given value equal to this value?
   * @param v value with which to compare equality.
   * @return true if values are equal, false otherwise.
   */
  public boolean equal(Value v){
    if(value.equals(((NominalValue)v).value))
      return true;
    else
      return false;
  }

  /** Get the value in String form. 
   * @return the current value
   */
  public String toString(){
    return value;
  }
}
