#include "ImitationMotionAccumulatorGUIMainFrame.h"

ImitationMotionAccumulatorGUIMainFrame::ImitationMotionAccumulatorGUIMainFrame( wxWindow* parent, const int argc, const wxArrayString &argv )
:
MainFrame( parent )
{

}

void ImitationMotionAccumulatorGUIMainFrame::OnMainFrameClose( wxCloseEvent& event )
{
	Destroy();
}

void ImitationMotionAccumulatorGUIMainFrame::OnMirrorMirrorTherapyClick( wxCommandEvent& event )
{
// TODO: Implement OnMirrorMirrorTherapyClick
}

void ImitationMotionAccumulatorGUIMainFrame::OnImitationModeSelect( wxCommandEvent& event )
{
	int modeNo = m_radioBox_mode->GetSelection();

	std::string msg = "radioBox_mode=" + std::to_string(modeNo);

	switch (modeNo)
	{
		case 0 : 
		{
			mm_textCtrl_origin_max_time      ->Enable( true );
			mm_textCtrl_imitation_group_id   ->Enable( false );
			imitation_rec_type               ->Enable( false );
			imitation_imitation_origin_rec_id->Enable( false );

			mm_textCtrl_db_imitation_condition_pulse_power    ->Enable( false );
			mm_textCtrl_db_imitation_condition_pulse_frequency->Enable( false );
			mm_textCtrl_db_imitation_condition_pulse_duration ->Enable( false );
			mm_textCtrl_db_imitation_condition_pulse_interval ->Enable( false );
			mm_textCtrl_db_imitation_condition_pulse_number   ->Enable( false );
			mm_textCtrl_db_imitation_memo                     ->Enable( false );

			break;
		}
		case 1 : 
		{
			mm_textCtrl_origin_max_time      ->Enable( false );
			mm_textCtrl_imitation_group_id   ->Enable( true );
			imitation_rec_type               ->Enable( true );
			imitation_imitation_origin_rec_id->Enable( true );

			mm_textCtrl_db_imitation_condition_pulse_power    ->Enable( true );
			mm_textCtrl_db_imitation_condition_pulse_frequency->Enable( true );
			mm_textCtrl_db_imitation_condition_pulse_duration ->Enable( true );
			mm_textCtrl_db_imitation_condition_pulse_interval ->Enable( true );
			mm_textCtrl_db_imitation_condition_pulse_number   ->Enable( true );
			mm_textCtrl_db_imitation_memo                     ->Enable( true );

			break;
		}
	}
}
