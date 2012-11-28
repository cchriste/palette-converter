/*! \brief A small program to convert a SCITRun 1D transfer function saved 
 *		   in text mode with extension .cmap to a visit 1D transfer function
 *
 *	Created by Pascal Grosset
 *  Date May 25, 2011
 */
 
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;


const string part1 = "\
<?xml version=\"1.0\"?>	\n\
<Object name=\"VolumeAttributes\">	\n\
\t<Field name=\"legendFlag\" type=\"bool\">true</Field>	\n\
\t<Field name=\"lightingFlag\" type=\"bool\">false</Field>	\n\
\t<Object name=\"colorControlPoints\"> 	\n\
\t\t<Object name=\"ColorControlPointList\"> \n";


const string part2 = "\
\t\t\t<Field name=\"smoothingFlag\" type=\"bool\">true</Field>\n\
\t\t\t<Field name=\"equalSpacingFlag\" type=\"bool\">false</Field>\n\
\t\t\t<Field name=\"discreteFlag\" type=\"bool\">false</Field>\n\
\t\t\t<Field name=\"externalFlag\" type=\"bool\">false</Field>\n\
\t\t</Object>\n\
\t</Object>\n\
\t<Field name=\"opacityAttenuation\" type=\"float\">1</Field>\n	\
\t<Field name=\"opacityMode\" type=\"string\">FreeformMode</Field>\n\
\t<Object name=\"opacityControlPoints\">\n	\
\t\t<Object name=\"GaussianControlPointList\">\n\
\t\t</Object>\n\
\t</Object>\n\
\t<Field name=\"resampleTarget\" type=\"int\">10000000</Field>\n	\
\t<Field name=\"opacityVariable\" type=\"string\">default</Field>\n\
\t<Field name=\"freeformOpacity\" type=\"unsignedCharArray\" length=\"256\">";


const string part3 = "\
\t</Field>\n\
\t<Field name=\"useColorVarMin\" type=\"bool\">false</Field>\n\
\t<Field name=\"colorVarMin\" type=\"float\">0</Field>\n\
\t<Field name=\"useColorVarMax\" type=\"bool\">false</Field>\n\
\t<Field name=\"colorVarMax\" type=\"float\">0</Field>\n\
\t<Field name=\"useOpacityVarMin\" type=\"bool\">false</Field>\n\
\t<Field name=\"opacityVarMin\" type=\"float\">0</Field>\n\
\t<Field name=\"useOpacityVarMax\" type=\"bool\">false</Field>\n\
\t<Field name=\"opacityVarMax\" type=\"float\">0</Field>\n\
\t<Field name=\"smoothData\" type=\"bool\">true</Field>\n\
\t<Field name=\"samplesPerRay\" type=\"int\">400</Field>\n\
\t<Field name=\"rendererType\" type=\"string\">SLIVR</Field>\n\
\t<Field name=\"gradientType\" type=\"string\">SobelOperator</Field>\n\
\t<Field name=\"num3DSlices\" type=\"int\">400</Field>\n\
\t<Field name=\"scaling\" type=\"string\">Linear</Field>\n\
\t<Field name=\"skewFactor\" type=\"double\">1</Field>\n\
\t<Field name=\"limitsMode\" type=\"string\">OriginalData</Field>\n\
\t<Field name=\"sampling\" type=\"string\">Rasterization</Field>\n\
\t<Field name=\"rendererSamples\" type=\"float\">4</Field>\n\
\t<Field name=\"transferFunctionDim\" type=\"int\">1</Field>\n\
\t<Field name=\"lowGradientLightingReduction\" type=\"string\">Lower</Field>\n\
\t<Field name=\"lowGradientLightingClampFlag\" type=\"bool\">false</Field>\n\
\t<Field name=\"lowGradientLightingClampValue\" type=\"double\">1</Field>\n\
</Object>";


