/**
 * @file      132.call.c
 * @author    The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br
 *
 * @version   1.0
 * @date      Mon, 19 Jun 2006 15:33:22 -0300
 * @brief     It is a simple main function that uses indirect calls functions.
 *
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

/* Declarations */
float funcA(float input);
float funcB(float input);
float funcC(float input);
float funcD(float input);
float funcE(float input);
float funcF(float input);
float funcG(float input);
float funcH(float input);

/* The file begin.h is included if compiler flag -DBEGINCODE is used */
#ifdef BEGINCODE
#include "begin.h"
#endif

int main() {
  
  float tmp=0;
  tmp=funcA(-1);
  tmp=0;  
  
  return 0; 
  /* Return 0 only */
}

/* The file end.h is included if compiler flag -DENDCODE is used */
#ifdef ENDCODE
#include "end.h"
#endif

float count=0;

float funcA(float input) {
 float a=-2;
  funcB(input);
  count++;
  a++;
  return input;
} 



float funcB(float input) {
  float b=0;
  funcC(input);
  count++;
  b++;
  return input;
}



float funcC(float input) {
  float c=-1;
  funcD(input);
  count++;
  c++;
  return input;
} 



float funcD(float input) {
  float d=9;
  funcE(input);
  count++;
  d++;
  return input;
}



float funcE(float input) {
  float e=-1;
  funcF(input);
  count++;
  e++;
  return input;
} 



float funcF(float input) {
  float f=14;
  funcG(input);
  count++;
  f++;
  return input;
}



float funcG(float input) {
  float g=-2;
  funcH(input);
  count++;
  g++;
  return input;
} 



float funcH(float input) {
  float h=1;
  count++;
  h++;

  return input;
}

