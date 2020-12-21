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
  return n;
}
