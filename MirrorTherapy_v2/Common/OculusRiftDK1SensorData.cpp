#include "OculusRiftDK1SensorData.h"
#include <sstream>
std::string OculusRiftDK1SensorData::encodeSensorData2Message(const std::string &pairsDelim, const std::string &keyValueDelim, const std::string &vectorDelim)
{
	std::stringstream ss;
	ss << sensorKey << keyValueDelim << 
		"(" << this->eulerAngle.yaw << vectorDelim << this->eulerAngle.pitch << vectorDelim << this->eulerAngle.roll << ")" << pairsDelim;
	return ss.str();
}

void OculusRiftDK1SensorData::decodeMessage2SensorData(const std::string &message, const std::string &pairsDelim, const std::string &keyValueDelim, const std::string &vectorDelim)
{
	// Generate map<string, string> from message. Just split message by ";" and ":".
	std::map<std::string, std::vector<std::string> > messageMap = SensorData::convertMessage2Map(message, pairsDelim, keyValueDelim, vectorDelim);

	// Get values (vector data) from euler value "(yaw, pitch, roll)".

	try {

		// Split values by ",".
		std::vector<std::string> tmpValuesStringVector = messageMap[sensorKey];
	
		// 3�����̒l�łȂ��Ƃ��́C���̓f�[�^���o�͂��ďI��
		if((int)tmpValuesStringVector.size() != 3) {
			std::cout << "Not euler value: " << message << std::endl;
			return;
		}

		// Euler�p�𒊏o���ăZ�b�g����D
		// Extract euler angle.
		const float yaw		 = atof(tmpValuesStringVector[0].c_str());
		const float pitch	 = atof(tmpValuesStringVector[1].c_str());
		const float roll	 = atof(tmpValuesStringVector[2].c_str());

		//std::cout << "yaw, pitch, roll = " << yaw << "," << pitch << "," << roll << std::endl;

		// Set extracted angle to sensor data.
		this->setEulerAngle(yaw, pitch, roll);
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}

}

///@brief Set sensor data.
void OculusRiftDK1SensorData::setEulerAngle(const float &yaw, const float &pitch, const float &roll)
{
	this->eulerAngle.yaw = yaw;
	this->eulerAngle.pitch = pitch;
	this->eulerAngle.roll = roll;
}
