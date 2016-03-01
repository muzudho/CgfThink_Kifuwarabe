/*** cgf.h ***/
#define CGF_E		// �p��ł͂�����`�B���\�[�X�͉p��ŁA���{��ł𓝈�B
//#define CGF_DEV	// �J����

#define BAN_9   9 // �X�H��
#define BAN_13 13 // 13�H��
#define BAN_19 19 // 19�H��
#define BANMAX ((19+2)*256)	// = 21*256		������21*32=672�ɂ������B8����1�ɂȂ邵�B10bit�ō��W��\���B��͂�ʓ|���E�E�E

#define KIFU_MAX 2048	// �L�^�\�ȍő�萔

extern int ban_size;	// �Ղ̑傫�� 9,13,19;
extern int tesuu;		// �萔
extern int all_tesuu;	// �Ō�܂ł̎萔
extern int kifu[KIFU_MAX][3];		// ����	 [0]...�ʒu�A[1]...�F�A[2]...�v�l���ԁi���̎�́j


// make_move()�֐��Ŏ��i�߂����̌���
enum MoveResult {
	MOVE_SUCCESS,	// ����
	MOVE_SUICIDE,	// ���E��
	MOVE_KOU,		// �R�E
	MOVE_EXIST,		// ���ɐ΂�����
	MOVE_FATAL,		// ����ȊO

	// �ȉ���set_kifu_time()�ŗ��p
	MOVE_PASS_PASS,	// �A���p�X�A�΋ǏI��
	MOVE_KIFU_OVER,	// �萔��������
	MOVE_RESIGN,	// ����
};


extern int kou_iti;			// ���ɂȂ�ꍇ�̈ʒu
extern int hama[2];			// ���Ɣ��̗g�l�i���΂̐��j

extern int ban[];			// �͌�Ղ̏�ԁB�� 0�A�� 1�A�� 2�A�� 3
extern int ban_start[];		// �͌�Ղ̊J�n��ԁB
extern int dll_col;			// dll������(1)����(2)

extern double komi;			// �R�~�B6.5��6�ڔ��B0�Ōݐ�

extern int thinking_utikiri_flag;	// ���f�t���O
extern int total_time[2];	// �݌v�̎v�l����
						
extern int ban_9[][11];		// �����Ֆʔz�u���
extern int ban_13[][15];
extern int ban_19[][21];


#define UN_COL(col) (3-(col))	// ����̐΂̐F


#define SGF_BUF_MAX 65536			// SGF��ǂݍ��ނ��߂̃o�b�t�@
extern unsigned long nSgfBufSize;	// �o�b�t�@�Ƀ��[�h�����T�C�Y
extern unsigned long nSgfBufNum;	// �o�b�t�@�̐擪�ʒu������
extern char SgfBuf[SGF_BUF_MAX];	// �o�b�t�@

#define IsKanji(c) (( 0x80<(c) && (c)<0xa0 ) || ( 0xdf<(c) && (c)<0xfd ))	// �����̐擪�o�C�g���H c ��unsigned char�łȂ��ƃo�O��

#define TMP_BUF_LEN	256			// ������p�̓K���Ȕz�񒷂�

// cgf_term.cpp
extern int RetZ;		// �ʐM�Ŏ󂯎���̍��W
extern int RetComm;		// �A���Ă����f�[�^�̎��
extern int nCommNumber;	// COM�|�[�g�̔ԍ��B




