#ifndef __OCULUS_RIFT_DK1_SENSOR_DATA_H__
#define __OCULUS_RIFT_DK1_SENSOR_DATA_H__

#include "SensorData.h"

#define MSG_KEY_EULER "EULER"


class OculusRiftDK1SensorData : public SensorData
{
private:
	EulerAngleType eulerAngle;

public:

	///@brief Convert sensor information to string.
	///@param itemsDelim Symbol to split items.
	///@param keyValueDelim Symbol to split KEY and VALUE.
	///@param valuesDelim Symbol to split each componet of VALUE. When VALUE is multidimensional data, you use this symbol.
	std::string encodeSensorData(const std::string &itemsDelim = ITEMS_DELIMITER, const std::string &keyValueDelim = KEY_AND_VALUE_DELIMITER, const std::string &valuesDelim = VALUES_DELIMITER) const;

	///@brief Set to sensor data.
	///@param Map of sensor data;
	bool setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap);

	///@brief Set sensor data.
	void setEulerAngle(const float &yaw, const float &pitch, const float &roll);

	///@brief Get sensor data.
	float getYaw()
	{
		return this->eulerAngle.yaw;
	}
	float getPitch()
	{
		return this->eulerAngle.pitch;
	}
	float getRoll()
	{
		return this->eulerAngle.roll;
	}
};


#endif //__OCULUS_RIFT_DK1_SENSOR_DATA_H__
