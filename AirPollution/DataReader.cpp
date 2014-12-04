#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "DataReader.h"

const unsigned int   DataReader::INVALID_PM10   = UINT_MAX;
const unsigned short DataReader::INVALID_PM25   = USHRT_MAX;
const float          DataReader::INVALID_CARBON = FLT_MAX;
const float          DataReader::INVALID_NO2    = FLT_MAX;


Data DataReader::read(char* path)
{
  if (!cacheExists(path))
    createCache(path);

  return readFromCache(path);
}

bool DataReader::cacheExists(char* path)
{
  std::string cache_path = path;
  cache_path += ".cache";

  std::ifstream f(cache_path.c_str());
  if (f.good()) 
  {
    f.close();
    return true;
  }
  else 
  {
    f.close();
    return false;
  }
}

Data DataReader::readFromCache(char* path)
{
  std::string cache_path = path;
  cache_path += ".cache";

  // load binary in

  std::ifstream infile(cache_path, std::ios::in | std::ios::binary);

  unsigned int size = 0;
  infile.read((char*) &size, sizeof(unsigned int));

  DataPoint* points = new DataPoint[size];
  infile.read((char*) points, size * sizeof(DataPoint*));

  Data data;
  data.size = size;
  data.points = points;

  return data;
}

void DataReader::createCache(char* path)
{
  std::cout << "creating cache..." << std::endl;

  unsigned int max_size = countLines(path) - 1;
  DataPoint* points = new DataPoint[max_size];

  std::ifstream file(path);

  // skip header line
  std::string line;
  std::getline(file, line); 
  unsigned int current_size = 0;
  while(file.good())
  {
    // read lines and split them
    std::vector<std::string> parts = nextParts(file);

    // convert types and set in datapoint
    setProperties(&(points[current_size]), parts);
    current_size += 1;

    if (current_size % 100 == 0)
      std::cout << current_size << " / " << max_size << std::endl;
  }

  // build data object
  Data data;
  data.size = current_size - 1;
  data.points = points;

  // interpolate values
  interpolateSeconds(data);
  // maybe also interpolate carbon (one value per minute)

  // write to cache file
  
  std::string cache_path = path;
  cache_path += ".cache";

  std::ofstream outfile;
  outfile.open(cache_path, std::ios::binary | std::ios::out);
  outfile.write((char*) &(data.size), sizeof(unsigned int));
  outfile.write((char*) data.points, data.size * sizeof(DataPoint));
  outfile.close();

  delete[] data.points;
}

unsigned int DataReader::countLines(char* path)
{
  std::ifstream file(path);
  std::string line;

  unsigned int lines = 0;
  while (std::getline(file, line))
    lines++;

  return lines;
}

std::vector<std::string> DataReader::nextParts(std::istream& stream)
{
  std::vector<std::string> tokens;

  std::string line;
  std::getline(stream, line);

  std::stringstream line_stream(line);
  for (std::string part; std::getline(line_stream, part, ',');)
    tokens.push_back(part);

  return tokens;
}

// 0 - Timestamp
// 1 - Latitude
// 2 - Longitude
// 3 - CSlatitude
// 4 - CSlongitude
// 5 - Source
// 6 - AQI
// 7 - NO2
// 8 - HUMD
// 9 - PRES
// 10 - TEMP
// 11 - PM1.0 particles per cc
// 12 - PM2.5 Avg ug per m3
// 13 - Black Carbon ug per m3
DataPoint* DataReader::setProperties(DataPoint* target, std::vector<std::string> parts)
{
  std::stringstream date_stream(parts[0]);
  std::string date_part;

  std::getline(date_stream, date_part, '/');
  target->month = std::stoi(date_part);

  std::getline(date_stream, date_part, '/');
  target->day = std::stoi(date_part);

  std::getline(date_stream, date_part, ' ');
  target->year = std::stoi(date_part);

  std::getline(date_stream, date_part, ':');
  target->hour = std::stoi(date_part);

  std::getline(date_stream, date_part);
  target->minute = std::stoi(date_part);

  target->seconds = 0; // interpolate later

  target->latitude = std::stod(parts[1]);
  target->longitude = std::stod(parts[2]);

  try { target->pm10 = std::stoi(parts[11]); }
  catch (...) { target->pm10 = INVALID_PM10; }

  try { target->pm25 = std::stoi(parts[12]); }
  catch (...) { target->pm25 = INVALID_PM25; }

  try { target->carbon = std::stof(parts[13]); }
  catch (...) { target->carbon = INVALID_CARBON; }

  try { target->no2 = std::stof(parts[7]); }
  catch (...) { target->no2 = INVALID_NO2; }

  return target;
}

void DataReader::interpolateSeconds(Data data)
{
  unsigned int i = 0;
  while (i < data.size-1)
  {
    // find index of next minute
    unsigned next = i + 1;
    while (next < data.size && 
      data.points[i].hour == data.points[next].hour &&
      data.points[i].minute == data.points[next].minute)
      next++;

    // interpolate the values in between
    unsigned int diff = next - i;
    for (unsigned int j = i; j < next; j++)
      data.points[i].seconds = (60 * (j - i)) / diff;
        
    i += next;
  }
}