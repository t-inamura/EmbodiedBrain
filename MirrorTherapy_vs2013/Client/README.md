Name  
====  
MirrorTherapy_vs2013  

## Overview  
 Rehabilitation project of phantom limb pain with Kinect V2, as Visual Studio 2013 C++ project.  
 Also with OculusRift DK1, as Visual Studio 2010.  

## Description  
 If you lost your limbs or arms, you might suffer from pain of them even they doesn't exist.  
 That is called "phantom limb pain". You might relieve your pain with "MirrorTherapy".  
 "MirrorTherapy" is that you move existing limb or arm in the  mirror, recognizing it as a lost limb or arm.  
 MirrorTherapy project make the therapy efficient and easy with Kinect V2 and Oculus Rift. 

 MirrorTherapy_vs2013�ł́C�T�[�r�X�v���o�C�_�Ƃ���KinectV2_vs2013.sig��OculusDK1_vs2010.sig���g���C�T�[�o�[���̃R���g���[���Ƃ���MirrorTherapyController.cpp���g���܂��D  
 �ڂ����̓\�[�X�R�[�h�Ȃǂ����Ă��炤�����������܂��񂪁CMirrorTherapyController.cpp�́CManNiiAvatarController.cpp�̓��e���p�����Ă��܂��D  
 MirrorTherapy�p�̍\���ł͂���܂����CKinectV2_vs2013.sig��OculusDK1_vs2010.sig��ManNiiAvatarController.cpp�͔ėp�I�Ɏg����Ǝv���܂��D  

## Requirement  
 KinectV2�̕���VS2013�Ńr���h���COculus�̕���VS2010�Ńr���h���܂��D

 ����ɔ����āC�g�p���郉�C�u�����t�@�C���̃o�[�W�������قȂ�̂Œ��ӂ��K�v�ł��D  
 �܂�CKinectV2�ł�Oculus�ł��g���uSIGService.lib�v�́CKinectV2�̃v���W�F�N�g�ł�VS2013�p�̂��̂������N���COculus�̃v���W�F�N�g�ł�VS2010�p�̂��̂������N����K�v������܂��D  
 VS2010�p��SIGService.lib���r���h�ł���O��(*)�ŁCVS2013�p��SIGService.lib���r���h������@����������܂��D  

	(1) C:\SIGVerse\GitHub\Client\SIGService\Windows\ �� SIGService_2010.sln �𓯃t�H���_���ɃR�s�[����
		�uSIGService_2013.sln�v�Ȃǂɖ��O��ύX����D
	(2) SIGService_2013.sln �� VS2013 �ŊJ���D
	(3) �\�����[�V�����\�����uRelease�v�ɂ���D
	(4) �uSIGService�v�̃v���W�F�N�g�itest_lib�ł͂Ȃ��j�̃v���p�e�B�ŁC�o�͐�̃t�H���_��ݒ肵�����D
		�Ⴆ�΁C�u$(SolutionDir)$(Configuration)_2010\�v���u$(SolutionDir)$(Configuration)_2013\�v�ɂ���
		�i���̉��̒��ԃt�@�C���H�̏o�͐�����l�ɕς��Ă����j�D
	(5) �uSIGService�v�̃v���W�F�N�g�itest_lib�ł͂Ȃ��j�̃v���p�e�B�ŁC�ǉ��̃��C�u�����f�B���N�g������Cboost�̃��C�u�����Q�̃����N���VS2013�p�̃t�H���_�ɕύX����D 
		�Ⴆ�΁C�uC:\SIGVerse\src\boost_1_55_0\lib32-msvc-10.0�v���uC:\SIGVerse\src\boost_1_55_0\lib32-msvc-12.0�v�ɕύX����D
	(6) �ȏ�̏�ԂŁC�uSIGService�v�̃v���W�F�N�g���r���h����D
	(7) KinectV2_vs2013.sln�̃r���h���ɁC(6)�Ő�������SIGService.lib�������N����D


 (*)C:\SIGVerse\GitHub\ �̒�����https://github.com/SIGVerse/Client.git ���N���[�����āCC:\SIGVerse\GitHub\Client\SIGService\Windows\ �� SIGService_2010.sln ���r���h�ł��Ă��āCVS2010�p�� SIGService.lib �� Debug_2010\ �� Release_2010\ �̂ǂ��炩�̃t�H���_�ɐ�������Ă��邱�ƁC�Ƃ����̂��O��ł��D

## Author  
 National Institute of Informatics  

## Wrote by  
 Nozaki, 12th 2015 March. 