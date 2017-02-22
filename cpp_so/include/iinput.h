/** @file */

#ifndef IINPUT_H
#define IINPUT_H

#include "idtobase.h"
#include "abstract__event.h"


namespace TED
{

//! Драйвер устройств ввода (дУВ)
namespace Input
{

//! Версия интерфейса драйвера устройств ввода
#define DTO_IINPUT_VER1         6

//! Идентификаторы типов устройств ввода
/*!
   Константы для put_DeviceSettings() и put_DeviceSingleSettings()
 */
enum Model
{
    //! Сканер ШК
    Scanner = 0,
    //! Ридер магнитных карт
    CardReader,
    //! Механический ключ
    MechanicalKey
};

class IInput;

}
}


//! Создает экземпляр драйвера устройств ввода.
/*!
   При несовпадении версии ver и актуальной будет возвращаться 0, а в лог записываться соответствующее сообщение.
   \param ver Версия драйвера (DTO_IINPUT_VER1)
   \return Указатель на экземпляр драйвера устройств ввода
   \retval 0 Ошибка
 */
extern "C" DTOSHARED_EXPORT TED::Input::IInput * DTOSHARED_CCA CreateInputInterface(int ver);

//! Разрушает экземпляр драйвера устройств ввода.
/*!
   \param iface Экземпляр драйвера устройств ввода
 */
extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA ReleaseInputInterface(TED::Input::IInput **iface);

//! Тип функции CreateInputInterface().
typedef TED::Input::IInput*(*CreateInputInterfacePtr)(int ver);
//! Тип функции ReleaseInputInterface().
typedef void (*ReleaseInputInterfacePtr)(TED::Input::IInput **iface);


namespace TED
{

namespace Input
{

//! Интерфейс драйвера устройств ввода (дУВ)
class IInput : public IDTOBase
{
public:
    friend DTOSHARED_EXPORT void DTOSHARED_CCA::ReleaseInputInterface(IInput**);
    typedef ReleaseInputInterfacePtr ReleaseFunction;

    //! Возвращает указатель на функцию удаления интерфейса.
    /*!
       \return Указатель на функцию удаления интерфейса
     */
    virtual ReleaseInputInterfacePtr DTOSHARED_CCA get_ReleaseFunction() = 0;

