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

  DTOSHARED_EXPORT int DTOSHARED_CCA get_ResultCode(TED::Fptr::IFptr *ifptr, int *resultCode);
  DTOSHARED_EXPORT int DTOSHARED_CCA get_ResultDescription(TED::Fptr::IFptr *ifptr, char *bfr, int bfrSize);
  DTOSHARED_EXPORT int DTOSHARED_CCA get_BadParamDescription(TED::Fptr::IFptr *ifptr, char *bfr, int bfrSize);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Caption(TED::Fptr::IFptr *ifptr, const char *caption);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_TextWrap(TED::Fptr::IFptr *ifptr, int textWrap);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Alignment(TED::Fptr::IFptr *ifptr, int alignment);
  DTOSHARED_EXPORT int DTOSHARED_CCA PrintString(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Mode(TED::Fptr::IFptr *ifptr, int mode);
  DTOSHARED_EXPORT int DTOSHARED_CCA SetMode(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_CheckType(TED::Fptr::IFptr *ifptr, int type);
  DTOSHARED_EXPORT int DTOSHARED_CCA OpenCheck(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_TypeClose(TED::Fptr::IFptr *ifptr, int typeClose);
  DTOSHARED_EXPORT int DTOSHARED_CCA CloseCheck(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_TaxNumber(TED::Fptr::IFptr *ifptr, int taxNumber);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Quantity(TED::Fptr::IFptr *ifptr, double quantity);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Price(TED::Fptr::IFptr *ifptr, double price);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Name(TED::Fptr::IFptr *ifptr, const char *name);
  DTOSHARED_EXPORT int DTOSHARED_CCA Registration(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Summ(TED::Fptr::IFptr *ifptr, double sum);
  DTOSHARED_EXPORT int DTOSHARED_CCA Payment(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA get_Remainder(TED::Fptr::IFptr *ifptr, double *remainder);
  DTOSHARED_EXPORT int DTOSHARED_CCA get_Change(TED::Fptr::IFptr *ifptr, double *change);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_ReportType(TED::Fptr::IFptr *ifptr, int reportType);
  DTOSHARED_EXPORT int DTOSHARED_CCA Report(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA PrintFooter(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeType(TED::Fptr::IFptr *ifptr, int type);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Barcode(TED::Fptr::IFptr *ifptr, const char *barcode);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Height(TED::Fptr::IFptr *ifptr, int height);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeVersion(TED::Fptr::IFptr *ifptr, int version);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodePrintType(TED::Fptr::IFptr *ifptr, int type);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_PrintBarcodeText(TED::Fptr::IFptr *ifptr, int printBarcodeText);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeControlCode(TED::Fptr::IFptr *ifptr, int code);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Scale(TED::Fptr::IFptr *ifptr, double scale);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeCorrection(TED::Fptr::IFptr *ifptr, int correction);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeColumns(TED::Fptr::IFptr *ifptr, int columns);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeRows(TED::Fptr::IFptr *ifptr, int rows);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeProportions(TED::Fptr::IFptr *ifptr, int proportions);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodeUseProportions(TED::Fptr::IFptr *ifptr, int useProportions);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodePackingMode(TED::Fptr::IFptr *ifptr, int packingMode);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_BarcodePixelProportions(TED::Fptr::IFptr *ifptr, int pixelProportions);
  DTOSHARED_EXPORT int DTOSHARED_CCA PrintBarcode(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_DiscountType(TED::Fptr::IFptr *ifptr, int type);
  DTOSHARED_EXPORT int DTOSHARED_CCA put_Destination(TED::Fptr::IFptr *ifptr, int destination);
  DTOSHARED_EXPORT int DTOSHARED_CCA Discount(TED::Fptr::IFptr *ifptr);
  DTOSHARED_EXPORT int DTOSHARED_CCA Charge(TED::Fptr::IFptr *ifptr);

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
