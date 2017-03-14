#ifndef RCPPRN_INTERFACE_C_H
#define RCPPRN_INTERFACE_C_H

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


DTOSHARED_EXPORT int DTOSHARED_CCA AddText(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddPicture(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddPictureFromMemory(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddBarCode(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddCut(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddBeep(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddDrawer(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddFeed(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA AddSlip(void *ptr);

DTOSHARED_EXPORT int DTOSHARED_CCA get_CharSet(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_CharSet(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CodePage(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_CodePage(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FileName(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FileName(void *ptr, const wchar_t *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Caption(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA put_Caption(void *ptr, const wchar_t *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontIndex(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontIndex(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_LineSpacing(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_LineSpacing(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontBold(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontBold(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontItalic(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontItalic(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontDblHeight(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontDblHeight(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontDblWidth(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontDblWidth(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontUnderLine(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontUnderLine(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontOverLine(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontOverLine(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FontNegative(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FontNegative(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Alignment(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_Alignment(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_TextNewLine(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_TextNewLine(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_TextUpSideDown(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_TextUpSideDown(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CharRotation(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_CharRotation(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_ZeroSlashed(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_ZeroSlashed(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Rotation(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_Rotation(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Color(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_Color(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_TextWrap(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_TextWrap(void *ptr, int value);

DTOSHARED_EXPORT int DTOSHARED_CCA get_BarcodeText(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeText(void *ptr, const wchar_t *bfr);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BarcodeHeight(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeHeight(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BarcodeType(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeType(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BarcodePrintText(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodePrintText(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BarcodeControlCode(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeControlCode(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CutValue(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_CutValue(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_FeedValue(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_FeedValue(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_DrawerValue(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_DrawerValue(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_SlipValue(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_SlipValue(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_BeepValue(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_BeepValue(void *ptr, int value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_AutoSize(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_AutoSize(void *ptr, int value);

DTOSHARED_EXPORT int DTOSHARED_CCA get_PictureNumber(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_PictureNumber(void *ptr, int value);

DTOSHARED_EXPORT int DTOSHARED_CCA put_AutoPrint(void *ptr, int value);

DTOSHARED_EXPORT int DTOSHARED_CCA ResetPrintSettings(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA PrintAllTasks(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA ClearAllTasks(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA get_Progress(void *ptr, int *ready, int *all);

DTOSHARED_EXPORT int DTOSHARED_CCA GetStatus(void *ptr);
DTOSHARED_EXPORT int DTOSHARED_CCA get_StatusErrorCount(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_StatusErrorValue(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_StatusErrorDescription(void *ptr, wchar_t *bfr, int bfrSize);
DTOSHARED_EXPORT int DTOSHARED_CCA get_StatusErrorIndex(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA put_StatusErrorIndex(void *ptr, int value);

DTOSHARED_EXPORT int DTOSHARED_CCA get_CapBeep(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapPicture(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapPictureFromMemory(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFeed(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapCutFull(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapCutPart(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapDrawer(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontCount(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapLineSpacing(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontBold(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontItalic(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontDblHeight(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontDblWidth(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontUnderLine(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontOverLine(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapFontNegative(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapTextUpSideDown(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapZeroSlashed(void *ptr, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapColorCount(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapCharCount(void *ptr, int *value);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapCharRotation(void *ptr, int value, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapRotation(void *ptr, int value, int *isCap);
DTOSHARED_EXPORT int DTOSHARED_CCA get_CapPictureWidth(void *ptr, int *value);

}


#endif // RCPPRNINTERFACEC_H
