#include "TinyEngine.h"
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <chrono>
#include <varargs.h>
#include <SDL/SDL_audio.h>

/* NOTE
 * This engine is not for any sort of serious use, it's just used as a tool for teaching basic programming
*/

// Variables controlling the SDL context
static SDL_Window* Window;
static SDL_Renderer* Renderer;
static bool IsOpen = false;

static SDL_Color CurrentColor = { 255, 255, 255, 255 };

// Delta tracking
using namespace std::chrono;
using hr_clock = high_resolution_clock;

static hr_clock::time_point LastFrameTime;
static float FrameDelta = 0.f;

// Text stuff
static TTF_Font* StandardFont = nullptr;

// Input stuff
struct InputState
{
	bool Pressed;
	int FrameNum;
};
static InputState KeyStates[(unsigned int)Key::MAX];
static InputState ButtonStates[(unsigned int)ControllerKey::MAX];
static int CurrentFrameNum = 1;
float AxisState[(unsigned int)ControllerAxis::AXIS_MAX];

void engInit()
{
	// Initalize SDL and TTF
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_GAMECONTROLLER);
	SDL_Init(SDL_INIT_AUDIO);
	TTF_Init();

	// Open window
	Window = SDL_CreateWindow("Pew Pew Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	IsOpen = true;

	// Set last frame time to now, so that delta doesnt get messed up later
	LastFrameTime = hr_clock::now();

	// Load standard font
	StandardFont = TTF_OpenFont("Zapf.ttf", 20);
}

void engClear()
{
	// Clear for next frame
	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(Renderer);
}

void engUpdate()
{
	CurrentFrameNum++;


	// Poll window events
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
			engClose();

		if (e.type == SDL_KEYDOWN)
		{
			// We dont care about repeats
			if (e.key.repeat == 0)
			{
				InputState& state = KeyStates[e.key.keysym.scancode];
				state.Pressed = true;
				state.FrameNum = CurrentFrameNum;
			}
		}

		if (e.type == SDL_KEYUP)
		{
			InputState& state = KeyStates[e.key.keysym.scancode];
			state.Pressed = false;
			state.FrameNum = CurrentFrameNum;
		}

		if (e.type == SDL_JOYAXISMOTION)
		{
			AxisState[e.jaxis.axis] = (float)e.jaxis.value / 32767.f;
		}
		

		if (e.type == SDL_JOYBUTTONDOWN)
		{		
			InputState& state = ButtonStates[e.jbutton.button];
			state.Pressed = true;
			state.FrameNum = CurrentFrameNum;
		}

		if (e.type == SDL_JOYBUTTONUP)
		{
			InputState& state = ButtonStates[e.jbutton.button];
			state.Pressed = false;
			state.FrameNum = CurrentFrameNum;
		}
	}


	// Calculate next frame delta
	hr_clock::time_point Now = hr_clock::now();
	FrameDelta = duration_cast<microseconds>(Now - LastFrameTime).count() * 1e-6f;  // 10^6 microseconds in a second
	LastFrameTime = Now;

	// Present SDL renderer
	SDL_RenderPresent(Renderer);
	engClear();

	// Do a small delay so we dont fry the CPU
	SDL_Delay(5);
}

void engClose()
{
	// 'Closes' the window (sets the flag to close, actual closing happens in Destroy
	IsOpen = false;
}

void engDestroy()
{
	// Destroys the inner APIs and does a clean exit
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);

	Renderer = nullptr;
	Window = nullptr;

	TTF_CloseFont(StandardFont);
	StandardFont = nullptr;

	SDL_Quit();
	TTF_Quit();
}

bool engIsOpen()
{
	// Return if the window is open
	return IsOpen;
}

void engDelay(int Milliseconds)
{
	// Draw whats up, so its not just black
	SDL_RenderPresent(Renderer);
	SDL_Delay(Milliseconds);
}

Key engWaitKey()
{
	// Draw whats up, so its not just black
	SDL_RenderPresent(Renderer);

	SDL_Event e;

	do
	{
		SDL_WaitEvent(&e);
	} while (e.type != SDL_KEYDOWN && e.type != SDL_QUIT);

	if (e.type == SDL_QUIT)
	{
		// We want to handle quitting messages, so people dont think we're locked
		engClose();
		return Key::Undefined;
	}
	else
		return (Key)e.key.keysym.scancode;
}

