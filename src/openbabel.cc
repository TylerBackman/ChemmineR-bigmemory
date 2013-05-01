


#include <iostream>
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/oberror.h>
#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include <string>

using namespace std;
using namespace OpenBabel;

extern "C" {
   SEXP smile2sdf_file(SEXP smileFile, SEXP sdfFile);
   SEXP smile2sdf_string(SEXP smileFile);
	SEXP ob_convert_file(SEXP fromE,SEXP toE, SEXP sourceFileE,SEXP destinationFileE);
	SEXP ob_convert(SEXP fromE,SEXP toE, SEXP sourceStrE);
}


SEXP ob_convert_file(SEXP fromE,SEXP toE, SEXP sourceFileE,SEXP destinationFileE)
{
	const char* from = CHAR(STRING_ELT(fromE,0));
	const char* to = CHAR(STRING_ELT(toE,0));

   ifstream ifs(CHAR(STRING_ELT(sourceFileE,0)));
   if(!ifs){
      error("cannot open smile file for input: %s",CHAR(STRING_ELT(sourceFileE,0)));
      return R_NilValue;
   }

   ofstream ofs(CHAR(STRING_ELT(destinationFileE,0)));
   if(!ofs){
      error("cannot open sdf file for output: %s",CHAR(STRING_ELT(destinationFileE,0)));
      return R_NilValue;
   }

   OpenBabel::OBConversion conv(&ifs,&ofs);
	if(!conv.SetInAndOutFormats(from,to)) {
      error("conversion from %s to %s is not available",from,to);
      return R_NilValue;
   }
	if(conv.FindFormat(from)==0 ){
		error("could not find format %s",from);
		return R_NilValue;
	}
	if(conv.FindFormat(to)==0){
		error("could not find format %s",to);
		return R_NilValue;
	}

   conv.AddOption("gen2D",OBConversion::GENOPTIONS);
	
   conv.Convert();

	return R_NilValue;

}
SEXP ob_convert(SEXP fromE,SEXP toE, SEXP sourceStrE)
{
	
	const char* from = CHAR(STRING_ELT(fromE,0));
	const char* to = CHAR(STRING_ELT(toE,0));

	istringstream ifs(CHAR(STRING_ELT(sourceStrE,0)));
	ostringstream ofs;


   OpenBabel::OBConversion conv(&ifs,&ofs);
	if(!conv.SetInAndOutFormats(from,to)) {
      error("conversion from %s to %s is not available",from,to);
      return R_NilValue;
   }
	if(conv.FindFormat(from)==0 ){
		error("could not find format %s",from);
		return R_NilValue;
	}
	if(conv.FindFormat(to)==0){
		error("could not find format %s",to);
		return R_NilValue;
	}

   conv.AddOption("gen2D",OBConversion::GENOPTIONS);
	
   conv.Convert();

	SEXP sdfString;
   PROTECT(sdfString = NEW_STRING(1));
   SET_STRING_ELT(sdfString,0,mkChar(ofs.str().c_str()));
   UNPROTECT(1);


   return sdfString;
}

SEXP smile2sdf_file(SEXP smileFile, SEXP sdfFile)
{

   ifstream ifs(CHAR(STRING_ELT(smileFile,0)));
   if(!ifs){
      error("cannot open smile file for input: %s",CHAR(STRING_ELT(smileFile,0)));
      return R_NilValue;
   }

   ofstream ofs(CHAR(STRING_ELT(sdfFile,0)));
   if(!ofs){
      error("cannot open sdf file for output: %s",CHAR(STRING_ELT(sdfFile,0)));
      return R_NilValue;
   }

   OpenBabel::OBConversion conv(&ifs,&ofs);

   if(!conv.SetInAndOutFormats("SMI","SDF")) {
      error("conversion from smile to sdf not available");
      return R_NilValue;
   }

   conv.AddOption("gen2D",OBConversion::GENOPTIONS);
	
   conv.Convert();

   return R_NilValue;

}

SEXP smile2sdf_string(SEXP smileFile)
{

   ifstream ifs(CHAR(STRING_ELT(smileFile,0)));
   if(!ifs){
      error("cannot open smile file for input: %s",CHAR(STRING_ELT(smileFile,0)));
      return R_NilValue;
   }


	ostringstream ofs;

   OpenBabel::OBConversion conv(&ifs,&ofs);

   if(!conv.SetInAndOutFormats("SMI","SDF")) {
      error("conversion from smile to sdf not available");
      return R_NilValue;
   }

   conv.AddOption("gen2D",OBConversion::GENOPTIONS);
	
   conv.Convert();

   /* start at returning data in a list of vectors
    * problem is we may not be able to fit entire file into memory

   istringstream smileStream(ofs.str());
   string line;
   vector<string>
   while(getline(smileStream,line))
   {
   }
   */

   SEXP sdfString;
   PROTECT(sdfString = NEW_STRING(1));
   SET_STRING_ELT(sdfString,0,mkChar(ofs.str().c_str()));
   UNPROTECT(1);


   return sdfString;

}
