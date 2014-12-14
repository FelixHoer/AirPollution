#ifndef _DATA_READER_H_
#define _DATA_READER_H_

typedef struct {
  unsigned int line;

  unsigned short year;
  unsigned char month;
  unsigned char day;

  unsigned char hour;
  unsigned char minute;
  unsigned char seconds; // interpolated

  double latitude;
  double longitude;

  unsigned int pm10;   // PM 1.0 (Count) 
  unsigned short pm25; // PM 2.5 (ug)
  float carbon;        // Black Carbon(ug)
  float no2;           // NO2
} DataPoint;

typedef struct {
  unsigned int size;
  DataPoint* points;
} Data;

class DataReader
{
protected:
  static bool cacheExists(char* path);
  static void createCache(char* path);
  static Data readFromCache(char* path);

  static unsigned int countLines(char* path);
  static std::vector<std::string> nextParts(std::istream& stream);
  static DataPoint* setProperties(DataPoint* target, std::vector<std::string> parts);
  static void interpolateSeconds(Data data);

public:
  static const unsigned int   INVALID_PM10;
  static const unsigned short INVALID_PM25;
  static const float          INVALID_CARBON;
  static const float          INVALID_NO2;

  static Data read(char* path);
};

#endif

