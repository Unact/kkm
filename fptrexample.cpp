#include <ctime>
#include <exception>
#include <QLibrary>
#include <QDebug>
#include <QDir>

// Заголовочные файлы ДТО9
#include "ifptr.h"
#include "dto_errors.h"
#include "dto_const.h"

#include "fptrexample.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#  include <QTextCodec>

static class TrInit
{
public:
    TrInit()
    {
        QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    }
} g_TrInit;
#endif

class Exception : public std::exception {
public:
    Exception(const QString &msg)
        : m_message(msg)
    {
    }

    virtual ~Exception() throw()
    {
    }

    virtual const char* what() const throw()
    {
        static QByteArray rawMessage = m_message.toUtf8();
        return rawMessage.data();
    }

private:
    QString m_message;
};

static wchar_t* charToWChar(const char* text)
{
    size_t size = strlen(text) + 1;
    wchar_t* wa = new wchar_t[size];
    mbstowcs(wa,text,size);
    return wa;
}

void checkError(TED::IDTOBase *iface)
{
    if(!iface)
        throw Exception("Invalid interface");

    int rc = EC_OK;
    iface->get_ResultCode(&rc);
    if(rc < 0)
    {
        QString resultDescription, badParamDescription;
        QVector<wchar_t> v(256);
        int size = iface->get_ResultDescription(&v[0], v.size());
        if (size <= 0)
            throw Exception("get_ResultDescription error");
        if (size > v.size())
        {
            v.clear();
            v.resize(size + 1);
            iface->get_ResultDescription(&v[0], v.size());
        }
        resultDescription = QString::fromWCharArray(&v[0]);
        if(rc == EC_INVALID_PARAM) {
            QVector<wchar_t> v(256);
            int size = iface->get_BadParamDescription(&v[0], v.size());
            if (size <= 0)
                throw Exception("get_BadParamDescription error");
            if (size > v.size())
            {
                v.clear();
                v.resize(size + 1);
                iface->get_ResultDescription(&v[0], v.size());
            }
            badParamDescription = QString::fromWCharArray(&v[0]);
        }

        if(badParamDescription.isEmpty())
            throw Exception(QObject::trUtf8("[%1] %2")
                            .arg(rc)
                            .arg(resultDescription));
        else
            throw Exception(QObject::trUtf8("[%1] %2 (%3)")
                            .arg(rc)
                            .arg(resultDescription)
                            .arg(badParamDescription));
    }
}

void printText(TED::Fptr::IFptr *ifptr, const char *text, TED::Fptr::Alignment alignment = TED::Fptr::AlignmentLeft,
               TED::Fptr::TextWrap wrap = TED::Fptr::TextWrapLine)
{
    if(!ifptr)
        throw Exception("Invalid interface");
      
    wchar_t *buff;
    buff = charToWChar(text);
    
    if(ifptr->put_Caption(buff) < 0)
        checkError(ifptr);
    if(ifptr->put_TextWrap(wrap) < 0)
        checkError(ifptr);
    if(ifptr->put_Alignment(alignment) < 0)
        checkError(ifptr);
    if(ifptr->PrintString() < 0)
        checkError(ifptr);
}

void openCheck(TED::Fptr::IFptr *ifptr, TED::Fptr::ChequeType type)
{
    if(ifptr->put_Mode(TED::Fptr::ModeRegistration) < 0)
        checkError(ifptr);
    if(ifptr->SetMode() < 0)
        checkError(ifptr);
    if(ifptr->put_CheckType(type) < 0)
        checkError(ifptr);
    if(ifptr->OpenCheck() < 0)
        checkError(ifptr);
}

void closeCheck(TED::Fptr::IFptr *ifptr, int typeClose)
{
    if(ifptr->put_TypeClose(typeClose) < 0)
        checkError(ifptr);
    if(ifptr->CloseCheck() < 0)
        checkError(ifptr);
}

void registration(TED::Fptr::IFptr *ifptr, const char *name, double price, double quantity)
{
    wchar_t *buff;
    buff = charToWChar(name);
    
    if(ifptr->put_Quantity(quantity) < 0)
        checkError(ifptr);
    if(ifptr->put_Price(price) < 0)
        checkError(ifptr);
    if(ifptr->put_TextWrap(TED::Fptr::TextWrapWord) < 0)
        checkError(ifptr);
    if(ifptr->put_Name(buff) < 0)
        checkError(ifptr);
    if(ifptr->Registration() < 0)
        checkError(ifptr);
}

void registrationFZ54(TED::Fptr::IFptr *ifptr, const char *name, double price,
                      double quantity, 
                      int taxNumber)
{
    wchar_t *buff;
    buff = charToWChar(name);
    
    if(ifptr->put_TaxNumber(taxNumber) < 0)
        checkError(ifptr);
    if(ifptr->put_Quantity(quantity) < 0)
        checkError(ifptr);
    if(ifptr->put_Price(price) < 0)
        checkError(ifptr);
    if(ifptr->put_TextWrap(TED::Fptr::TextWrapWord) < 0)
        checkError(ifptr);
    if(ifptr->put_Name(buff) < 0)
        checkError(ifptr);
    if(ifptr->Registration() < 0)
        checkError(ifptr);
}

