/** @file */

#ifndef IDISPLAY_H
#define IDISPLAY_H

#include "idtobase.h"

namespace TED
{

//! Драйвер дисплеев покупателя (дДП)
namespace Display
{

//! Версия интерфейса драйвера дисплеев покупателя
#define DTO_IDISPLAY_VER1       3

class IDisplay;

//! Модель
enum Model
{
    ModelNull = -1,
    ModelEpson = 0,         /*!< Epson протокол                               */
    ModelEpsonUS = 1,       /*!< Epson протокол USA                           */
    ModelPD309 = 2,         /*!< Posiflex PD-201/PD-309/PD-320                */
    ModelEutron = 11,       /*!< Eutron PD-202S                               */
    ModelIPOS = 12,         /*!< IPOS VFD220                                  */
    ModelZonerich = 13,     /*!< Zonerich ZQ-VFD2300                          */
    ModelWincorNixdorf = 14,/*!< Wincor Nixdorf                               */
};

//! Тип прокрутки текста
enum ScrollType
{
    Off = -1,   /*!< Для внутреннего использования                            */
    None = 0,   /*!< Прокрутка отключена (статичный текст)                    */
    Auto,       /*!< Прокрутка текста влево до границы экрана                 */
    Left        /*!< Прокрутка текста влево и за границу экрана               */
};

//! Кодовая страница
enum CodePage
{
    CP437 = 437,    /*!< CP437                                                */
    CP866 = 866,    /*!< CP866                                                */
    CP1251 = 1251   /*!< CP1251                                               */
};

}
}

//! Создает экземпляр драйвера дисплеев покупателя.
/*!
   При несовпадении версии ver и актуальной будет возвращаться 0, а в лог записываться соответствующее сообщение.
   \param ver Версия драйвера (DTO_IDISPLAY_VER1)
   \return Указатель на экземпляр драйвера дисплеев покупателя
   \retval 0 Ошибка
 */
extern "C" DTOSHARED_EXPORT TED::Display::IDisplay * DTOSHARED_CCA CreateDisplayInterface(int ver);

//! Разрушает экземпляр драйвера дисплеев покупателя.
/*!
   \param iface Экземпляр драйвера дисплеев покупателя
 */
extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA ReleaseDisplayInterface(TED::Display::IDisplay **iface);

//! Тип функции CreateDisplayInterface().
typedef TED::Display::IDisplay*(*CreateDisplayInterfacePtr)(int ver);
//! Тип функции ReleaseDisplayInterface().
typedef void (*ReleaseDisplayInterfacePtr)(TED::Display::IDisplay **iface);

namespace TED
{

namespace Display
{

//! Интерфейс драйвера дисплеев покупателя (дДП)
class IDisplay : public IDTOBase
{
public:
    friend void::ReleaseDisplayInterface(IDisplay * *);
    typedef ReleaseDisplayInterfacePtr ReleaseFunction;

    //! Возвращает указатель на функцию удаления интерфейса.
    /*!
       \return Указатель на функцию удаления интерфейса
     */
    virtual ReleaseDisplayInterfacePtr DTOSHARED_CCA get_ReleaseFunction() = 0;

    //! Выводит на дисплей текст
    /*!
       Запускает процедуру вывода текст \a text с прокруткой \a scrollType и временными параметрами \a repeatTimeout и \a unitTimeout на дисплей.
       Метод неблокирующий.
       \warning При пустой строке выводит тестовый текст
       \param text Текст для отображения
       \param scrollType Тип прокрутки
       \param repeatTimeout Ожидание повтора строки
       \param unitTimeout Ожидание символа
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA DisplayText(const wchar_t *text, int scrollType, int repeatTimeout, int unitTimeout) = 0;

    //! Возвращает идентификатор экземпляра драйвера.
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

    //! Возвращает количество строк экрана дисплея.
    /*!
       \param value Количество строк
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_DeviceRows(int *value) = 0;

    //! Возвращает количество столбцов дисплея.
    /*!
       \param value Количество столбцов
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_DeviceColumns(int *value) = 0;

    //! Возвращает кодировку дисплея.
    /*!
       \param value Кодировка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_CharacterSet(int *value) = 0;

    //! Устанавливает дескриптор устройства вывода (для подключения через ККТ).
    /*!
       Используется для настройки дисплея, подключенного через порт ККТ.
       В данный метод нужно передать указатель, полученный с помощью TED::Fptr::IFptr::get_ClsPtr().
       \warning Передача неверного указателя может привести к краху приложения.
       \param value Устройство вывода
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA put_ClsPtr(void *value) = 0;

protected:
    IDisplay()
    {
    }
    ~IDisplay()
    {
    }

private:
    IDisplay(const IDisplay&);
    IDisplay& operator=(const IDisplay&);
};

} // namespace Display
} // namespace TED

#endif // IDISPLAY_H
