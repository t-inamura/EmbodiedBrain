#include "SensorData.h"


std::string SensorData::encodeSensorData(const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const { return NULL; }


std::map<std::string, std::vector<std::string> > SensorData::decodeSensorData(const std::string &message, const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim) const
{
	// Generate map<string, vector<string> from message. Just split message by ";" and ":".
	return this->convertMessage2Map(message, itemsDelim, keyValueDelim, valuesDelim);
}


bool SensorData::setSensorData(const std::map<std::string, std::vector<std::string> > &sensorDataMap){ return NULL; }


std::map<std::string, std::vector<std::string> > SensorData::convertMessage2Map(const std::string &message, const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim)
{
	std::map<std::string, std::vector<std::string> > map;

	try
	{
		// まずは「;」でKEY:VALUEごとに文字列を取得する
		std::vector<std::string> items;
		boost::split(items, message, boost::is_any_of(itemsDelim));

		for(int i = 0; i < (int)items.size(); i++)
		{
			if (items[i] == "") continue ;

			//std::cout << pairs[i] << std::endl;
			
			// KEY:VALUE となっている文字列から KEY と VALUE を抽出してマップにする
			std::vector<std::string> keyAndValues;
			boost::split(keyAndValues, items[i], boost::is_any_of(keyValueDelim));

//			// 「(」と「)」を削除する（無い場合は何もしない）．
//			// Delete "(" and ")" from message.
//			const std::string valuesStringDeletedParentheses = SensorData::deleteParenthesesFromString(keyAndValues[1]);

			// VALUE部分を「,」で分割する
			std::vector<std::string> values;
//			boost::split(valuesVector, valuesStringDeletedParentheses, boost::is_any_of(valuesDelim));
			boost::split(values, keyAndValues[1], boost::is_any_of(valuesDelim));

			map.insert( std::map<std::string, std::vector<std::string> >::value_type(keyAndValues[0], values) );
		}
	}
	catch (std::exception &ex)
	{
		std::cout << "error!(SensorData::convertMessage2Map):" << ex.what() << std::endl;
		throw ex;
	}
	return map;
}


std::string SensorData::convertMap2Message(const std::map<std::string, std::vector<std::string> > &map, const std::string &itemsDelim, const std::string &keyValueDelim, const std::string &valuesDelim)
{
	std::stringstream ss;

	try
	{
		std::map<std::string, std::vector<std::string> >::const_iterator itItem = map.begin();

		while(itItem != map.end())
		{
			ss << (*itItem).first << keyValueDelim;

			std::vector<std::string> values = (*itItem).second;

			std::vector<std::string>::const_iterator itValues = values.begin();

			while(itValues != values.end())
			{
				ss << *itValues;

				itValues++;

				if(itValues != values.end()){ ss << valuesDelim; }
			}

			itItem++;

			if(itItem!=map.end()){ ss << itemsDelim; }
		}
	}
	catch (std::exception &ex)
	{
		std::cout << "error!(SensorData::convertMap2Message):" << ex.what() << std::endl;
		throw ex;
	}

	return ss.str();
}


//std::string SensorData::deleteParenthesesFromString(const std::string &input, const std::string &pBegin, const std::string &pEnd)
//{
//	// 「(」と「)」を削除する．
//	// Delete "(" and ")" from message.
//
//	// ( を探す
//	std::string tmpValuesString = input;
//	const std::string::size_type pos1(tmpValuesString.find("("));
//
//	// ( が無い場合は入力の文字列を返して終了する．
//	if (pos1 == std::string::npos) {
//		return input;
//	}
//
//	tmpValuesString.replace(pos1, 1, "");
//	const std::string::size_type pos2(tmpValuesString.find(")"));
//	tmpValuesString.replace(pos2, 1, "");
//	const std::string valuesStringDeletedParenthesis = tmpValuesString;
//	return tmpValuesString;
//}
