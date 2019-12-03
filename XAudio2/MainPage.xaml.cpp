//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace XAudio2;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409
static const int sampleRate = 44100;
static const int seconds = 5;
static const int channels = 2;
static const int samples = seconds * sampleRate;

short volumeSoundData[samples * channels];
short pitchSoundData[samples * channels];
short spaceSoundData[samples * channels];
short timbreSoundData[samples * channels];

MainPage::MainPage()
{
	InitializeComponent();
	// Создаем объект IXAudio2
	HRESULT hr = XAudio2Create(&pXAudio2);
	if (FAILED(hr))
	{
		errorText->Text = "XAudio2Create failure: " + hr.ToString();
		return;
	}
	// Создаем мастеринговый голос
	hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice);
	if (FAILED(hr))
	{
		errorText->Text = "CreateMasteringVoice failure: " + hr.ToString();
		return;
	}
	// Создаем голос источника
	WAVEFORMATEX waveformat;
	waveformat.wFormatTag = WAVE_FORMAT_PCM;
	waveformat.nChannels = 1;
	waveformat.nSamplesPerSec = 44100;
	waveformat.nAvgBytesPerSec = 44100 * 2;
	waveformat.nBlockAlign = 2;
	waveformat.wBitsPerSample = 16;
	waveformat.cbSize = 0;
	hr = pXAudio2->CreateSourceVoice(&pSourceVoice, &waveformat);
	if (FAILED(hr))
	{
		errorText->Text = "CreateSourceVoice failure: " + hr.ToString();
		return;
	}
	// Запускаем голос источника
	hr = pSourceVoice->Start();
	if (FAILED(hr))
	{
		errorText->Text = "Start failure: " + hr.ToString();
		return;
	}
	// Заполняем массив звуковыми данными
	/*
	for (int index = 0, sample = 0; sample < samples; sample++)
	{
		double t = 1;
		if (sample < 2 * samples / 5)
			t = sample / (2.0 * samples / 5);
		else if (sample > 3 * samples / 5)
			t = (samples - sample) / (2.0 * samples / 5);
		double amplitude = pow(2, 15 * t) - 1;
		short waveform = sample % 100 < 50 ? 1 : -1;
		short value = short(amplitude * waveform);
		volumeSoundData[index++] = value;
		volumeSoundData[index++] = value;
	}
	*/
	for (int index = 0, second = 0; second < 5; second++)
	{
		for (int cycle = 0; cycle < 441; cycle++)
		{
			for (int sample = 0; sample < 100; sample++)
			{
				short value = sample < 50 ? 32767 : -32768;
				soundData[index++] = value & 0xFF;
				soundData[index++] = (value >> 8) & 0xFF;
			}
		}
	}
	// Делаем кнопку видимой
	submitButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
	pauseButton->Visibility = Windows::UI::Xaml::Visibility::Visible;
}

void MainPage::OnSubmitButtonClick(Object^ sender, RoutedEventArgs^ args)
{
	pSourceVoice->Start();
	// Создаем кнопку для ссылки на байтовый массив
	XAUDIO2_BUFFER buffer = { 0 };
	buffer.AudioBytes = 2 * 5 * 44100;
	buffer.pAudioData = soundData;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.PlayBegin = 0;
	buffer.PlayLength = 5 * 44100;
	buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
	// Передаем буфер
	HRESULT hr = pSourceVoice->SubmitSourceBuffer(&buffer);
	if (FAILED(hr))
	{
		errorText->Text = "SubmitSourceBuffer failure: " + hr.ToString();
		submitButton->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		return;
	}
}

void MainPage::OnPauseButtonClick(Platform::Object^ sender,
	Windows::UI::Xaml::RoutedEventArgs^ args)
{
	pSourceVoice->Stop();
	pSourceVoice->FlushSourceBuffers();
}