void payment(TED::Fptr::IFptr *ifptr, double sum, int type)
{
    if(ifptr->put_Summ(sum) < 0)
        checkError(ifptr);
    if(ifptr->put_TypeClose(type) < 0)
        checkError(ifptr);
    if(ifptr->Payment() < 0)
        checkError(ifptr);
    double remainder, change;
    ifptr->get_Remainder(&remainder);
    ifptr->get_Change(&change);
    qDebug() << qPrintable(QObject::trUtf8("Remainder: %1, Change: %2")
                               .arg(remainder)
                               .arg(change));
}

void reportZ(TED::Fptr::IFptr *ifptr)
{
    if(ifptr->put_Mode(TED::Fptr::ModeReportClear) < 0)
        checkError(ifptr);
    if(ifptr->SetMode() < 0)
        checkError(ifptr);
    if(ifptr->put_ReportType(TED::Fptr::ReportZ))
        checkError(ifptr);
    if(ifptr->Report() < 0)
        checkError(ifptr);
}

void printFooter(TED::Fptr::IFptr *ifptr)
{
    if(ifptr->put_Mode(TED::Fptr::ModeReportNoClear) < 0)
        checkError(ifptr);
    if(ifptr->SetMode() < 0)
        checkError(ifptr);
    if(ifptr->PrintFooter() < 0)
        checkError(ifptr);
}

void printBarcode(TED::Fptr::IFptr *ifptr, TED::Fptr::BarcodeType type,
                  const char *barcode, double scale = 100)
{
    wchar_t *buff;
    buff = charToWChar(barcode);
    
    if(ifptr->put_Alignment(TED::Fptr::AlignmentCenter) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeType(type) < 0)
        checkError(ifptr);
    if(ifptr->put_Barcode(buff) < 0)
        checkError(ifptr);
    if(ifptr->put_Height(0))
        checkError(ifptr);
    if(ifptr->put_BarcodeVersion(0))
        checkError(ifptr);
    if(ifptr->put_BarcodePrintType(TED::Fptr::BarcodeAuto) < 0)
        checkError(ifptr);
    if(ifptr->put_PrintBarcodeText(0) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeControlCode(1) < 0)
        checkError(ifptr);
    if(ifptr->put_Scale(scale) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeCorrection(0) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeColumns(3) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeRows(1) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeProportions(50) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeUseProportions(1) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodePackingMode(TED::Fptr::BarcodePDF417PackingModeDefault) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodePixelProportions(300) < 0)
        checkError(ifptr);
    if(ifptr->PrintBarcode() < 0)
        checkError(ifptr);
}

void discount(TED::Fptr::IFptr *ifptr, double sum, TED::Fptr::DiscountType type,
              TED::Fptr::DestinationType destination)
{
    if(ifptr->put_Summ(sum) < 0)
        checkError(ifptr);
    if(ifptr->put_DiscountType(type) < 0)
        checkError(ifptr);
    if(ifptr->put_Destination(destination) < 0)
        checkError(ifptr);
    if(ifptr->Discount() < 0)
        checkError(ifptr);
}

void charge(TED::Fptr::IFptr *ifptr, double sum, TED::Fptr::DiscountType type,
              TED::Fptr::DestinationType destination)
{
    if(ifptr->put_Summ(sum) < 0)
        checkError(ifptr);
    if(ifptr->put_DiscountType(type) < 0)
        checkError(ifptr);
    if(ifptr->put_Destination(destination) < 0)
        checkError(ifptr);
    if(ifptr->Charge() < 0)
        checkError(ifptr);
}

int put_DeviceSingleSetting(TED::Fptr::IFptr *ifptr, const char *name, const char *value){
    wchar_t *buff;
    buff = charToWChar(name);
    QString settings = QString::fromWCharArray(buff);
    wchar_t *bvalue;
    bvalue = charToWChar(value);
    QString vsettings = QString::fromWCharArray(bvalue);
    qDebug() << "put_DeviceSingleSetting: " << qPrintable(settings) << " " << qPrintable(vsettings);
    return ifptr->put_DeviceSingleSetting(buff,bvalue);
}

int put_DeviceSettings(TED::Fptr::IFptr *ifptr, const char *deviceSettings)
{
  wchar_t *buff;
  buff = charToWChar(deviceSettings);
  return ifptr->put_DeviceSettings(buff);
}

int get_DeviceSettings(TED::Fptr::IFptr *ifptr, char* buf, int buf_size)
{
    QVector<wchar_t> v(buf_size);
    int size = ifptr->get_DeviceSettings(&v[0], v.size());
    QString settings = QString::fromWCharArray(&v[0], v.size());
    QByteArray ba = settings.toLocal8Bit();
    const char *c_str2 = ba.data();
    strcpy(buf, c_str2);
    return size;
}

int ApplySingleSettings(TED::Fptr::IFptr *ifptr){
  return ifptr->ApplySingleSettings();
}

int put_DeviceEnabled(TED::Fptr::IFptr *ifptr, int deviceEnabled)
{
  return ifptr->put_DeviceEnabled(deviceEnabled);
}

int GetStatus(TED::Fptr::IFptr *ifptr)
{
  return ifptr->GetStatus();
}

int CancelCheck(TED::Fptr::IFptr *ifptr)
{
  return ifptr->CancelCheck();
}
