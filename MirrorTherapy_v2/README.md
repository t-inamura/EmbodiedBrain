MirrorTherapy_v2
================

���߂ɁC(1) Controller, (2) Plugin, (3) Common��3�̃f�B���N�g���ɂ��Đ������܂��D  
���̌�C(4) ��{�I�Ȏg�����i�\�[�X�R�[�h�̃r���h���@�Ǝ��s���@�̗�j�ɂ��Đ������܂��D

(1) Controller
--------------
�R���g���[���̃\�[�X�R�[�h�̃f�B���N�g���ł��D  
Ubuntu��SIGServer���N�����CPlugin���瑗�M����郁�b�Z�[�W���󂯎���āCSIGViewer���man-nii�𑀍삷�邱�Ƃ��ł��܂��D

(2) Plugin
----------
�T�[�r�X�v���o�C�_�̃\�[�X�R�[�h�̃f�B���N�g���ł��D
���������T�[�r�X�v���o�C�_�̋@�\�͎��̂悤�Ȃ��̂ł��D

* Windows�œ��삷��KinectV2��OculusRiftDK1�̏���Controller�֑��M����D  
 * KinectV2�p�̃T�[�r�X�v���o�C�_�ł́CKinectV2��SDK�ŎZ�o�����N�H�[�^�j�I���𑗐M����D  
 * OculusRiftDK1�p�̃T�[�r�X�v���o�C�_�ł́COculus��SDK�ŎZ�o������̃I�C���[�p�𑗐M����D

(3) Common
----------
Controller��Plugin�̗����ɕK�v�ȃ\�[�X�R�[�h�̃f�B���N�g���ł��D  
SensorData�N���X�̎������܂܂�Ă��܂��D  
���ӓ_�Ȃǂ����ɗ񋓂��܂��D

* Window �� Ubuntu �̗����Ŏg�p����D
* �����ɂ� boost ���܂܂�Ă���̂ŁC�\�[�X�R�[�h�̃r���h�̂��߂� Ubuntu �� boost ���C���X�g�[������K�v������D
* Ubuntu �ւ� boost �̃C���X�g�[����i�[���őłR�}���h�j�����Ɏ����D

` sudo apt-get install boost-all-dev


(4) ��{�I�Ȏg����
-----------------

���|�W�g���� Clone ���CMirrorTherapy_v2 �̑S�\�[�X�R�[�h���r���h����ƁC���̕\�́u���s�t�@�C�����v�Ɏ������s�t�@�C���������܂��D  
���\�ɁC���s�t�@�C���̎�ށiPlugin��Windows�p�CController��Ubuntu�p�j�ƊȒP�ȊT�v���L�ڂ��܂����D

| - | ���s�t�@�C����  | Plugin / Controller | �T�v |
| ---------- | ------------- | ------------- | ------------- |
|a| KinectV2_vs2013.sig  | Plugin  | KinectV2�̃N�H�[�^�j�I����Controller�֑��M���� |
|b| OculusRiftDK1_vs2010.sig  | Plugin  | OculusRiftDK1�̃I�C���[�p��Controller�֑��M���� |
|c| ManNiiAvatarControllerByKinectV2.so  | Controller | KinectV2�̃N�H�[�^�j�I������M����SIGViewer��̃A�o�^�[�𑀍삷�� |
|d| ManNiiAvatarControllerByOculus.so  | Controller | Oculus�̃I�C���[�p����M����SIGViewer��̃A�o�^�[�𑀍삷�� |
|e| MirrorTherapyController.so  | Controller | KinectV2��Oculus�𓯎��ɗ��p����SIGViewer��̃A�o�^�[�̓�������H���� |

��{�I�ɂ́C���̂悤�Ȏ菇�Ńr���h���Ď��s���܂��D

1. �g�p����@��iKinect V2 �� OculusRiftDK1�j�� PC �ɐڑ�����iOculus�̓L�����u���[�V���������Ă����� better�j�D
2. Windows���ŁCC:\SIGVerse\src\ �����Ƀ��|�W�g���� Clone ����D
3. Windows���ŁC��̕\�� a �� b �𐶐��i�r���h�j����D
4. Windows���ŁCSIGViewer ���N�����C��̕\�� a �� b �� SIGViewer �� Services �� Add ����D
5. Ubuntu���ŁC/home/sigverse/ �ȉ��̔C�ӂ̃f�B���N�g���Ƀ��|�W�g���� Clone ����D
6. Ubuntu���ŁC��̕\�� c, d, e �𐶐��i�r���h�j����D
7. Ubuntu���ŁCSIGServer ���N������iSIGServer �̃��[���h�t�@�C���� c, d, e �̂����ꂩ�������N����j�D
8. Windows���ŁCSIGViewer �� IP Address �� Port Number ���w�肵�� Connect ����D
9. Windows���ŁCSIGViewer �� START ���N���b�N���� Services -> Start �� a �� b �̂����ꂩ�C���邢�͗������N���b�N���� Plugin �����s����D
10. �i�ȍ~�͏I�����̎菇�j Windows���ŁCSIGViewer �� STOP ���N���b�N����D
11. Windows���ŁCPlugin ���s���ɊJ�����R�}���h�v�����v�g���N���b�N���āi�A�N�e�B�u�ɂ��āj�C�L�[�{�[�h�� Ctrl+C ����������D
12. Windows���ŁC SIGViewer �� Quit Simulation ���N���b�N����D

�菇�� 3 �� 6 �Ńr���h���܂����C�r���h�̗�͂��ꂼ��� Plugin / Controller �̃f�B���N�g�����ɋL�ڂ��Ă��܂��̂ŁC�r���h�̍ۂ͂�������Q�l�ɂ��Ă��������D  
���s�t�@�C���Q�i.sig �� .so�j���r���h�ς݂̏ꍇ�́C�菇�� 2 ���� 6 �͕K�v����܂���D