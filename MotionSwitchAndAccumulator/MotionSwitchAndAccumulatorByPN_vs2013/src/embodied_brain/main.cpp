#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <sstream>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/property_tree/ptree.hpp>

#include <embodied_brain/motion_switch_and_accumulator.h>
#include <embodied_brain/common/param.h>


/*
 * 真似動作収録プログラムのエントリポイント
 *
 * プログラムの詳細は、クラス説明を参照。
 */
int main(int argc, char **argv)
{
	try
	{
		MotionSwitchAndAccumulator mainApp;

		//メイン処理
		return mainApp.run(argc, argv);
	}
	catch (std::string &ex)
	{
		std::cout << " ERR :" << ex << std::endl;
		exit(EXIT_FAILURE);
	}
	catch(const property_tree::ptree_error &ex)
	{
		std::cout << " ERR :" << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (boost::exception &ex) 
	{
		std::cout << " ERR :" << boost::diagnostic_information_what(ex) << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (std::exception ex)
	{
		std::cout << " ERR :" << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
}
