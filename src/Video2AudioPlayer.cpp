#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include <wx/timer.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <portaudio.h>

using namespace std;
namespace fs = std::filesystem;

class DualVideoPlayerFrame : public wxFrame {
public:
    DualVideoPlayerFrame(const wxString &title, 
                         const string &videoPath1, 
                         const string &videoPath2,
                         const string &audioPath, 
                         int width, int height, int fps);
    ~DualVideoPlayerFrame();

private:
    // Audio Callback Function
    static int AudioCallback(const void *inputBuffer, void *outputBuffer,
                             unsigned long framesPerBuffer,
                             const PaStreamCallbackTimeInfo* timeInfo,
                             PaStreamCallbackFlags statusFlags,
                             void *userData);

    void OnTimer(wxTimerEvent &event);
    void OnPaint(wxPaintEvent &event);

    // Button Event Handlers
    void OnPlay(wxCommandEvent &event);
    void OnPause(wxCommandEvent &event);
    void OnStop(wxCommandEvent &event);
    void OnStep(wxCommandEvent &event);

    wxTimer *frameTimer;
    wxScrolledWindow *leftScrolledWindow;
    wxScrolledWindow *rightScrolledWindow;
    
    // Video File Handling
    ifstream videoFile1;
    ifstream videoFile2;
    int frameWidth;
    int frameHeight;
    int fps;
    int totalFrames1;
    int totalFrames2;
    int currentFrame;
    vector<unsigned char> frameBuffer1;
    vector<unsigned char> frameBuffer2;
    wxImage currentImage1;
    wxImage currentImage2;

    // Audio File Handling
    ifstream audioFile;
    vector<short> audioBuffer;
    size_t audioPosition;
    size_t totalAudioSamples;

    // PortAudio Stream
    PaStream *audioStream = nullptr;
    PaStreamParameters outputParameters;

    // Buttons
    wxButton *playButton;
    wxButton *pauseButton;
    wxButton *stopButton;
    wxButton *stepButton;

