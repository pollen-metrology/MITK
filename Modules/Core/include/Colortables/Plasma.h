/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

Raw data from new mathplolib colormaps see:
https://github.com/BIDS/colormap/blob/master/colormaps.py
===================================================================*/

#ifndef _lut_Plasma_h_
#define _lut_Plasma_h_

static const int Plasma[256][3] =
{ { 13, 8, 135 },{ 16, 7, 136 },{ 19, 7, 137 },{ 22, 7, 138 },{ 25, 6, 140 },{ 27, 6, 141 },{ 29, 6, 142 },{ 32, 6, 143 },
  { 34, 6, 144 },{ 36, 6, 145 },{ 38, 5, 145 },{ 40, 5, 146 },{ 42, 5, 147 },{ 44, 5, 148 },{ 46, 5, 149 },{ 47, 5, 150 },
  { 49, 5, 151 },{ 51, 5, 151 },{ 53, 4, 152 },{ 55, 4, 153 },{ 56, 4, 154 },{ 58, 4, 154 },{ 60, 4, 155 },{ 62, 4, 156 },
  { 63, 4, 156 },{ 65, 4, 157 },{ 67, 3, 158 },{ 68, 3, 158 },{ 70, 3, 159 },{ 72, 3, 159 },{ 73, 3, 160 },{ 75, 3, 161 },
  { 76, 2, 161 },{ 78, 2, 162 },{ 80, 2, 162 },{ 81, 2, 163 },{ 83, 2, 163 },{ 85, 2, 164 },{ 86, 1, 164 },{ 88, 1, 164 },
  { 89, 1, 165 },{ 91, 1, 165 },{ 92, 1, 166 },{ 94, 1, 166 },{ 96, 1, 166 },{ 97, 0, 167 },{ 99, 0, 167 },{ 100, 0, 167 },
  { 102, 0, 167 },{ 103, 0, 168 },{ 105, 0, 168 },{ 106, 0, 168 },{ 108, 0, 168 },{ 110, 0, 168 },{ 111, 0, 168 },{ 113, 0, 168 },
  { 114, 1, 168 },{ 116, 1, 168 },{ 117, 1, 168 },{ 119, 1, 168 },{ 120, 1, 168 },{ 122, 2, 168 },{ 123, 2, 168 },{ 125, 3, 168 },
  { 126, 3, 168 },{ 128, 4, 168 },{ 129, 4, 167 },{ 131, 5, 167 },{ 132, 5, 167 },{ 134, 6, 166 },{ 135, 7, 166 },{ 136, 8, 166 },
  { 138, 9, 165 },{ 139, 10, 165 },{ 141, 11, 165 },{ 142, 12, 164 },{ 143, 13, 164 },{ 145, 14, 163 },{ 146, 15, 163 },{ 148, 16, 162 },
  { 149, 17, 161 },{ 150, 19, 161 },{ 152, 20, 160 },{ 153, 21, 159 },{ 154, 22, 159 },{ 156, 23, 158 },{ 157, 24, 157 },
  { 158, 25, 157 },{ 160, 26, 156 },{ 161, 27, 155 },{ 162, 29, 154 },{ 163, 30, 154 },{ 165, 31, 153 },{ 166, 32, 152 },
  { 167, 33, 151 },{ 168, 34, 150 },{ 170, 35, 149 },{ 171, 36, 148 },{ 172, 38, 148 },{ 173, 39, 147 },{ 174, 40, 146 },
  { 176, 41, 145 },{ 177, 42, 144 },{ 178, 43, 143 },{ 179, 44, 142 },{ 180, 46, 141 },{ 181, 47, 140 },{ 182, 48, 139 },
  { 183, 49, 138 },{ 184, 50, 137 },{ 186, 51, 136 },{ 187, 52, 136 },{ 188, 53, 135 },{ 189, 55, 134 },{ 190, 56, 133 },
  { 191, 57, 132 },{ 192, 58, 131 },{ 193, 59, 130 },{ 194, 60, 129 },{ 195, 61, 128 },{ 196, 62, 127 },{ 197, 64, 126 },
  { 198, 65, 125 },{ 199, 66, 124 },{ 200, 67, 123 },{ 201, 68, 122 },{ 202, 69, 122 },{ 203, 70, 121 },{ 204, 71, 120 },
  { 204, 73, 119 },{ 205, 74, 118 },{ 206, 75, 117 },{ 207, 76, 116 },{ 208, 77, 115 },{ 209, 78, 114 },{ 210, 79, 113 },
  { 211, 81, 113 },{ 212, 82, 112 },{ 213, 83, 111 },{ 213, 84, 110 },{ 214, 85, 109 },{ 215, 86, 108 },{ 216, 87, 107 },
  { 217, 88, 106 },{ 218, 90, 106 },{ 218, 91, 105 },{ 219, 92, 104 },{ 220, 93, 103 },{ 221, 94, 102 },{ 222, 95, 101 },
  { 222, 97, 100 },{ 223, 98, 99 },{ 224, 99, 99 },{ 225, 100, 98 },{ 226, 101, 97 },{ 226, 102, 96 },{ 227, 104, 95 },
  { 228, 105, 94 },{ 229, 106, 93 },{ 229, 107, 93 },{ 230, 108, 92 },{ 231, 110, 91 },{ 231, 111, 90 },{ 232, 112, 89 },
  { 233, 113, 88 },{ 233, 114, 87 },{ 234, 116, 87 },{ 235, 117, 86 },{ 235, 118, 85 },{ 236, 119, 84 },{ 237, 121, 83 },
  { 237, 122, 82 },{ 238, 123, 81 },{ 239, 124, 81 },{ 239, 126, 80 },{ 240, 127, 79 },{ 240, 128, 78 },{ 241, 129, 77 },
  { 241, 131, 76 },{ 242, 132, 75 },{ 243, 133, 75 },{ 243, 135, 74 },{ 244, 136, 73 },{ 244, 137, 72 },{ 245, 139, 71 },
  { 245, 140, 70 },{ 246, 141, 69 },{ 246, 143, 68 },{ 247, 144, 68 },{ 247, 145, 67 },{ 247, 147, 66 },{ 248, 148, 65 },
  { 248, 149, 64 },{ 249, 151, 63 },{ 249, 152, 62 },{ 249, 154, 62 },{ 250, 155, 61 },{ 250, 156, 60 },{ 250, 158, 59 },
  { 251, 159, 58 },{ 251, 161, 57 },{ 251, 162, 56 },{ 252, 163, 56 },{ 252, 165, 55 },{ 252, 166, 54 },{ 252, 168, 53 },
  { 252, 169, 52 },{ 253, 171, 51 },{ 253, 172, 51 },{ 253, 174, 50 },{ 253, 175, 49 },{ 253, 177, 48 },{ 253, 178, 47 },
  { 253, 180, 47 },{ 253, 181, 46 },{ 254, 183, 45 },{ 254, 184, 44 },{ 254, 186, 44 },{ 254, 187, 43 },{ 254, 189, 42 },
  { 254, 190, 42 },{ 254, 192, 41 },{ 253, 194, 41 },{ 253, 195, 40 },{ 253, 197, 39 },{ 253, 198, 39 },{ 253, 200, 39 },
  { 253, 202, 38 },{ 253, 203, 38 },{ 252, 205, 37 },{ 252, 206, 37 },{ 252, 208, 37 },{ 252, 210, 37 },{ 251, 211, 36 },
  { 251, 213, 36 },{ 251, 215, 36 },{ 250, 216, 36 },{ 250, 218, 36 },{ 249, 220, 36 },{ 249, 221, 37 },{ 248, 223, 37 },
  { 248, 225, 37 },{ 247, 226, 37 },{ 247, 228, 37 },{ 246, 230, 38 },{ 246, 232, 38 },{ 245, 233, 38 },{ 245, 235, 39 },
  { 244, 237, 39 },{ 243, 238, 39 },{ 243, 240, 39 },{ 242, 242, 39 },{ 241, 244, 38 },{ 241, 245, 37 },{ 240, 247, 36 },{ 240, 249, 33 } 
};
#endif