float engDelta()
{
	// Returns the deltatime for this frame
	return FrameDelta;
}

void engSetColor(uint8 Red, uint8 Green, uint8 Blue)
{
	CurrentColor.r = Red;
	CurrentColor.g = Green;
	CurrentColor.b = Blue;
	CurrentColor.a = 255;
}

void engSetColor(unsigned int Color)
{
	CurrentColor.r = (Color >> 24) & 0xFF;
	CurrentColor.g = (Color >> 16) & 0xFF;
	CurrentColor.b = (Color >> 8) & 0xFF;
	CurrentColor.a = Color & 0xFF;
}

void engDrawRect(int X, int Y, int Width, int Height)
{
	SDL_SetRenderDrawColor(Renderer, CurrentColor.r, CurrentColor.g, CurrentColor.b, CurrentColor.a);

	SDL_Rect rect = {X, Y, Width, Height};
	SDL_RenderFillRect(Renderer, &rect);
}

void engDrawString(int X, int Y, const char* String)
{
	// Render text onto surface
	SDL_Surface* MsgSurface = TTF_RenderText_Solid(StandardFont, String, CurrentColor);
	SDL_Texture* MsgTexture = SDL_CreateTextureFromSurface(Renderer, MsgSurface);

	// Find out dimensions
	int MsgW = 0, MsgH = 0;
	TTF_SizeText(StandardFont, String, &MsgW, &MsgH);

	// Copy that bad boy
	SDL_Rect MessageRect = { X, Y, MsgW, MsgH };
	SDL_RenderCopy(Renderer, MsgTexture, NULL, &MessageRect);

	// Remember to clean up
	SDL_FreeSurface(MsgSurface);
	SDL_DestroyTexture(MsgTexture);
}

void engDrawStringFormat(int X, int Y, const char* String, ...)
{
	static char Buffer[1024];

	// Vararg lists, scary stuff
	va_list vl;
	va_start(vl, String);
	vsprintf_s<1024>(Buffer, String, vl);
	va_end(vl);

	engDrawString(X, Y, Buffer);
}

bool engGetKey(Key InKey)
{
	return KeyStates[(int)InKey].Pressed;
}

bool engGetKeyDown(Key InKey)
{
	// Is pressed and changed this frame
	InputState& State = KeyStates[(int)InKey];
	return State.Pressed && State.FrameNum == CurrentFrameNum;
}

bool engGetKeyUp(Key InKey)
{
	// Is not pressed and changed this frame
	InputState& State = KeyStates[(int)InKey];
	return !State.Pressed && State.FrameNum == CurrentFrameNum;
}

bool engGetController(ControllerKey InButton)
{
	return ButtonStates[(int)InButton].Pressed;
}

bool engGetControllerDown(ControllerKey InButton)
{
	InputState& State = ButtonStates[(int)InButton];
	return State.Pressed && State.FrameNum == CurrentFrameNum;
}

float engGetAxis(ControllerAxis InAxis)
{
	float value = AxisState[(int)InAxis];
	if (abs(value) < 0.5f)
		value = 0.f;
	return value;
}

static Uint8 *audio_pos; 
static Uint32 audio_len; 
static Uint32 wav_length;
static Uint8 *wav_buffer;
static SDL_AudioSpec wav_spec; 

void engPlaySound(char* path)
{
	if (SDL_LoadWAV(path, &wav_spec, &wav_buffer, &wav_length) == NULL) 
	{
		engDrawStringFormat(10, 10, "Could not load sound \n%s", SDL_GetError());
	}
	wav_spec.callback = engAudioCallback;
	wav_spec.userdata = NULL;

	audio_pos = wav_buffer; 
	audio_len = wav_length; 

	if (SDL_OpenAudio(&wav_spec, NULL) < 0)
	{
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
	}

	SDL_PauseAudio(0);
}

void engAudioCallback(void *userdata, Uint8 *stream, int len) {

	if (audio_len == 0)
	{
		SDL_PauseAudio(1);
		return;
	}
	
	len = (len > audio_len ? audio_len : len);
	SDL_memcpy (stream, audio_pos, len); 

	audio_pos += len;
	audio_len -= len;	
}