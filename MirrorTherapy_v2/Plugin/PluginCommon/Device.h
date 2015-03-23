#pragma once
#include <SIGService.h>
#include <Common/SensorData.h>

// Define delimiters to split string.
#define KEY_DELIMITER ":"
#define RECORD_DELIMITER ";"
#define VALUE_DELIMITER ","
static const std::string keyDelimDefault = KEY_DELIMITER;
static const std::string recordDelimDefault = RECORD_DELIMITER;
static const std::string valueDelimDefault = VALUE_DELIMITER;

// Define message header keys.
#define DEV_TYPE_KEY "DEV_TYPE"
#define DEV_ID_KEY "DEV_ID"
static const std::string devTypeKey = DEV_TYPE_KEY;
static const std::string devIDKey = DEV_ID_KEY;

class Device
{
protected:

	///@brief Sensor data.
	SensorData *sensorData;
	
	///@brief Device type.
	std::string deviceType;

	///@brief Device unique ID.
	std::string deviceUniqueID;

	///@brief Parameter file name.
	std::string parameterFileName;

	///@brief Command line parameter. IP address.
	std::string serverAddress;

	///@brief Command line parameter. Port number.
	int portNumber;

	///@brief Service name on SIGService.
	std::string serviceName;

	///@brief Flag of send message to server.
	bool sendMessageFlag;

public:

	virtual ~Device() {};

	///@brief Initialize SIGService.
	///@param sigService Pointer to SIGService.
	virtual void initializeSigService(sigverse::SIGService &sigService);

	///@brief Read parameter file.
	///@return Success flag to read parameter.
	virtual bool readIniFile();

	///@brief Set service name.
	virtual void setSigServiceName() = 0;

	///@brief Send message to SIGServer.
	///@param sigService Pointer to SIGService.
	///@param message Message you want to send to SIGServer.
	virtual const void sendMessage(sigverse::SIGService &sigService, const std::string &message);
	// ���b�Z�[�W�̐����K���ɂ���
	// (1) ��{�I�ɁC�uKEY:VALUE;�v�Ƃ���KEY��VALUE�̃y�A���u;�v�ŋ�؂��Ă�����̂�K�v�Ȃ����q���ă��b�Z�[�W�ɂ��܂��D
	//		��)�@DEV_TYPE:KINECTV2;DEV_ID:0;
	//		����)�@KEY��DEV_TYPE�ŁCVALUE��KINECTV2�̃y�A�ƁCKEY��DEV_ID�ŁCVALUE��0��2�̃y�A�����b�Z�[�W�ɂ���ꍇ�̗�D
	// (2) VALUE���x�N�g��(������)�̏ꍇ�C�l���ƂɁu,�v�ŋ�؂�C�擪�Ɂu(�v�C�����Ɂu)�v��t���܂��D
	//		��)�@EULER:(0.3,-0.2,1.0);
	// (3) ���b�Z�[�W�̖`���ɂ�KEY���uDEV_TYPE�v�̃y�A��KEY���uDEV_ID�v�̃y�A�́C��̃y�A�̏����܂ނׂ��ł��D
	// (4) ���b�Z�[�W���ɂ̓X�y�[�X���܂ނׂ��ł͂Ȃ��ƍl���܂��D

	///@brief Get device type. For example, device type is "KINECTV2".
	const std::string getDeviceType() {
		return this->deviceType;
	}

	///@brief Get device unique ID.
	const std::string getDeviceUniqueID() {
		return this->deviceUniqueID;
	}

	void setDeviceType(const std::string &deviceTypeString) {
		this->deviceType = deviceTypeString;
	}

	void setDeviceUniqueID(const std::string &deviceUniqueIDString) {
		this->deviceUniqueID = deviceUniqueIDString;
	}

	///@brief Generate message header that is made by device type and unique ID.
	std::string generateMessageHeader() {
		const std::string tmp = devTypeKey + keyDelimDefault + this->deviceType + recordDelimDefault + devIDKey + keyDelimDefault + this->deviceUniqueID + recordDelimDefault;
		return tmp;
	}
};

