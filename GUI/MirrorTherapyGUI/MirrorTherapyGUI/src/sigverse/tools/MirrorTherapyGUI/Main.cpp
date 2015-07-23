/*
 * データベースの「モデル動作情報(m_motion_info_of_model)」テーブルの「アノテーション」カラムに値を設定するGUIプログラム。
 * 人物モデルの確認用にSIGVerseを使用しているため、SIGVerseのサービスプロバイダとして実装する。
 *
 * ＜流れ＞
 * 1. SIGServerとSIGViewerを起動し、サービスプロバイダとして本プログラムを起動する
 * 2. アノテーションを設定したい動作情報の動作ID(motion_id)を入力し、[Get Motion data]ボタンをクリックする
 *    （このとき、既にアノテーションを設定済みの場合、アノテーション表に既存のアノテーション情報が表示される）
 * 3. アバター操作スライダーを動かして、アノテーション開始時間・終了時間を調整し、アノテーションを入力し、[Add]ボタンをクリックする
 * 4. 手順3を繰り返しモデル動作収録時間の最後までアノテーションを設定する
 * 5. [Update DB]ボタンをクリックし、データベースを更新する
 *
 * ＜引数＞
 * 第1引数：IPアドレス、第2引数：ポート番号
 * 
 * ＜その他＞
 * ・設定ファイル名は、config.ini
 * ・GUIの設計には、wxFormBuilderを使用している（MirrorTherapyGUI.hとMirrorTherapyGUI.cppはwxFormBuilderにより生成している）
 */
#include <sigverse/tools/MirrorTherapyGUI/Param.h>
#include <sigverse/tools/MirrorTherapyGUI/gui/MirrorTherapyGUIMainFrame.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

const std::string Param::PARAM_FILE_NAME = "MirrorTherapyGUI.ini";

std::string Param::sigServiceName;


/*
 * メインクラス
 */
class MainApp : public wxApp
{
private :

public:
	bool OnInit();

	void OnInitCmdLine(wxCmdLineParser &parser);
	bool OnCmdLineParsed(wxCmdLineParser &parser);
};


IMPLEMENT_APP( MainApp )


/*
 * メイン初期化処理
 */
bool MainApp::OnInit()
{
	if ( !wxApp::OnInit() ){ return false; }


	MirrorTherapyGUIMainFrame *frame = new MirrorTherapyGUIMainFrame(NULL, wxGetApp().argc, wxGetApp().argv.GetArguments());
	
	frame->Show();

	return true;
}

/*
 * コマンドライン引数の初期化処理
 */
void MainApp::OnInitCmdLine(wxCmdLineParser &parser)
{
	wxCmdLineEntryDesc g_cmdLineDesc [] =
	{
		{ wxCMD_LINE_PARAM,  NULL, NULL, NULL, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE },
		{ wxCMD_LINE_NONE }
	};

	parser.SetDesc(g_cmdLineDesc);
}


bool MainApp::OnCmdLineParsed(wxCmdLineParser &parser)
{
	return true;
}
