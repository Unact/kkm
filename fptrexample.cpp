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

void printText(TED::Fptr::IFptr *ifptr, const wchar_t *text, TED::Fptr::Alignment alignment = TED::Fptr::AlignmentLeft,
               TED::Fptr::TextWrap wrap = TED::Fptr::TextWrapLine)
{
    if(!ifptr)
        throw Exception("Invalid interface");

    if(ifptr->put_Caption(text) < 0)
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

void registration(TED::Fptr::IFptr *ifptr, const wchar_t *name, double price, double quantity)
{
    if(ifptr->put_Quantity(quantity) < 0)
        checkError(ifptr);
    if(ifptr->put_Price(price) < 0)
        checkError(ifptr);
    if(ifptr->put_TextWrap(TED::Fptr::TextWrapWord) < 0)
        checkError(ifptr);
    if(ifptr->put_Name(name) < 0)
        checkError(ifptr);
    if(ifptr->Registration() < 0)
        checkError(ifptr);
}

void registrationFZ54(TED::Fptr::IFptr *ifptr, const wchar_t *name, double price,
                      double quantity, // TED::Fptr::DiscountType discountType,
                      //double discount,
                      int taxNumber)
{
//    if(ifptr->put_DiscountType(discountType) < 0)
//        checkError(ifptr);
//    if(ifptr->put_Summ(discount) < 0)
//        checkError(ifptr);
    if(ifptr->put_TaxNumber(taxNumber) < 0)
        checkError(ifptr);
    if(ifptr->put_Quantity(quantity) < 0)
        checkError(ifptr);
    if(ifptr->put_Price(price) < 0)
        checkError(ifptr);
    if(ifptr->put_TextWrap(TED::Fptr::TextWrapWord) < 0)
        checkError(ifptr);
    if(ifptr->put_Name(name) < 0)
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
                  const wchar_t *barcode, double scale = 100)
{
    if(ifptr->put_Alignment(TED::Fptr::AlignmentCenter) < 0)
        checkError(ifptr);
    if(ifptr->put_BarcodeType(type) < 0)
        checkError(ifptr);
    if(ifptr->put_Barcode(barcode) < 0)
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

int put_DeviceSettings(TED::Fptr::IFptr *ifptr, const wchar_t *deviceSettings)
{
  return ifptr->put_DeviceSettings(deviceSettings);
}

int get_DeviceSettings(TED::Fptr::IFptr *ifptr, char* buf, int buf_size)
{
    QVector<wchar_t> v(256);
    int size = ifptr->get_DeviceSettings(&v[0], v.size());
    if (size <= 0)
        throw Exception("get_DeviceSettings error");
    if (size > v.size())
    {
                v.clear();
                v.resize(size + 1);
                ifptr->get_DeviceSettings(&v[0], v.size());
    }
    QString settings = QString::fromWCharArray(&v[0], v.size());
    // settings - строка с настройками
    qDebug() << qPrintable(settings);
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

int test()
{
    const bool UseShowProperties =false;
    const bool PrintFiscalCheck = true;
    const bool PrintNonfiscalCheck = false;
    const bool UseFZ54 = true;

    srand(time(0));
    qDebug() << qPrintable(QObject::trUtf8("Hello!!!!!!"));
#if defined(Q_OS_LINUX)
    QLibrary lib("./bins/linux-x64/libfptr.so");
    if(!lib.load())
    {
        lib.setFileName("./bins/linux-x86/libfptr.so");
        lib.load();
    }
#elif defined(Q_OS_WIN32)
    QLibrary lib("../bins/nt-x86-mingw/fptr.dll");
    lib.load();
#else
#   error "Unknown OS"
#endif

    if(!lib.isLoaded())
    {
        qDebug() << qPrintable(QObject::trUtf8("Не удалось загрузить библиотеку fptr - [%1]")
                               .arg(lib.errorString()));
        return 1;
    }

    CreateFptrInterfacePtr create = (CreateFptrInterfacePtr) lib.resolve("CreateFptrInterface");
    ReleaseFptrInterfacePtr release = (ReleaseFptrInterfacePtr) lib.resolve("ReleaseFptrInterface");

    TED::Fptr::IFptr *ifptr = create(DTO_IFPTR_VER1);
    if(!ifptr)
    {
        qDebug() << qPrintable("Cannot create Fptr object");
        return 1;
    }

    try
    {
        // Выставляем рабочий каталог. В нем дККМ будет искать требуемые ему библиотеки.
        QFileInfo finfo(lib.fileName());
        if(ifptr->put_DeviceSingleSetting(S_SEARCHDIR, finfo.absolutePath().toStdWString().c_str()) < 0)
            checkError(ifptr);
        if(ifptr->ApplySingleSettings() < 0)
            checkError(ifptr);

        // Для настройки драйвера можно вызвать графическое окно настроек
        if(UseShowProperties)
        {
            if(ifptr->ShowProperties() < 0)
                checkError(ifptr);
        }
        // Или настроить без него
        else
        {
            // COM17
            //if(ifptr->put_DeviceSingleSetting(S_PORT, 17) < 0)
                //checkError(ifptr);
            // USB. Можно указать положение на шине (USB$1-1.3, например), но тогда не нужно указывать Vid и Pid
            // Работа напрямую с USB - Linux Only!
/*
            if(ifptr->put_DeviceSingleSetting(S_PORT, SV_PORT_USB) < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(S_VID, 0x2912) < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(S_PID, 0x0002) < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(S_PROTOCOL, TED::Fptr::ProtocolAtol30) < 0)
                checkError(ifptr);
            if(UseFZ54)
            {
                if(ifptr->put_DeviceSingleSetting(S_MODEL, TED::Fptr::ModelATOL55F) < 0)
                    checkError(ifptr);
            }
            else
            {
                if(ifptr->put_DeviceSingleSetting(S_MODEL, TED::Fptr::ModelFPrint55K) < 0)
                    checkError(ifptr);
            }
            if(ifptr->put_DeviceSingleSetting(S_BAUDRATE, 115200) < 0)
                checkError(ifptr);
*/

            if(ifptr->put_DeviceSingleSetting(S_OFD_PORT, L"NONE") < 0)
                 checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(S_BITS, SV_BITS_8) < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(S_MODEL, L"63") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(S_IPPORT, L"5555") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"IPAddress", L"192.168.101.64") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"UserPassword", L"30") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"BaudRate", L"9600") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"TTYSuffix", L"ttyACM0") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"ttyACM0", L"63") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"AccessPassword", L"0") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"Parity", L"0") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"Protocol", L"0") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"Port", L"TCPIP") < 0)
                checkError(ifptr);
            if(ifptr->put_DeviceSingleSetting(L"StopBits", L"0") < 0)
                checkError(ifptr);
