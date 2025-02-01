#include <napi.h>
#include <obs.h>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace Napi;
static Number GetNAPIVersion(const CallbackInfo& info)
{
    return Number::New(info.Env(), NAPI_VERSION);
}

namespace OBS
{
// Duplicating def_log_handler in base.c to get logs working
static void log_handler(int log_level, const char *format, va_list args, void *param)
{
	char out[8192];
	vsnprintf(out, sizeof(out), format, args);

	switch (log_level) {
	case LOG_DEBUG:
		fprintf(stdout, "debug: %s\n", out);
		fflush(stdout);
		break;

	case LOG_INFO:
		fprintf(stdout, "info: %s\n", out);
		fflush(stdout);
		break;

	case LOG_WARNING:
		fprintf(stdout, "warning: %s\n", out);
		fflush(stdout);
		break;

	case LOG_ERROR:
		fprintf(stderr, "error: %s\n", out);
		fflush(stderr);
	}
}

static wchar_t obsPath[MAX_PATH];
// A way to let obs load stuff it needs without changing the working directory permanently
// Windows only for now
struct TempCWDSwitch
{
    wchar_t restorePath[MAX_PATH];
    TempCWDSwitch(wchar_t* targetPath)
    {
#ifdef _WIN32
        GetCurrentDirectoryW(MAX_PATH, restorePath);
        SetCurrentDirectoryW(targetPath);
#endif
    }

    ~TempCWDSwitch()
    {
#ifdef _WIN32
        SetCurrentDirectoryW(restorePath);
#endif
    }
};

static Boolean Startup(const CallbackInfo& info)
{
#ifdef _WIN32
    // Store path to obs dll for later use
    HMODULE obsModuleHandle;
	if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCWSTR>(obs_startup), &obsModuleHandle))
	{
        DWORD pathLength = GetModuleFileNameW(obsModuleHandle, obsPath, MAX_PATH);
        if (pathLength > 0)
        {
            wchar_t* fileNameStart = wcsrchr(obsPath, L'\\');
            if (fileNameStart)
            {
                fileNameStart[0] = L'\0';
            }
        }
	}
#endif //_WIN32

    base_set_log_handler(&log_handler, nullptr);
    // https://docs.obsproject.com/frontends#initialization-and-shutdown
    bool success = obs_startup("en-US", nullptr, nullptr);
    return Boolean::New(info.Env(), success);
}

static const char* OpenGL_Module = "libobs-opengl";
static const char* D3D11_Module = "libobs-d3d11";
static obs_video_info GetVideoDefaults()
{
    obs_video_info ovi{};
    ovi.graphics_module = OpenGL_Module;
    ovi.fps_num = 1;
    ovi.fps_den = 60;
    ovi.base_width = 1280;
    ovi.base_height = 720;
    ovi.output_width = ovi.base_width;
    ovi.output_height = ovi.base_height;
    ovi.output_format = video_format::VIDEO_FORMAT_NV12;
    return ovi;
}

static Boolean ResetVideo(const CallbackInfo& info)
{
    // TODO: This temporary directory change is not enough to fix all file loading in OBS (graphics thread loads a dll for example)
    // Might just need run obs code in its own process.
    TempCWDSwitch pushd(obsPath);

    obs_video_info ovi = GetVideoDefaults();
    // Ok to pass stack ptr. Gets copied
    int resultCode = obs_reset_video(&ovi);
    if (resultCode == OBS_VIDEO_MODULE_NOT_FOUND)
    {
        ovi.graphics_module = D3D11_Module;
        resultCode = obs_reset_video(&ovi);
    }

    if (resultCode != OBS_VIDEO_SUCCESS)
    {
        printf("obs_reset_video = %d\n", resultCode);
        return Boolean::New(info.Env(), false);
    }

    return Boolean::New(info.Env(), true);
}

static void Shutdown(const CallbackInfo& info)
{
    obs_shutdown();
}

} // end namespace OBS


Object Init(Env env, Object exports)
{
    exports.Set(String::New(env, "getNAPIVersion"), Function::New(env, GetNAPIVersion));
    exports.Set(String::New(env, "startup"), Function::New(env, OBS::Startup));
    exports.Set(String::New(env, "resetVideo"), Function::New(env, OBS::ResetVideo));
    exports.Set(String::New(env, "shutdown"), Function::New(env, OBS::Shutdown));
    return exports;
}

NODE_API_MODULE(mini-obs, Init)
