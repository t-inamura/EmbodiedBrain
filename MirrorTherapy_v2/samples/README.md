#Controller#

Controller �̎�������i�[���Ă��܂��D  
SIGVerse �� man-nii �̃A�o�^�[���CKinectV2 �� OculusRiftDK1 �ő��삷�� Controller �̎�����ł��D  

�iUbunutu�̒[���Łj  
�e�f�B���N�g������ make ����ƁC�e Controller �Ń����N���鋤�L�I�u�W�F�N�g���C�u�����i.so�j�𐶐����܂��D  
���̌�C sigserver.sh -w ./world.xml �ƃR�}���h����͂���ƁCSIGServer ���N�����C  
 Plugin ����̃��b�Z�[�W�̎�M�҂���ԂɂȂ�܂��D  
���̌�CSIGViewer �� Connect �{�^�����N���b�N���� START �{�^�����N���b�N���C Services ����e Plugin ���X�^�[�g������ƁC  
�e Controller �����삵�܂��D

##ManNiiAvatarControllerByKinectV2##

KinectV2 ���g���ăA�o�^�[�𑀍삷��Controller�̎�����ł��D  
Plugin ���瑗���郁�b�Z�[�W���󂯎�������̏����́C��Ɏ��̒ʂ�ł��D

* KinectV2_vs2013.sig ���g�p���đ��M����郁�b�Z�[�W���󂯎��D
* �󂯎�������b�Z�[�W�Ɋ�Â��� KinectV2SensorData �N���X�̃C���X�^���X�𐶐��idecode�j����D
* KinectV2SensorData �iKinectV2 �Ōv�����ꂽ�S�g�̃N�H�[�^�j�I���j���Cman-nii �̃N�H�[�^�j�I���֕ϊ�����D
* man-nii �̃N�H�[�^�j�I�����CSIGViewer ��̃A�o�^�[�փZ�b�g����D


##ManNiiAvatarControllerByOculus##

OculusRiftDK1 ���g���ăA�o�^�[�𑀍삷��Controller�̎�����ł��D  
Plugin ���瑗���郁�b�Z�[�W���󂯎�������̏����́C��Ɏ��̒ʂ�ł��D

* OculusRiftDK1_vs2010.sig ���g�p���đ��M����郁�b�Z�[�W���󂯎��D
* �󂯎�������b�Z�[�W�Ɋ�Â��� OculusRiftDK1SensorData �N���X�̃C���X�^���X�𐶐��idecode�j����D
* OculusRiftDK1SensorData �iOculus �̎p���������I�C���[�p�j���Cman-nii �̎�̃N�H�[�^�j�I���փZ�b�g����D
* man-nii �̎�̃N�H�[�^�j�I�����CSIGViewer ��̃A�o�^�[�փZ�b�g����D

##MirrorTherapyController##

KinectV2 �� OculusRiftDK1 �̗����i���邢�͂��̂ǂ��炩�j���g���ăA�o�^�[�𑀍삵�CMirrorTherapy ���s��������ł��D  
��L2�̃R���g���[�������@�\�ɉ����āC���̂悤�ȋ@�\����Ɏ������Ă��܂��D

* �����E�ł̉E�r�̓���𔽓]�����ăA�o�^�[�̍��r�֓`����@�\�i�����E�ł̍��r�̓���͖��������j
* �����E�ł̍��r�̓���𔽓]�����ăA�o�^�[�̉E�r�֓`����@�\�i�����E�ł̉E�r�̓���͖��������j
* �����E�ł̂ǂ��炩�̘r�̓���𔽓]�����ē`����ۂɁC�w��̕b[ms]�����x�点�ē`����@�\
* ����̔��]���[�h�i�E�r�̓�������r�ցC���r�̓�����E�r�ցC�����E�̓�������̂܂ܓ`����i���]���Ȃ��j�C��3�j��  
SIGViewer ���瑗�M���郁�b�Z�[�W�ɂ���ĕύX����@�\
* �����x�点��b���i����̒x�����ԁj���CSIGViewer ���瑗�M���郁�b�Z�[�W�ɂ���ĕύX����@�\

##ControllerCommon##

��L3�� Controller �ŋ��ʂ���@�\�����N���X�����������\�[�X�R�[�h���i�[���Ă��܂��D

* AvatarController �N���X�̎������܂܂�Ă��܂��D
 * ��L3��Controller�́CAvatarController �N���X���p�����Ă��܂��D
* Posture �N���X�̎������܂܂�Ă��܂��D
* Posture �N���X���p������ ManNiiPosture �N���X�̎������܂܂�Ă��܂��D
 * man-nii �̎p���̏��́C ManNiiPosture �N���X�ň����܂��D