#ifndef UTILS_V2_H
#define UTILS_V2_H

////////////////////////////////////////////////
double massFromCoord(int coord){
  double m = 4;
  for(int i = 0; i <= coord; i++) {
    m += m*0.01;
  }
  return m;
}

int coordFromMass(double mass){
  int coord = -999;
  double m  = 4;
  for(int n = 0; n < 261; n++) {
    m += m*0.01;
    if      (mass == 4 )  { coord = 0;   break; }
    else if (m >= mass  ) { coord = n;   break; }
  }
  return coord;
}
////////////////////////////////////////////////

std::string getChannelName(std::string etacat) {

    std::stringstream chanss;
    chanss << "cat" << etacat;

    return chanss.str();
} 

#endif
