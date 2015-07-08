#ifndef __SENSOR_DATA_H__
#define __SENSOR_DATA_H__

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include <sigverse/common/device/DeviceCommon.h>

class SensorData
{
public:
	SensorData() {};
	virtual ~SensorData() {};

	///@brief Convert sensor information to string.
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	virtual std::string encodeSensorData(const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER) const;
	
	///@brief Convert message(string) to sensor data map.
	///@param message string of message.
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	static std::map<std::string, std::vector<std::string> > decodeSensorData(const std::string &message, const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER);

	///@brief Set to sensor data.
	///@param Map of sensor data;
	virtual bool setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap);

	///@brief Convert message to std::map. 
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	static std::map<std::string, std::vector<std::string> > convertMessage2Map(const std::string &message, const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER);

	///@brief Convert std::map to message.
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	static std::string convertMap2Message(const std::map<std::string, std::vector<std::string> > &map, const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER);
};

#endif //__SENSOR_DATA_H__
