palette-converter
=================

conversion between transfer functions of SCIRun, VisIt, ViSUS, ...

Credits:
This application was originally created by Pascal Grosset (pascal.av.grosset@gmail.com).
Modified for ViSUS by Cameron Christensen (scicameron@gmail.com).

License:
MIT

Disclaimer:
This application is provided without warantee of any kind. Seriously. Use at your own risk.

NOTES:

This program converts a 1D transfer function generated from SCIRun to a VisIt type 1D transfer function.

Exporting a transfer function:
- From the GenStandardColorMaps module, add a ColorMapWriter output.
- Save the transfer function as TextColorMap (*.*)

Compile:
g++ cmapfileReading.cpp -o cmfr

Run:
./cmfr defaultCharlesTransfer.cpp visitTransfer.xml