    bool isPaused;
    bool isStopped;

    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(DualVideoPlayerFrame, wxFrame)
    EVT_BUTTON(wxID_HIGHEST + 1, DualVideoPlayerFrame::OnPlay)
    EVT_BUTTON(wxID_HIGHEST + 2, DualVideoPlayerFrame::OnPause)
    EVT_BUTTON(wxID_HIGHEST + 3, DualVideoPlayerFrame::OnStop)
    EVT_BUTTON(wxID_HIGHEST + 4, DualVideoPlayerFrame::OnStep)
wxEND_EVENT_TABLE()

class VideoPlayerApp : public wxApp {
public:
    bool OnInit() override;
};

// Constructor Implementation
DualVideoPlayerFrame::DualVideoPlayerFrame(
    const wxString &title, 
    const string &videoPath1, 
    const string &videoPath2,
    const string &audioPath, 
    int width, int height, int fps
) : wxFrame(NULL, wxID_ANY, title), 
    frameWidth(width), 
    frameHeight(height), 
    fps(fps),
    currentFrame(0),
    audioPosition(0),
    isPaused(false),
    isStopped(false)
{
    // Initialize PortAudio
    Pa_Initialize();

    // Open video files
    videoFile1.open(videoPath1, ios::binary);
    videoFile2.open(videoPath2, ios::binary);
    if (!videoFile1.is_open() || !videoFile2.is_open()) {
        cerr << "Error opening video files" << endl;
        exit(1);
    }

    // Open audio file (44.1 kHz, 16-bit PCM WAV)
    audioFile.open(audioPath, ios::binary);
    if (!audioFile.is_open()) {
        cerr << "Error opening audio file" << endl;
        exit(1);
    }

    // Skip WAV header (assumes standard 44-byte header)
    audioFile.seekg(44);

    // Read audio samples
    audioFile.seekg(0, ios::end);
    size_t fileSize = static_cast<size_t>(audioFile.tellg()) - 44; // Subtract header size
    totalAudioSamples = fileSize / sizeof(short);
    audioBuffer.resize(totalAudioSamples);

    audioFile.seekg(44);
    audioFile.read(reinterpret_cast<char*>(audioBuffer.data()), fileSize);

    // Calculate total frames for both videos
    videoFile1.seekg(0, ios::end);
    long fileSize1 = videoFile1.tellg();
    totalFrames1 = fileSize1 / (width * height * 3);
    videoFile1.seekg(0, ios::beg);

    videoFile2.seekg(0, ios::end);
    long fileSize2 = videoFile2.tellg();
    totalFrames2 = fileSize2 / (width * height * 3);
    videoFile2.seekg(0, ios::beg);

    // Prepare frame buffers
    frameBuffer1.resize(width * height * 3);
    frameBuffer2.resize(width * height * 3);

    // Setup PortAudio output
    outputParameters.device = Pa_GetDefaultOutputDevice();
    outputParameters.channelCount = 1; // Mono
    outputParameters.sampleFormat = paInt16;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    // Create layout
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *videoSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create scrolled windows for videos
    leftScrolledWindow = new wxScrolledWindow(this, wxID_ANY);
    rightScrolledWindow = new wxScrolledWindow(this, wxID_ANY);
    
    leftScrolledWindow->SetScrollbars(10, 10, width, height);
    leftScrolledWindow->SetVirtualSize(width, height);
    rightScrolledWindow->SetScrollbars(10, 10, width, height);
    rightScrolledWindow->SetVirtualSize(width, height);

    // Bind paint events
    leftScrolledWindow->Bind(wxEVT_PAINT, &DualVideoPlayerFrame::OnPaint, this);
    rightScrolledWindow->Bind(wxEVT_PAINT, &DualVideoPlayerFrame::OnPaint, this);

    // Add video windows to sizer
    videoSizer->Add(leftScrolledWindow, 1, wxEXPAND | wxALL, 5);
    videoSizer->Add(rightScrolledWindow, 1, wxEXPAND | wxALL, 5);

    // Create buttons
    playButton = new wxButton(this, wxID_HIGHEST + 1, "Play");
    pauseButton = new wxButton(this, wxID_HIGHEST + 2, "Pause");
    stopButton = new wxButton(this, wxID_HIGHEST + 3, "Stop");
    stepButton = new wxButton(this, wxID_HIGHEST + 4, "Step");

    // Add buttons to button sizer
    buttonSizer->Add(playButton, 1, wxALL | wxEXPAND, 5);
    buttonSizer->Add(pauseButton, 1, wxALL | wxEXPAND, 5);
    buttonSizer->Add(stopButton, 1, wxALL | wxEXPAND, 5);
    buttonSizer->Add(stepButton, 1, wxALL | wxEXPAND, 5);

    // Add sizers to main sizer
    mainSizer->Add(videoSizer, 1, wxEXPAND);
    mainSizer->Add(buttonSizer, 0, wxEXPAND);

    // Set sizer and frame properties
    SetSizer(mainSizer);
    SetClientSize(width * 2, height + 50);
    SetBackgroundColour(*wxBLACK);

    // Create and start timer for frame playback
    frameTimer = new wxTimer(this);
    Bind(wxEVT_TIMER, &DualVideoPlayerFrame::OnTimer, this, frameTimer->GetId());
}

// Destructor
DualVideoPlayerFrame::~DualVideoPlayerFrame() {
    // Cleanup
    Pa_StopStream(audioStream);
    Pa_CloseStream(audioStream);
    Pa_Terminate();
}

// Audio Callback
int DualVideoPlayerFrame::AudioCallback(const void *inputBuffer, void *outputBuffer,
                                        unsigned long framesPerBuffer,
                                        const PaStreamCallbackTimeInfo* timeInfo,
                                        PaStreamCallbackFlags statusFlags,
                                        void *userData)
{
    DualVideoPlayerFrame *frame = static_cast<DualVideoPlayerFrame*>(userData);
    short *out = static_cast<short*>(outputBuffer);

    if (frame->isPaused || frame->isStopped) {
        memset(out, 0, framesPerBuffer * sizeof(short));
        return paContinue;
    }

    unsigned long framesToCopy = min(framesPerBuffer, 
        static_cast<unsigned long>(frame->totalAudioSamples - frame->audioPosition));

    for (unsigned long i = 0; i < framesToCopy; ++i) {
        out[i] = frame->audioBuffer[frame->audioPosition + i];
    }

    // Zero out remaining buffer if needed
    for (unsigned long i = framesToCopy; i < framesPerBuffer; ++i) {
        out[i] = 0;
    }

    frame->audioPosition += framesToCopy;

    // Reset audio if reached end
    if (frame->audioPosition >= frame->totalAudioSamples) {
        frame->audioPosition = 0;
    }

    return paContinue;
}

// Timer Event Handler
void DualVideoPlayerFrame::OnTimer(wxTimerEvent &event) {
    if (!isPaused && !isStopped) {
        // Read first video frame
        if (!videoFile1.read(reinterpret_cast<char*>(frameBuffer1.data()), 
                             frameWidth * frameHeight * 3)) {
            videoFile1.clear();
            videoFile1.seekg(0, ios::beg);
        }
        currentImage1 = wxImage(frameWidth, frameHeight, frameBuffer1.data(), true);

        // Read second video frame
        if (!videoFile2.read(reinterpret_cast<char*>(frameBuffer2.data()), 
                             frameWidth * frameHeight * 3)) {
            videoFile2.clear();
            videoFile2.seekg(0, ios::beg);
        }
        currentImage2 = wxImage(frameWidth, frameHeight, frameBuffer2.data(), true);

        // Refresh both windows
        leftScrolledWindow->Refresh();
        rightScrolledWindow->Refresh();
        currentFrame++;
    }
}

// Paint Event Handler
void DualVideoPlayerFrame::OnPaint(wxPaintEvent &event) {
    wxObject* obj = event.GetEventObject();
    wxBufferedPaintDC dc(static_cast<wxWindow*>(obj));
    
    // Determine which window is being painted
    if (obj == leftScrolledWindow) {
        if (currentImage1.IsOk()) {
            wxBitmap frameBitmap(currentImage1);
            dc.DrawBitmap(frameBitmap, 0, 0, false);
        }
    }
    else if (obj == rightScrolledWindow) {
        if (currentImage2.IsOk()) {
            wxBitmap frameBitmap(currentImage2);
            dc.DrawBitmap(frameBitmap, 0, 0, false);
        }
    }
}

// Play Button Event Handler
void DualVideoPlayerFrame::OnPlay(wxCommandEvent &event) {
    if (isStopped) {
        videoFile1.clear();
        videoFile1.seekg(0, ios::beg);
        videoFile2.clear();
        videoFile2.seekg(0, ios::beg);
        audioFile.clear();
        audioFile.seekg(44);
        currentFrame = 0;
        audioPosition = 0;
        isStopped = false;
    }
    
    isPaused = false;

    // Open audio stream if not already open
    if (audioStream == nullptr) {
        Pa_OpenStream(
            &audioStream,
            NULL,
            &outputParameters,
            88200, // Sample rate
            paFramesPerBufferUnspecified,
            paClipOff,
            AudioCallback,
            this
        );
    }

    Pa_StartStream(audioStream);
    frameTimer->Start(1000 / fps);
}

// Pause Button Event Handler
void DualVideoPlayerFrame::OnPause(wxCommandEvent &event) {
    isPaused = true;
    frameTimer->Stop();
    Pa_StopStream(audioStream);
}

// Stop Button Event Handler
void DualVideoPlayerFrame::OnStop(wxCommandEvent &event) {
    isStopped = true;
    frameTimer->Stop();
    Pa_StopStream(audioStream);
    
    currentFrame = 0;
    audioPosition = 0;
    
    videoFile1.clear();
    videoFile1.seekg(0, ios::beg);
    videoFile2.clear();
    videoFile2.seekg(0, ios::beg);
    audioFile.clear();
    audioFile.seekg(44);
}

// Step Button Event Handler
void DualVideoPlayerFrame::OnStep(wxCommandEvent &event) {
    if (isPaused) {
        // Step first video
        if (videoFile1.read(reinterpret_cast<char*>(frameBuffer1.data()), 
                            frameWidth * frameHeight * 3)) {
            currentImage1 = wxImage(frameWidth, frameHeight, frameBuffer1.data(), true);
            leftScrolledWindow->Refresh();
        }

        // Step second video
        if (videoFile2.read(reinterpret_cast<char*>(frameBuffer2.data()), 
                            frameWidth * frameHeight * 3)) {
            currentImage2 = wxImage(frameWidth, frameHeight, frameBuffer2.data(), true);
            rightScrolledWindow->Refresh();
        }

        currentFrame++;
    }
}

// Main Application Initialization
bool VideoPlayerApp::OnInit() {
    wxInitAllImageHandlers();

    if (wxApp::argc != 7) {
        cerr << "Usage: " << wxApp::argv[0] 
             << " <video_path1> <video_path2> <audio_path> <width> <height> <fps>" << endl;
        return false;
    }

    string videoPath1 = wxApp::argv[1].ToStdString();
    string videoPath2 = wxApp::argv[2].ToStdString();
    string audioPath = wxApp::argv[3].ToStdString();
    int width = stoi(wxApp::argv[4].ToStdString());
    int height = stoi(wxApp::argv[5].ToStdString());
    int fps = stoi(wxApp::argv[6].ToStdString());

    DualVideoPlayerFrame *frame = new DualVideoPlayerFrame(
        "Dual RGB Video Player with Audio", 
        videoPath1, 
        videoPath2,
        audioPath, 
        width, height, fps
    );
    frame->Show(true);

    return true;
}

wxIMPLEMENT_APP(VideoPlayerApp);