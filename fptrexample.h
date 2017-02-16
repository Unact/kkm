#ifndef KKM_INTERFACE_C_H
#define KKM_INTERFACE_C_H

extern "C" DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceEnabled(TED::Fptr::IFptr *ifptr, int deviceEnabled);
extern "C" DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSingleSetting(TED::Fptr::IFptr *ifptr,
                                                                      const char *name,
                                                                      const char *value);

extern "C" DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSettings(TED::Fptr::IFptr *ifptr, const wchar_t *deviceSettings);
extern "C" DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceSettings(TED::Fptr::IFptr *ifptr, char* buf, int buf_size);

extern "C" DTOSHARED_EXPORT int DTOSHARED_CCA ApplySingleSettings(TED::Fptr::IFptr *ifptr);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA checkError(TED::IDTOBase *iface);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA printText(TED::Fptr::IFptr *ifptr, const wchar_t *text,
               TED::Fptr::Alignment alignment ,
               TED::Fptr::TextWrap wrap);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA openCheck(TED::Fptr::IFptr *ifptr, TED::Fptr::ChequeType type);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA closeCheck(TED::Fptr::IFptr *ifptr, int typeClose);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA registration(TED::Fptr::IFptr *ifptr, const wchar_t *name, double price, double quantity);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA registrationFZ54(TED::Fptr::IFptr *ifptr, const wchar_t *name,
                                                                double price,
                                                                double quantity,
                                                                int taxNumber);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA payment(TED::Fptr::IFptr *ifptr, double sum, int type);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA reportZ(TED::Fptr::IFptr *ifptr);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA printFooter(TED::Fptr::IFptr *ifptr);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA printBarcode(TED::Fptr::IFptr *ifptr,
                                                            TED::Fptr::BarcodeType type,
                                                            const wchar_t *barcode, double scale);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA discount(TED::Fptr::IFptr *ifptr, double sum, TED::Fptr::DiscountType type,
              TED::Fptr::DestinationType destination);

extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA charge(TED::Fptr::IFptr *ifptr, double sum, TED::Fptr::DiscountType type,
              TED::Fptr::DestinationType destination);

#endif /* KKM_INTERFACE_C_H */
