MirrorTherapy_v2
=============

Controller
----------
�R���g���[���ɕK�v�ȃ\�[�X�R�[�h�̃f�B���N�g���ł��D

* ���̂Ƃ���CUbuntu��SIGServer���N�����Cman-nii���R���g���[������������܂܂�Ă��܂��D  
* ��̓I�ɂ́CPlugin���瑗�M����郁�b�Z�[�W���󂯎���āCSIGViewer���man-nii�𓮂����܂��D  

Plugin
------
�T�[�r�X�v���o�C�_�̃\�[�X�R�[�h�̃f�B���N�g���ł��D

* ���̂Ƃ���CWindows�œ��삷��KinectV2��OculusRiftDK1�̏���Controller�֑��M����������܂܂�Ă��܂��D  
* KinectV2�p�̃T�[�r�X�v���o�C�_�ł́CKinectV2��SDK�ŎZ�o�����N�H�[�^�j�I���𑗐M���܂��D  
* OculusRiftDK1�p�̃T�[�r�X�v���o�C�_�ł́COculus��SDK�ŎZ�o������̃I�C���[�p�𑗐M���܂��D


Common
------
Controller��Plugin�̗����ɕK�v�ȃ\�[�X�R�[�h�̃f�B���N�g���ł��D

* SensorData�N���X�̎������܂܂�Ă��܂��D


SensorData�N���X�ɂ���
------------------------
SensorData�N���X�́C�f�o�C�X���瓾������̂����CController�֑��M������݂̂����������������܂��D  
SensorData�N���X�̃T�u�N���X�ł́C����2�̃��\�b�h����������K�v������܂��D
1. �f�o�C�X���瓾����Z���T�̏��i�p���Ȃǁj�����b�Z�[�W�i������j�֕ϊ��E�������郁�\�b�h�F encodeSensorData2Message()
2. ���b�Z�[�W�i������j���Z���T�̏��i�p���Ȃǁj�֕ϊ����郁�\�b�h�FdecodeMessage2SensorData(std::string &message)