/*
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() Bits = 8
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() Model = 63
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() IPPort = 5555
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() IPAddress = 192.168.101.64
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() MACAddress =
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() UserPassword = 30
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() BaudRate = 9600
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() TTYSuffix = ttyACM0
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() AccessPassword = 0
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() Parity = 0
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() Protocol = 0
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() Port = TCPIP
            [2017.02.14 14:53:31+524 D T:00000F53] >> put_DeviceSingleSetting() StopBits = 0
            [2017.02.14 14:53:31+524 D T:00000F53] >> ApplySingleSettings()
*/


            if(ifptr->ApplySingleSettings() < 0)
                checkError(ifptr);
        }

        // Процедура сохранения актуальных настроек дККМ. Если пользуетесь ShowProperties(),
        // то ей можно вытащить актуальные настройки, чтобы потом передать из в драйвер через put_DeviceSettings(),
        // а не настраивать заново
        if(true)
        {
            QVector<wchar_t> v(256);
            int size = ifptr->get_DeviceSettings(&v[0], v.size());
            if (size <= 0)
                throw Exception("get_DeviceSettings error");
            if (size > v.size())
            {
                v.clear();
                v.resize(size + 1);
                ifptr->get_DeviceSettings(&v[0], v.size());
            }
            QString settings = QString::fromWCharArray(&v[0], v.size());
            // settings - строка с настройками
            qDebug() << qPrintable(settings);
        }

        // Активируем дККМ
        if (ifptr->put_DeviceEnabled(1) < 0)
            checkError(ifptr);

        // Проверка связи
        if(ifptr->GetStatus() < 0)
            checkError(ifptr);

        // Отменяем чек, если уже открыт. Ошибки "Неверный режим" и "Чек уже закрыт"
        // не являются ошибками, если мы хотим просто отменить чек
        try
        {
            if(ifptr->CancelCheck() < 0)
                checkError(ifptr);
        }
        catch(const Exception&)
        {
            int rc = EC_OK;
            ifptr->get_ResultCode(&rc);
            if(rc != EC_INVALID_MODE && rc != EC_3801)
                throw;
        }

        if(PrintFiscalCheck)
        {
            // Открываем чек продажи, попутно обработав превышение смены
            try
            {
                openCheck(ifptr, TED::Fptr::ChequeSell);
            }
            catch(const Exception&)
            {
                int rc = EC_OK;
                ifptr->get_ResultCode(&rc);
                // Проверка на превышение смены
                if(rc == EC_3822)
                {
                    reportZ(ifptr);
                    openCheck(ifptr, TED::Fptr::ChequeSell);
                }
                else
                {
                    throw;
                }
            }

            // Пробиваем позиции
            double sum = 0;
            /*
            for(int i = 0; i < 3; ++i)
            {
                if(UseFZ54)
                {
                    double price = (double)(rand() % 10000) / 100 + 1, quantity = (double)(rand() % 100) / 10 + 1;
               //     double discountSum = (i % 2) ? 1.0 : -1.0;
               //     TED::Fptr::DiscountType discountType = (i % 2) ? TED::Fptr::DiscountSumm : TED::Fptr::DiscountPercent;
                    registrationFZ54(ifptr, QObject::trUtf8("Позиция %1").arg(i + 1).toStdWString().c_str(),
                                 price, quantity, //discountType, discountSum,
                                     1);
                    sum += price * quantity;
                }
                else
                {
                    double price = (double)(rand() % 10000) / 100 + 1, quantity = (double)(rand() % 100) / 10 + 1;
                    registration(ifptr, QObject::trUtf8("Позиция %1").arg(i + 1).toStdWString().c_str(),
                                 price, quantity);
                    sum += price * quantity;
                    // Скидка на позицию
                    if(i % 2)
                        discount(ifptr, 1, TED::Fptr::DiscountSumm, TED::Fptr::OnPosition);
                    else
                        charge(ifptr, 1, TED::Fptr::DiscountPercent, TED::Fptr::OnPosition);
                }
            }*/
            double price = 1.10, quantity = 2.0;
            registrationFZ54(ifptr, QObject::trUtf8("Позиция Ku 1").toStdWString().c_str(),
                         price, quantity, //discountType, discountSum,
                             3);
            // Скидка на чек
            // discount(ifptr, 1, TED::Fptr::DiscountPercent, TED::Fptr::OnCheck);
            // Оплачиваем
            sum = 2.20;
            payment(ifptr, sum , 0);
            //payment(ifptr, sum / 4, 2);
            //payment(ifptr, sum / 4, 3);
            //payment(ifptr, sum , 0);
            // Закрываем чек
            closeCheck(ifptr, 0);
        }


        // Нефискальный чек
        // Его можно не открывать, а сразу начинать печатать
        if(PrintNonfiscalCheck)
        {
            printText(ifptr,
                      L"Гриша - красавчик;\n"
                      L"Я сразу смазал карту будня, плеснувши краску из стакана;\n"
                      L"Я показал на блюде студня косые скулы океана.\n"
                      L"На чешуе жестяной рыбы прочел я зовы новых губ.",
                      TED::Fptr::AlignmentLeft,
                      TED::Fptr::TextWrapWord);
            printText(ifptr, L"А вы",
                      TED::Fptr::AlignmentLeft,
                      TED::Fptr::TextWrapWord);
            printText(ifptr, L"ноктюрн сыграть",
                      TED::Fptr::AlignmentCenter,
                      TED::Fptr::TextWrapWord);
            printText(ifptr, L"могли бы",
                      TED::Fptr::AlignmentRight,
                      TED::Fptr::TextWrapWord);
            printText(ifptr, L"на флейте водосточных труб?",
                      TED::Fptr::AlignmentLeft,
                      TED::Fptr::TextWrapWord);

            printText(ifptr, L"\nEAN8", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodeEAN8, L"40182735");
            printText(ifptr, L"");
            printText(ifptr, L"\nEAN13", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodeEAN13, L"4007630000116", 200);
            printText(ifptr, L"");
            printText(ifptr, L"\nUPCA", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodeUPCA, L"697929110035");
            printText(ifptr, L"");
            printText(ifptr, L"\nCODE39", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodeCode39, L"ATOL.RU");
            printText(ifptr, L"");
            printText(ifptr, L"\nPDF417", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodePDF417, L"1234567", 100);
            printText(ifptr, L"");
            printText(ifptr, L"\nQR", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodeQR, L"АТОЛ.РУ", 500);
            printText(ifptr, L"");
            printText(ifptr, L"\nITF-14", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodeITF14, L"00012345600012");
            printText(ifptr, L"");
            printText(ifptr, L"\nInterleaved 2 of 5", TED::Fptr::AlignmentCenter);
            printBarcode(ifptr, TED::Fptr::BarcodeInterleaved2of5, L"04062300106659");
            printText(ifptr, L"");
            printFooter(ifptr);
        }
    }
    catch(const std::exception &ex)
    {
        qDebug() << qPrintable(ex.what());
        release(&ifptr);
        return 1;
    }

    release(&ifptr);
    return 0;
}
