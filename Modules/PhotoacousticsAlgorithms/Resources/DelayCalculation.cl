/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/
 
__kernel void ckDelayCalculationSphe(  __global unsigned short *gDest,
                         __global unsigned short *usedLines,
                         unsigned int inputL,
                         unsigned int inputS,
                         unsigned int outputL,
                         unsigned int outputS,
                         char isPAImage,
                         float delayMultiplicatorRaw,
                         float totalSamples_i // parameters
                         )
{
  uint globalPosX = get_global_id(0);
  uint globalPosY = get_global_id(1);

  if (globalPosX * 2 < outputL && globalPosY < outputS)
  {
    float l_i = 0; // we calculate the delays relative to line zero
    float s_i = (float)globalPosY / (float)outputS * totalSamples_i;
    float l_s = (float)globalPosX / (float)outputL * (float)inputL; // the currently calculated line
      
    gDest[globalPosY * (outputL / 2) + globalPosX] =
      sqrt(
        pow(s_i, 2)
        +
        pow((delayMultiplicatorRaw * ((l_s - l_i)) / inputL), 2)
      ) + (1-isPAImage)*s_i;
  }
}