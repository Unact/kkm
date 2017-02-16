#ifndef KKM_INTERFACE_C_H
#define KKM_INTERFACE_C_H

extern "C" {
  DTOSHARED_EXPORT int DTOSHARED_CCA GetStatus(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA CancelCheck(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceEnabled(TED::Fptr::IFptr *ifptr, int deviceEnabled);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSingleSetting(TED::Fptr::IFptr *ifptr, const char *name,
                                                                      const char *value);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_DeviceSettings(TED::Fptr::IFptr *ifptr, const char *deviceSettings);
  DTOSHARED_EXPORT int DTOSHARED_CCA get_DeviceSettings(TED::Fptr::IFptr *ifptr, char* buf, int buf_size);

  DTOSHARED_EXPORT int DTOSHARED_CCA ApplySingleSettings(TED::Fptr::IFptr *ifptr);

  DTOSHARED_EXPORT void DTOSHARED_CCA checkError(TED::IDTOBase *iface);

  DTOSHARED_EXPORT void DTOSHARED_CCA printText(TED::Fptr::IFptr *ifptr, const char* text,
               TED::Fptr::Alignment alignment ,
               TED::Fptr::TextWrap wrap);

  DTOSHARED_EXPORT void DTOSHARED_CCA openCheck(TED::Fptr::IFptr *ifptr, TED::Fptr::ChequeType type);

  DTOSHARED_EXPORT void DTOSHARED_CCA closeCheck(TED::Fptr::IFptr *ifptr, int typeClose);

  DTOSHARED_EXPORT void DTOSHARED_CCA registration(TED::Fptr::IFptr *ifptr, const char *name, double price, double quantity);

  DTOSHARED_EXPORT void DTOSHARED_CCA registrationFZ54(TED::Fptr::IFptr *ifptr, const char *name,
                                                                double price,
                                                                double quantity,
                                                                int taxNumber);

  DTOSHARED_EXPORT void DTOSHARED_CCA payment(TED::Fptr::IFptr *ifptr, double sum, int type);

  DTOSHARED_EXPORT void DTOSHARED_CCA reportZ(TED::Fptr::IFptr *ifptr);

  DTOSHARED_EXPORT void DTOSHARED_CCA printFooter(TED::Fptr::IFptr *ifptr);

  DTOSHARED_EXPORT void DTOSHARED_CCA printBarcode(TED::Fptr::IFptr *ifptr,
                                                            TED::Fptr::BarcodeType type,
                                                            const char *barcode, double scale);

  DTOSHARED_EXPORT void DTOSHARED_CCA discount(TED::Fptr::IFptr *ifptr, double sum, TED::Fptr::DiscountType type,
              TED::Fptr::DestinationType destination);

  DTOSHARED_EXPORT void DTOSHARED_CCA charge(TED::Fptr::IFptr *ifptr, double sum, TED::Fptr::DiscountType type,
              TED::Fptr::DestinationType destination);
}
#endif /* KKM_INTERFACE_C_H */
