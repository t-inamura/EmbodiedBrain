#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <sstream>

#include <embodied_brain/imitation_motion_accumulator.h>
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
		ImitationMotionAccumulator mainApp;

		//メイン処理
		mainApp.run(argc, argv);

		return EXIT_SUCCESS;
	}
	catch (std::exception ex)
	{
		std::cout << ex.what() << std::endl;
	}
}
