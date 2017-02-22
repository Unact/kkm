/** @file */

#ifndef ISCALE_H
#define ISCALE_H

#include "idtobase.h"

namespace TED
{

//! Драйвер электронных весов (дЭВ)
namespace Scale
{

//! Версия интерфейса драйвера электронных весов
#define DTO_ISCALE_VER1         3

class IScale;

//! Модель
enum Model
{
    ModelShtrihPOS20 = 1,           /*!< ШТРИХ-М: протокол POS 2.0            */
    ModelMassaK2 = 6,               /*!< Масса-К: протокол 2 серий МК         */
    ModelCASCommon = 8              /*!< CAS серий AD, AP, ER, SC             */
};

}
}

//! Создает экземпляр драйвера электронных весов.
/*!
   При несовпадении версии ver и актуальной будет возвращаться 0, а в лог записываться соответствующее сообщение.
   \param ver Версия драйвера (DTO_ISCALE_VER1)
   \return Указатель на экземпляр драйвера электронных весов
   \retval 0 Ошибка
 */
extern "C" DTOSHARED_EXPORT TED::Scale::IScale * DTOSHARED_CCA CreateScaleInterface(int ver);

//! Разрушает экземпляр драйвера электронных весов.
/*!
   \param iface Экземпляр драйвера электронных весов
 */
extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA ReleaseScaleInterface(TED::Scale::IScale **iface);

//! Тип функции CreateScaleInterface().
typedef TED::Scale::IScale*(*CreateScaleInterfacePtr)(int ver);
//! Тип функции ReleaseScaleInterface().
typedef void (*ReleaseScaleInterfacePtr)(TED::Scale::IScale **iface);


namespace TED
{

namespace Scale
{

//! Интерфейс драйвера электронных весов (дЭВ)
class IScale : public IDTOBase
{
public:
    friend DTOSHARED_EXPORT void DTOSHARED_CCA::ReleaseScaleInterface(TED::Scale::IScale**);
    typedef ReleaseScaleInterfacePtr ReleaseFunction;

    //! Возвращает указатель на функцию удаления интерфейса.
    /*!
       \return Указатель на функцию удаления интерфейса
     */
    virtual ReleaseScaleInterfacePtr DTOSHARED_CCA get_ReleaseFunction() = 0;

    //! Читает показания весов.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Выходные свойства
        <tr><td>Weight          <td>Вес                                 <td>get_Weight()
        <tr><td>TareWeight      <td>Вес тары                            <td>get_TareWeight()
        <tr><td>NettoWeight     <td>Флаг нетто                          <td>get_NettoWeight()
        <tr><td>NegWeight       <td>Флаг отрицательного веса            <td>get_NegWeight()
        <tr><td>NonStable       <td>Флаг нестабильного показателя веса  <td>get_NonStable()
        <tr><td>BigWeight       <td>Флаг превышения допустимого веса    <td>get_BigWeight()
        <tr><td>AutoZeroMode    <td>Флаг наличия груза на чаше          <td>get_AutoZeroMode()
        <tr><td>InvalidRange    <td>Флаг неверного диапазона веса       <td>get_InvalidRange()
        <tr><td>OutOfZero       <td>Флаг состояния нуля                 <td>get_OutOfZero()
       </table>

       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ReadWeight() = 0;

    //! Устанавливает «нуль» на весах.
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ZeroScale() = 0;

    //! Устанавливает массу тары на весах.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>TareWeight  <td>Вес         <td>put_TareWeight()
       </table>

       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA SetTareWeight() = 0;

    //! Устанавливает массу тары на весах.
    /*!
       Метод выполняет установку значения «Массы тары» на весах равным грузу установленному сейчас на чаше весов.
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA Tare() = 0;

    //! Возвращает значение веса.
    /*!
       \param value Вес
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_Weight(double *value) = 0;

    //! Возвращает значение веса тары.
    /*!
       \param value Вес тары
       \retval 0 Успех
       \sa put_TareWeight()
     */
    virtual int DTOSHARED_CCA get_TareWeight(double *value) = 0;

    //! Устанавливает значение веса тары.
    /*!
       \param value Вес тары
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_TareWeight()
     */
    virtual int DTOSHARED_CCA put_TareWeight(double value) = 0;

    //! Сбрасывает текущее состояние весов.
    /*!
       Метод выполняет перезапуск весов. При этом весы переходят в состояние аналогичное состоянию после их включения.
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA Reset() = 0;

    //! Возвращает флаг нетто.
    /*!
       \param value Флаг нетто
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_NettoWeight(int *value) = 0;

    //! Возвращает флаг наличия груза на чаше.
    /*!
       \param value Флаг наличия груза на чаше
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_AutoZeroMode(int *value) = 0;

    //! Возвращает флаг состояния нуля.
    /*!
       Флаг = 1 (true), если ноль находится вне допустимого диапазона.
       \param value Флаг состояния нуля
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_OutOfZero(int *value) = 0;

    //! Возвращает флаг отрицательного веса.
    /*!
       \param value Флаг отрицательного веса
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_NegWeight(int *value) = 0;

    //! Возвращает флаг превышения допустимого веса.
    /*!
       \param value Флаг превышения допустимого веса
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_BigWeight(int *value) = 0;

    //! Возвращает флаг нестабильного показателя веса.
    /*!
       \param value Флаг нестабильного показателя веса
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_NonStable(int *value) = 0;

    //! Возвращает флаг неверного диапазона веса.
    /*!
       Отвечает за состояние, когда весы не способны взвесить текущий груз
       \param value Флаг неверного диапазона веса
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_InvalidRange(int *value) = 0;

    //! Запрашивает описание устройства
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Выходные свойства
        <tr><td>DeviceDescription   <td>Описание оборудования   <td>get_DeviceDescription()
        <tr><td>DeviceVersion       <td>Версия оборудования     <td>get_DeviceVersion()
       </table>

       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA GetStatus() = 0;

    //! Возвращает описание оборудования.
    /*!
       \param bfr Буфер для описания
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
     */
    virtual int DTOSHARED_CCA get_DeviceDescription(wchar_t *bfr, int bfrSize) = 0;

    //! Возвращает версию и подверсию оборудования.
    /*!
       \param major Версия
       \param minor Подверсия
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_DeviceVersion(int *major, int *minor) = 0;

    //! Возвращает режим работы оборудования.
    /*!
       \warning Не используется
       \param mode Режим
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_Mode(int *mode) = 0;

protected:
    IScale()
    {
    }

    ~IScale()
    {
    }

private:
    IScale(const IScale&);
    IScale& operator=(const IScale&);
};


} // namespace Scale

} // namespace TED


#endif // ISCALE_H
