#ifndef SCALE_INTERFACE_C_H
#define SCALE_INTERFACE_C_H

#if defined(_WIN32) || defined(_WIN32_WCE)
#  if defined(DTO_LIBRARY)
#    define DTOSHARED_EXPORT __declspec(dllexport)
#  else
#    define DTOSHARED_EXPORT __declspec(dllimport)
#  endif
#  define DTOSHARED_CCA __cdecl
#elif defined(__linux__) || defined(ANDROID)
#  define DTOSHARED_EXPORT
#  define DTOSHARED_CCA
#else
#  define DTOSHARED_EXPORT
#  define DTOSHARED_CCA __attribute__ ((cdecl))
#endif

extern "C"
{
DTOSHARED_EXPORT int DTOSHARED_CCA get_LicenseValid(void *ptr, int *state);
DTOSHARED_EXPORT int DTOSHARED_CCA get_LicenseExpiredDate(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Version(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DriverName(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceEnabled(void *ptr, int *deviceEnabled);
DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceEnabled(void *ptr, int deviceEnabled);
DTOSHARED_EXPORT int DTOSHARED_CCA get_ResultCode(void *ptr, int *resultCode);
DTOSHARED_EXPORT int DTOSHARED_CCA get_ResultDescription(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BadParam(void *ptr, int *badParam);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BadParamDescription(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA get_ApplicationHandle(void *ptr, void **appHandle);
DTOSHARED_EXPORT int DTOSHARED_CCA put_ApplicationHandle(void *ptr, void *appHandle);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceSettings(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSettings(void *ptr, const wchar_t *deviceSettings);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceSingleSettingAsBuff(void *ptr, const wchar_t *name, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSingleSettingAsBuff(void *ptr, const wchar_t *name, const wchar_t *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceSingleSettingAsInt(void *ptr, const wchar_t *name, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSingleSettingAsInt(void *ptr, const wchar_t *name, const int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceSingleSettingAsDouble(void *ptr, const wchar_t *name, double *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSingleSettingAsDouble(void *ptr, const wchar_t *name, const double value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceSingleSettingMapping(void *ptr, const wchar_t *name, wchar_t *bfr, int bfrSize);

DTOSHARED_EXPORT int DTOSHARED_CCA ShowProperties(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA ApplySingleSettings(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA ResetSingleSettings(void *ptr);

DTOSHARED_EXPORT int DTOSHARED_CCA GetStatus(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceDescription(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceVersion(void *ptr, int *major, int *minor);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Mode(void *ptr, int *mode);

DTOSHARED_EXPORT int DTOSHARED_CCA ReadWeight(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA ZeroScale(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA SetTareWeight(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA Tare(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Weight(void *ptr, double *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_TareWeight(void *ptr, double *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_TareWeight(void *ptr, double value);
DTOSHARED_EXPORT int DTOSHARED_CCA Reset(void *ptr);

DTOSHARED_EXPORT int DTOSHARED_CCA get_NettoWeight(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_AutoZeroMode(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_OutOfZero(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_NegWeight(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BigWeight(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_NonStable(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_InvalidRange(void *ptr, int *value);
}

#endif // SCALE_INTERFACE_C_H
