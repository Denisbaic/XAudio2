//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"


namespace XAudio2
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>


	public ref class MainPage sealed
	{
	private:


		Microsoft::WRL::ComPtr<IXAudio2> pXAudio2;
		IXAudio2MasteringVoice * pMasteringVoice;
		IXAudio2SourceVoice * pSourceVoice;
		byte soundData[2 * 5 * 44100];
	public:
		MainPage();
	private:
		void OnPauseButtonClick(Platform::Object^ sender,
			Windows::UI::Xaml::RoutedEventArgs^ args);
		void OnSubmitButtonClick(Platform::Object^ sender,
			Windows::UI::Xaml::RoutedEventArgs^ args);
	};
}
