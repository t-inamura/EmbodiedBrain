#KinectV2_vs2013#

KinectV2_vs2013.sig �𐶐�����菇��������܂��D  

##�K�v�Ȋ��Ȃ�##

KinectV2_vs2013.sln ���g���� KinectV2_vs2013.sig ���r���h���邽�߂ɁC���̊��Ȃǂ��K�v�ł��D

* Windows8 �i�܂��͂���ȏ��Windows OS�j
* Visual Studio Express 2013 for Desktop �i�ȍ~��VS2013�ƕ\�L���܂��j
* VS2013 �p�Ƀr���h���ꂽ SIGService.lib  
�� VS2013 �p�� SIGService.lib ���r���h����菇�́C�����ɋL�q���Ă���܂��D
* OpenCV ���C���X�g�[������Ă��邱�Ɓi�{���|�W�g���ł� 2.4.10 ���g�p���Ă��܂����C2.4�n�Ȃ�ǂ�ł�OK���Ǝv���܂��j
* Kinect for Windows SDK 2.0 ���C���X�g�[������Ă��邱��

�Ȃ��C�uVS2013 �p�́`�`�v�ƕ\�L���Ă�����̂́C�M�҂̊��ŁuVS2013 ���g���ăr���h�������́v�Ƃ����Ӗ����܂�ł��܂��D  
�����炭�́CVS2012 �Ńr���h�������̂ł�OK���Ǝv���܂��D  

##�r���h�̎菇##

�r���h����ꍇ�́C���̎菇���Q�l�ɂ��Ă��������D  

1. GitHub ����N���[�����Ă��� KinectV2_vs2013.sln �� VS2013 �ŊJ���D
2. �v���W�F�N�g�̃A�N�e�B�u�ȍ\���� Release �ɂ���D
3. �v���W�F�N�g�̃v���p�e�B���J���āC����4�`7�ɏ]���ăC���N���[�h�ƃ����N�̐ݒ���s���D
4. �ǉ��̃C���N���[�h�f�B���N�g���ɁC���̃f�B���N�g�����w�肷��D
 * SIGService.h ������f�B���N�g��
 * MirrorTherapy_v2 �f�B���N�g��
 * boost �̃f�B���N�g��
 * KinectV2 �� SDK ���C���X�g�[������Ă���f�B���N�g�����ɂ���uinc�v�f�B���N�g��  
  �� SDK �̃o�[�W�����ɂ���āC�C���X�g�[�������i�쐬�����j�f�B���N�g�������ς��l�ł��D  
��{�I�ɂ́C�uC:\Program Files\Microsoft SDKs\Kinect\�v�f�B���N�g���ȉ��̂ǂ����ɂ���Ǝv���܂��D
 * OpenCV �� include �f�B���N�g��
5. �ǉ��̃��C�u�����f�B���N�g���ɁC���̃f�B���N�g�����w�肷��D  
 �Ȃ��C���ׂ� VS2013 �p�̃��C�u����������f�B���N�g�����w�肵�Ă��������D
 * SIGService.lib ������f�B���N�g��
 * boost �̃��C�u�����Q������f�B���N�g��
 * OpenCV �̃��C�u�����Q������f�B���N�g��
 * KinectV2 �� SDK �̃��C�u�����ikinect20.lib�j������f�B���N�g��
6. �ǉ��̈ˑ��t�@�C���ɁC���̕������ǉ�����D
 * SIGService.lib
 * kinect20.lib
 * opencv\_highgui*.lib
 * opencv\_core*.lib
 * opencv\_imgproc*.lib  
 ���u\*�v�̕����́C�C���X�g�[�����Ă��� OpenCV �̃o�[�W�����ɍ��킹�ĕύX���Ă��������D
7. �^�[�Q�b�g�̊g���q���u.exe�v����u.sig�v�֕ύX����D
8. �r���h����D
9. OpenCV ���C���X�g�[������Ă���f�B���N�g���i�f�t�H���g�ł�C:\opencv\build\x86\vc12\bin�j����C  
����3�̃t�@�C���� Release �f�B���N�g���փR�s�[����D
 * opencv\_core\*.dll
 * opencv\_highgui\*.dll
 * opencv\_imgproc\*.dll  
 ���u\*�v�̕����́C�C���X�g�[�����Ă��� OpenCV �̃o�[�W�����ɍ��킹�ĕύX���Ă��������D

##VS2013�p��SIGService.lib�ɂ���##
KinectV2 ���g���v���O�C�����r���h����ꍇ�C VS2013 �Ŏg���� SIGService.lib ���K�v�ł��D  

��W�搶�̌������̊֌W�҂̕��́CVS2013 �Ńr���h���� SIGService.lib ���ȉ�����R�s�[�ł��܂��D

* \\tera2.iir.nii.ac.jp\share\tome-nozaki\SIGServiceLibs

SIGService.lib �� VS2013 �ŉ��߂ăr���h����ۂ́C�ȉ����������������D

###VS2013�p��SIGService.lib���r���h����###
  
SIGService.lib ���r���h���邽�߂ɂ́C boost �̃��C�u������ VS2013 �p�̂��̂��K�v�ł��D  
SIGService.lib ���r���h�����͎��̒ʂ�ł��D

1. http://sourceforge.net/projects/boost/files/boost-binaries/1.55.0-build2/  
�́uboost\_1\_55\_0-msvc-12.0-32.exe�v�� DL ���ăC���X�g�[������D
2. https://github.com/SIGVerse/Client ���|�W�g���� C:\SIGVerse\GitHub\ ������ Clone ����D
3. SIGService_2010.sln �� VS2013 �ŊJ���āC�@boost ����̐ݒ�� 1. �ŃC���X�g�[������ boost �̂��̂ɂ���D
4. �iVS2010 �p�� SIGService.lib ���r���h�������Ƃ�����ꍇ�́C�����̂��̂��㏑�����Ă��܂�Ȃ��悤�ɁC  
�o�͐�� Release �f�B���N�g����ޔ����Ă����D�j
5. �r���h����D
6. �r���h���ʂ� Release �f�B���N�g���ɏo�͂���Ă��邱�Ƃ��m�F����D
7. Release �f�B���N�g���̖��̂� Release_2013 �֕ύX����D

����Git�ւ������v���W�F�N�g�t�@�C���ɂ́C���̎菇�Ő������� SIGService.lib ���g�p����ݒ�ɂȂ��Ă��܂��D