int main (int argc,char *argv[]) {
	int numColors;
	float r, g, b, opacity, pos;
	
	int transferFnResolution = 10;	// 5 causes a crash!!!

	if (argc < 3 || argc > 5){
		cout << "Invalid number of parameters passed ("<<argc<<"). Program should be used as follows: " << endl;
		cout << "./transferConvert <scirun_Text_TransferFunction1D.cmap> <visitTransferFunction.xml> [destType (visit or visus)] [<opacityCorrection> (between -1 and 0)]" << endl;
		return 1;	
	}
	
	// open input and outpuf files
	ifstream inputFile (argv[1]);
	ofstream outputFile (argv[2]);

  string dstType("visus");
  if (argc>=4)
    dstType=argv[3];

  float opacityCorrection=0.0f;
  if (argc>=5)
    opacityCorrection=atof(argv[4]);

	cout << "Input filename: " << argv[1] << endl;
	cout << "Output filename: " << argv[2] << endl;
	
  if (inputFile.is_open() && outputFile.is_open()) {

    if (dstType=="visit")
    {
  		// Write first part of visit transfer function file
      outputFile << part1;
		
      // read the number of colors in the original transfer function
      inputFile >> numColors;
		
      // write the values every n times
  		for (int i=0; i<numColors; i++){
  			inputFile >> r >> g >> b >> opacity >> pos;
  			if ((i%transferFnResolution == 0) || (i == numColors-1)){
  				outputFile << "\t\t\t<Object name=\"ColorControlPoint\"> " << endl;
  				outputFile << "\t\t\t\t<Field name=\"colors\" type=\"unsignedCharArray\" length=\"4\">" 
                     << ((int)(r*255)) << " " << ((int)(g*255)) << " " <<((int)(b*255)) << " 255 </Field>" << endl;
  				outputFile << "\t\t\t\t<Field name=\"position\" type=\"float\">" << pos << "</Field>" << endl;
  				outputFile << "\t\t\t</Object>" << endl;
  			}
  		}

      inputFile.close();	

      // output second part of the transfer function file
      outputFile << part2;

      // reopen file to read
      inputFile.open(argv[1]);
   		
      if (inputFile.is_open()){
        // read the number of colors in the original transfer function
        inputFile >> numColors;
  		
  		
        // write the different alpha values
        for (int i=0; i<numColors; i++){
          inputFile >> r >> g >> b >> opacity >> pos;
          outputFile << ((int)(opacity*255)) << " ";
        }
        // topup the number of colors in the transfer function to 256 always - to mimic slivr
        for (int i=numColors; i<256; i++)
          outputFile << ((int)(opacity*255)) << " ";
  		
        // output final part of file
        outputFile << part3;

        // close input and output file
        inputFile.close();
        outputFile.close();
		
        cout << "Sussefully done reading from" << argv[1] << " and writing to " << argv[2]<< endl;
      }
      else {
        cout << "Unable to open input file"; 
        return 1;
      }
    }
    else if (dstType=="visus")
    {
      // read the number of colors in the original transfer function
      inputFile >> numColors;
      if (numColors != 256)
      {
        cerr << "Error: must have 256 values in original data to write ViSUS tf function (lame==lazy).\n";
        return 1;
      }

      outputFile<<numColors<<endl;

      // write the values assuming constant step between each
  		for (int i=0; i<numColors; i++){
  			inputFile >> r >> g >> b >> opacity >> pos;

        // modify opacity to handle wierd attenuation thing in VisIt
        if (opacityCorrection<0)
        {
          float bp=tan(1.570796327/*PI/2*/ * (0.5 - opacityCorrection*0.499999));
          opacity=pow(opacity,bp);
        }

        outputFile << ((int)(r*255)) << "," << ((int)(g*255)) << "," <<((int)(b*255)) << "," << ((int)(opacity*255)) << endl;
  		}
    }
    else
    {
      cerr << "Error: unknown destination type specified ("<<dstType<<")\n";
      return 1;
    }
  }
  else{
		cout << "Unable to open input or output file"; 
    return 1;
	}

 	return 0;
}
