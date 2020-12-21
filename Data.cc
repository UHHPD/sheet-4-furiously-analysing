#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

  //errors
  for (int i = 0; i < size; ++i) {
    double Sentries;
    file >> Sentries;
    m_errors.push_back(Sentries);
  }

  // done! close the file
  file.close();

  assertSizes();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

int Data :: checkCompatibility ( const Data & in , int n ){
  int k = 0;
  for (int i = 0; i < size(); ++i){
    if (abs(measurement(i)- in.measurement(i)) > n*sqrt(pow(error(i),2)+pow(in.error(i),2))){
     k++;
     }
  }
  return k;
}

Data Data:: operator+ ( const Data & in ){ 
  Data y=in;
  if (checkCompatibility(in,2) >  2 ){ //95% of measurements have to be insight 2 sigma
    cout << "Datasets are incompatible, as they differ in " << checkCompatibility(in,2) << " variables"	 
         << endl;
  }
  else {
    double w1,w2,y1,y2;
    vector<double> yvec1,yvec2;
    for (int i = 0; i < size(); ++i){
      w1 = pow(error(i),-2);
      w2 = pow(in.error(i),-2);
      y1 = measurement(i);
      y2 = in.measurement(i);
      yvec1.push_back((w1*y1 + w2*y2)/(w1+w2));
      yvec2.push_back(pow(w1+w2,-0.5));
    } 
    y.m_data=yvec1;
    y.m_errors=yvec2;
  }
  return y;
}

double Data::y_pred(int i){
  double a = 0.005;
  double b = -0.00001;
  double c = 0.08;
  double d = 0.015;
  double f;
  double x = binCenter(i);
  f = a + b * x + c * exp(-d * x);
  return f;
}

double Data::y_predsum(){
  int y = 0 ;
  for (int i = 0;i < size(); i++){
    y += y_pred(i);
  }
  return y;
}

double Data::chisq_n(){
  double chisq = 0;
  for (int i = 0;i < size(); i++){
    chisq += pow((measurement(i) - y_pred(i)) / error(i) ,2);
  }
  return chisq / (size() -4 );
}