    //! Возвращает идентификатор экземпляра драйвера
    /*!
       \param bfr Буфер для идентификатора
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \retval 0 Успех
       \sa put_CurrentDeviceID()
     */
    virtual int DTOSHARED_CCA get_CurrentDeviceID(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает идентификатор экземпляра драйвера.
    /*!
       \param value Идентификатор
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_CurrentDeviceID()
     */
    virtual int DTOSHARED_CCA put_CurrentDeviceID(const wchar_t *value) = 0;

    //! Устанавливает обработчик событий устройства ввода.
    /*!
       В экземпляр обработчика будут приходить события о появлении новых данных от устройства ввода.
       \param value Обработчик событий
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA put_EventHandler(AbstractEventHandler *value) = 0;

    //! Возвращает флаг автоматического отключения обработки событий.
    /*!
       \param value Флаг автоматического отключения обработки событий
       \retval 0 Успех
       \sa put_AutoDisable()
     */
    virtual int DTOSHARED_CCA get_AutoDisable(int *value) = 0;

    //! Устанавливает флаг автоматического отключения обработки событий.
    /*!
       При значении флага = 1 (true) флаг \a DataEventEnabled будет автоматически сбрасываться в 0 (false) после отправки события в обработчик \a EventHandler.
       \param value
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_AutoDisable()
       \sa put_DataEventEnabled()
     */
    virtual int DTOSHARED_CCA put_AutoDisable(int value) = 0;

    //! Возвращает флаг обработки событий.
    /*!
       \param value Флаг обработки событий
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_DataEventEnabled(int *value) = 0;

    //! Устанавливает флаг обработки событий.
    /*!
       Если флаг сброшен, события будут накапливаться внутри драйвера до тех пор, пока флаг не будет выставлен в 1 (true).
       Узнать количество сообщений в буфере драйвера можно с помощью свойства \a DataCount
       \param value Флаг обработки событий
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_DataCount()
     */
    virtual int DTOSHARED_CCA put_DataEventEnabled(int value) = 0;

    //! Возвращает считанные с устройства данные.
    /*!
       Данные считываются из сообщения по номером \a EventNumber.
       \param bfr Буфер для данных
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
     */
    virtual int DTOSHARED_CCA get_ScanData(wchar_t *bfr, int bfrSize) = 0;

    //! Возвращает чувствительность последнего сообщения.
    /*!
       Чувствительность - временной промежуток между двумя соседними клавиатурными сигналами.
       \warning Свойство имеет смысл только для клавиатурных устройств
       \param value Чувствительность
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_ActualSensitivity(int *value) = 0;

    //! Возвращает новый интерфейс устройств ввода.
    /*!
       \warning Для внутреннего использования
       \param value Интерфейс
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA NewInterface(IInput **value) = 0;

    //! Возвращает первую дорожку последнего сообщения.
    /*!
       \warning Свойство имеет смысл только для ридеров магнитных карт
       \param bfr Буфер для дорожки
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
     */
    virtual int DTOSHARED_CCA get_Track1(wchar_t *bfr, int bfrSize) = 0;

    //! Возвращает вторую дорожку последнего сообщения.
    /*!
       \warning Свойство имеет смысл только для ридеров магнитных карт
       \param bfr Буфер для дорожки
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
     */
    virtual int DTOSHARED_CCA get_Track2(wchar_t *bfr, int bfrSize) = 0;

    //! Возвращает третью дорожку последнего сообщения.
    /*!
       \warning Свойство имеет смысл только для ридеров магнитных карт
       \param bfr Буфер для дорожки
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
     */
    virtual int DTOSHARED_CCA get_Track3(wchar_t *bfr, int bfrSize) = 0;

    //! Возвращает количество сообщений в буфере.
    /*!
       \param value Количество сообщений
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_DataCount(int *value) = 0;

    //! Возвращает номер сообщения.
    /*!
       Свойство \a EventNumber заполняется при отправке события в обработчик \a EventHandler, либо соответвующий setter-ом.
       \param value Номер сообщения
       \retval 0 Успех
       \sa get_EventNumber()
     */
    virtual int DTOSHARED_CCA get_EventNumber(int *value) = 0;

    //! Устанавливает номер сообщения.
    /*!
       \param value Номер сообщения
       \retval -1 Ошибка
       \retval 0 Успех
       \sa put_EventNumber()
     */
    virtual int DTOSHARED_CCA put_EventNumber(int value) = 0;

    //! Удаляет сообщение из внутреннего буфера драйвера.
    /*!
       При выполнении метода происходит удаление драйвером из собственного буфера события (и данных, относящихся к этому событию).
       При отсутствии сообщения свойство \a ResultCode будет заполнено соответствующей ошибкой.

       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>EventNumber     <td>Номер сообщения     <td>put_EventNumber()
       </table>

       \retval -1 Ошибка
       \retval 0 Успех
       \sa put_EventNumber()
     */
    virtual int DTOSHARED_CCA DeleteEvent() = 0;

    //! Возвращает дескриптор для связи дУВ и дККТ.
    /*!
       Используется для связи дУВ и дККТ в случае, когда устройство ввода соединено с портом ККТ.
       Полученный с помощью данного метода дескриптор необходимо передать в НАСТРОЕННЫЙ экземпляр дККТ.
       Данный дескриптор будет использоваться для обмена сообщениями, полученными от устройства ввода.
       \param value Дескриптор
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_ScannerEventHandler(void **value) = 0;

    //! Устанавливает дескриптор для управления портом ККТ из дУВ.
    /*!
       Используется для связи дУВ и дККТ в случае, когда устройство ввода соединено с портом ККТ.
       С помощью данного дескриптора дУВ будет активировать дККТ, если он не активирован,
       а также устанавливать свойство \a ScannerMode (TED::Fptr::IFptr::put_ScannerMode()) в TED::Fptr::ScannerModeAsync.
       \param value Дескриптор
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA put_ScannerPortHandler(void *value) = 0;

protected:
    IInput()
    {
    }

    ~IInput()
    {
    }

private:
    IInput(const IInput&);
    IInput& operator=(const IInput&);
};


} // namespace Input

} // namespace TED


#endif /* IINPUT_H */
