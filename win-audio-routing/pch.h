// Purpose: Precompiled Header File.
// Creator: Zixand
#ifndef PCH_H
#define PCH_H

#pragma once
//STL
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <mutex>
#include <guiddef.h>
#include <mfapi.h>
#include <shlobj.h>
//SDL3
#include "SDL3/SDL.h"
#include <SDL3/SDL_opengl.h>
//IMGUI
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
//GL
#include <GL/gl.h>
#include <GL/glu.h>
//WASAPI
#include "mmdeviceapi.h"
#include "combaseapi.h"
#include <functiondiscoverykeys.h>
#include <strsafe.h>
#include "audiopolicy.h"
#include "audioclient.h"
#include "Audiosessiontypes.h"
#include "endpointvolume.h"
#include <audioclientactivationparams.h>
#include <wrl\implements.h>
//#include <wil\com.h>
//#include <wil\result.h>
//WINDOWS MISC
#include <windows.h>
#include <psapi.h>
#include <initguid.h>

#endif //PCH_H
