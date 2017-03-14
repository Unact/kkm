/** \file */

#ifndef ISCALE_LP_H
#define ISCALE_LP_H

#include "idtobase.h"

namespace TED
{

//! Драйвер весов с печатью этикеток (дВПЭ)
namespace ScaleLP
{

#define DTO_ISCALE_LP_VER1      2

class IScaleLP;

//! Список моделей
enum Models
{
    CAS_LP_V1_5 = 0,            /*!< CAS LP 1.5                               */
    CAS_LP_V1_6,                /*!< CAS LP 1.6/2.0                           */
    CAS_CL5000,                 /*!< CAS CL5000                               */
    CAS_CL5000J,                /*!< CAS CL5000J                              */
    Massa_K,                    /*!< МАССА-К                                  */
    Massa_K_VPM,                /*!< МАССА-К серии ВПМ                        */
    DIGI_SM,                    /*!< DIGI                                     */
    ACOM_NETS_TCPIP,            /*!< ACOM NETS TCP/IP                         */
    ACOM_NETS_RS232,            /*!< ACOM NETS RS232                          */
    METTLER_TOLEDO_TIGER_VX1,   /*!< METTLER TOLEDO "Tiger" X.01              */
    METTLER_TOLEDO_TIGER_VX2,   /*!< METTLER TOLEDO "Tiger" X.02              */
    Shtrih_Print,               /*!< Штрих-Принт                              */
    ISHIDA_BC4000,              /*!< ISHIDA ВС-4000                           */
    BIZEBRA_BCII,               /*!< BIZERBA BC II                            */
};

//! Типы таблиц
enum TableType
{
    WareTable = 0,          /*!< Таблица товаров                              */
    MessageTable            /*!< Таблица сообщений                            */
};

//! Типы изображения
enum PictureType
{
    PictureNone = 0,        /*!< Без изображения                              */
    PictureRosTest,         /*!< Логотип РОСТЕСТ                              */
    PictureUser             /*!< Пользовательское изображение                 */
};

//! Типы товаров
enum WareType
{
    WeightWare = 0,         /*!< Весовой                                      */
    PieceWare               /*!< Штучный                                      */
};


}

}

//! Создает экземпляр драйвера ВПЭ.
/*!
   При несовпадении версии ver и актуальной будет возвращаться 0, а в лог записываться соответствующее сообщение.
   \param ver Версия драйвера (DTO_ISCALE_LP_VER1)
   \return Указатель на экземпляр драйвера ВПЭ
   \retval 0 Ошибка
 */
extern "C" DTOSHARED_EXPORT TED::ScaleLP::IScaleLP * DTOSHARED_CCA CreateScaleLPInterface(int ver);

//! Разрушает экземпляр драйвера ВПЭ.
/*!
   \param iface Экземпляр драйвера ВПЭ
 */
extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA ReleaseScaleLPInterface(TED::ScaleLP::IScaleLP **iface);

//! Тип функции CreateScaleLPInterface().
typedef TED::ScaleLP::IScaleLP*(*CreateScaleLPInterfacePtr)(int ver);
//! Тип функции ReleaseScaleLPInterface().
typedef void (*ReleaseScaleLPInterfacePtr)(TED::ScaleLP::IScaleLP **iface);


namespace TED
{

namespace ScaleLP
{

//! Интерфейс драйвера весов с печатью этикеток (ВПЭ)
class IScaleLP : public IDTOBase
{
public:
    friend DTOSHARED_EXPORT void DTOSHARED_CCA::ReleaseScaleLPInterface(TED::ScaleLP::IScaleLP**);
    typedef void (*ReleaseFunction)(IScaleLP**);

    //! Возвращает указатель на функцию удаления интерфейса.
    /*!
       \return Указатель на функцию удаления интерфейса
     */
    virtual ReleaseFunction DTOSHARED_CCA get_ReleaseFunction() = 0;

    //! Начинает процедуру чтения данных
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>FirstRecord <td>Номер первой записи                     <td>put_FirstRecord()
        <tr><td>LastRecord  <td>Номер последней записи                  <td>put_LastRecord()
       </table>

       \sa GetRecord()
       \sa EndReport()
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA BeginReport() = 0;

    //! Завершает снятие отчета
    /*!
       \sa BeginReport()
       \sa GetRecord()
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA EndReport() = 0;

    //! Считывает данные о товаре из весов
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>PLUNumber <td>Номер записи                      <td>put_PLUNumber()
        <tr><th colspan=3>Выходные свойства
        <tr><td>WareCode        <td>Код товара                  <td>get_WareCode()
        <tr><td>Price           <td>Цена                        <td>get_Price()
        <tr><td>Tare            <td>Масса тары                  <td>get_Tare()
        <tr><td>Life            <td>Срок годности               <td>get_Life()
        <tr><td>Name            <td>Наименование                <td>get_Name()
        <tr><td>Name2           <td>Наименование 2              <td>get_Name2()
        <tr><td>GroupCode       <td>Номер группы товара         <td>get_GroupCode()
        <tr><td>MessageNumber   <td>Номер сообщения             <td>get_MessageNumber()
        <tr><td>SalesPrice      <td>Суммарная стоимость товара  <td>get_SalesPrice()
        <tr><td>SalesQuantity   <td>Количество продаж           <td>get_SalesQuantity()
        <tr><td>LabelFont       <td>Номер шрифта                <td>get_LabelFont()
        <tr><td>PictureNumber   <td>Тип изображения (TED::ScaleLP::PictureType) <td>get_PictureNumber()
        <tr><td>CertCenter      <td>Код сертифицирующего органа <td>get_CertCenter()
       </table>
       \sa BeginReport()
       \sa EndReport()
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA GetRecord() = 0;

    //! Начинает процесс записи данные в весы
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>TableType   <td>Тип таблицы (TED::ScaleLP::TableType)   <td>put_TableType()
        <tr><td>FirstRecord <td>Номер первой записи                     <td>put_FirstRecord()
        <tr><td>LastRecord  <td>Номер последней записи                  <td>put_LastRecord()
       </table>

       \sa EndAdd()
       \sa SetRecord()
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA BeginAdd() = 0;

    //! Передает все накопленные записи в весы
    /*!
       \sa BeginAdd()
       \sa SetRecord()
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA EndAdd() = 0;

    //! Записывает в весы информацию о товаре
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>PLUNumber       <td>Номер записи                <td>put_PLUNumber()
        <tr><td>Price           <td>Цена                        <td>put_Price()
        <tr><td>WareCode        <td>Код товара                  <td>put_WareCode()
        <tr><td>Name            <td>Наименование                <td>put_Name()
        <tr><td>Name2           <td>Наименование 2              <td>put_Name2()
        <tr><td>WareType        <td>Тип товара (TED::ScaleLP::WareType) <td>put_WareType()
        <tr><td>GroupCode       <td>Код группы товара           <td>put_GroupCode()
        <tr><td>Tare            <td>Вес тары                    <td>put_Tare()
        <tr><td>LabelFont       <td>Номер шрифта                <td>put_LabelFont()
        <tr><td>MessageNumber   <td>Номер сообщения             <td>put_MessageNumber()
        <tr><td>Life            <td>Срок годности               <td>put_Life()
        <tr><td>PictureNumber   <td>Тип изображения             <td>put_PictureNumber()
        <tr><td>CertCenter      <td>Код сертифицирующего органа <td>put_CertCenter()
        <tr><td>DirectMessage   <td>Сообщение                   <td>put_DirectMessage()
       </table>
       \sa BeginAdd()
       \sa EndAdd()
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA SetRecord() = 0;

    //! Очищает буфер записей
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ClearOutput() = 0;

    //! Очищает таблицы весов
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA Clear() = 0;

    //! Читает из весов суммарные итоги по всем товарам
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Выходные свойства
        <tr><td>Weight          <td>Вес             <td>get_Weight()
        <tr><td>SalesPrice      <td>Стоимость       <td>get_SalesPrice()
        <tr><td>SalesQuantity   <td>Количество      <td>get_SalesQuantity()
       </table>

       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ReadGrandTotal() = 0;

    //! Очищает количество продаж по всем PLU
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ClearAllTotal() = 0;

    //! Очищает суммарные итоги
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ClearPLUTotal() = 0;

    //! Считывает данные по товару на весах
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Выходные свойства
        <tr><td>Weight          <td>Показания индикатора "Вес"          <td>get_Weight()
        <tr><td>SalesPrice      <td>Показания индикатора "Стоимость"    <td>get_SalesPrice()
        <tr><td>SalesQuantity   <td>Показания индикатора "Цена"         <td>get_UnitPrice()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ReadWeight() = 0;

    //! Устанавливает дату в весы
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>Date        <td>Дата            <td>put_Date()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA SetDate() = 0;

    //! Устанавливает время в весы
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>Time        <td>Время           <td>put_Time()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA SetTime() = 0;

    //! Проверяет связь с весами
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA GetStatus() = 0;

    //! Возвращает цену за единицу товара.
    /*!
       \param value Цена
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_UnitPrice(double *value) = 0;

    //! Устанавливает номер строки таблицы.
    /*!
       \param value Номер строки таблицы
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_PLUNumber()
     */
    virtual int DTOSHARED_CCA put_PLUNumber(int value) = 0;

    //! Возвращает номер строки таблицы.
    /*!
       \param value Номер строки таблицы
       \retval 0 Успех
       \sa put_PLUNumber()
     */
    virtual int DTOSHARED_CCA get_PLUNumber(int *value) = 0;

    //! Устанавливает номер первой строки таблицы.
    /*!
       \param value Номер первой строки таблицы
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FirstRecord()
     */
    virtual int DTOSHARED_CCA put_FirstRecord(int value) = 0;

    //! Возвращает номер первой строки таблицы.
    /*!
       \param value Номер первой строки таблицы
       \retval 0 Успех
       \sa put_FirstRecord()
     */
    virtual int DTOSHARED_CCA get_FirstRecord(int *value) = 0;

    //! Устанавливает номер последней строки таблицы.
    /*!
       \param value Номер последней строки таблицы
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_LastRecord()
     */
    virtual int DTOSHARED_CCA put_LastRecord(int value) = 0;

    //! Возвращает номер последней строки таблицы.
    /*!
       \param value Номер последней строки таблицы
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_LastRecord(int *value) = 0;

    //! Устанавливает код товара.
    /*!
       \param value Код товара
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_WareCode()
     */
    virtual int DTOSHARED_CCA put_WareCode(int value) = 0;

    //! Возвращает код товара.
    /*!
       \param value Код товара
       \retval 0 Успех
       \sa put_WareCode()
     */
    virtual int DTOSHARED_CCA get_WareCode(int *value) = 0;

    //! Устанавливает цену.
    /*!
       \param value Цена
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Price()
     */
    virtual int DTOSHARED_CCA put_Price(double value) = 0;

    //! Возвращает цену.
    /*!
       \param value Цена
       \retval 0 Успех
       \sa put_Price()
     */
    virtual int DTOSHARED_CCA get_Price(double *value) = 0;

    //! Устанавливает наименование.
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
       \return Код результата
       \sa get_Name()
     */
    virtual int DTOSHARED_CCA put_Name(const wchar_t *value) = 0;

    //! Возвращает наименование
    /*!
       \param bfr Буфер для наименования
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_Name()
     */
    virtual int DTOSHARED_CCA get_Name(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает второе наименование.
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
       \return Код результата
       \sa get_Name2()
     */
    virtual int DTOSHARED_CCA put_Name2(const wchar_t *value) = 0;

    //! Возвращает второе наименование.
    /*!
       \param bfr Буфер для наименования
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_Name2()
     */
    virtual int DTOSHARED_CCA get_Name2(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает код группы товара.
    /*!
       \param value Код группы товара
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_GroupCode()
     */
    virtual int DTOSHARED_CCA put_GroupCode(int value) = 0;

    //! Возвращает код группы товара.
    /*!
       \param value Код группы.
       \retval 0 Успех
       \sa put_GroupCode()
     */
    virtual int DTOSHARED_CCA get_GroupCode(int *value) = 0;

    //! Устанавливает сообщение.
    /*!
       \param value Сообщение
       \retval -1 Ошибка
       \retval 0 Успех
       \return Код результата
       \sa get_DirectMessage()
     */
    virtual int DTOSHARED_CCA put_DirectMessage(const wchar_t *value) = 0;

    //! Возвращает сообщение.
    /*!
       \param bfr Буфер для сообщения
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_DirectMessage()
     */
    virtual int DTOSHARED_CCA get_DirectMessage(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает номер сообщения.
    /*!
       \param value Сообщение
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_MessageNumber()
     */
    virtual int DTOSHARED_CCA put_MessageNumber(int value) = 0;

    //! Возвращает номер сообщения.
    /*!
       \param value Номер сообщения
       \retval 0 Успех
       \sa put_MessageNumber()
     */
    virtual int DTOSHARED_CCA get_MessageNumber(int *value) = 0;

    //! Устанавливает строку сообщения.
    /*!
       \param number Номер строки
       \param value Строка
       \retval -1 Ошибка
       \retval 0 Успех
       \return Код результата
       \sa get_MessageLine()
     */
    virtual int DTOSHARED_CCA put_MessageLine(int number, const wchar_t *value) = 0;

    //! Возвращает строку сообщения
    /*!
       \param number Номер строки
       \param bfr Буфер для строки
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_MessageLine()
     */
    virtual int DTOSHARED_CCA get_MessageLine(int number, wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает вес тары.
    /*!
       \param value Вес тары
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Tare()
     */
    virtual int DTOSHARED_CCA put_Tare(double value) = 0;

    //! Возвращает вес тары.
    /*!
       \param value Вес тары
       \retval 0 Успех
       \sa put_Tare()
     */
    virtual int DTOSHARED_CCA get_Tare(double *value) = 0;

    //! Устанавливает срок годности.
    /*!
       \param value Срок годности
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Life()
     */
    virtual int DTOSHARED_CCA put_Life(int value) = 0;

    //! Возвращает срок годности.
    /*!
       \param value Срок годности
       \retval 0 Успех
       \sa put_Life()
     */
    virtual int DTOSHARED_CCA get_Life(int *value) = 0;

    //! Устанавливает тип товара.
    /*!
       \param value Тип товара
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_WareType()
     */
    virtual int DTOSHARED_CCA put_WareType(int value) = 0;

    //! Возвращает тип товара.
    /*!
       \param value Тип товара
       \retval 0 Успех
       \sa put_WareType()
     */
    virtual int DTOSHARED_CCA get_WareType(int *value) = 0;

    //! Возвращает вес товара.
    /*!
       \param value Вес товара
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_Weight(double *value) = 0;

    //! Возвращает стоимость товара.
    /*!
       \param value Стоимость товара
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_SalesPrice(double *value) = 0;

    //! Возвращает количество товара.
    /*!
       \param value Количество товара
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_SalesQuantity(int *value) = 0;

    //! Устанавливает номер шрифта.
    /*!
       \param value Номер шрифта
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_LabelFont()
     */
    virtual int DTOSHARED_CCA put_LabelFont(int value) = 0;

    //! Возвращает номер шрифта.
    /*!
       \param value Номер шрифта
       \retval 0 Успех
       \sa put_LabelFont()
     */
    virtual int DTOSHARED_CCA get_LabelFont(int *value) = 0;

    //! Устанавливает тип изображения.
    /*!
       \param value Тип изображения
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_PictureNumber()
     */
    virtual int DTOSHARED_CCA put_PictureNumber(int value) = 0;

    //! Возвращает тип изображения.
    /*!
       \param value Тип изображения
       \retval 0 Успех
       \sa put_PictureNumber()
     */
    virtual int DTOSHARED_CCA get_PictureNumber(int *value) = 0;

    //! Устанавливает код сертифицирующего органа РОСТЕСТ.
    /*!
       \param value Код органа
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_CertCenter()
     */
    virtual int DTOSHARED_CCA put_CertCenter(const wchar_t *value) = 0;

    //! Возвращает код сертифицирующего органа РОСТЕСТ.
    /*!
       \param bfr Буфер для кода
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_CertCenter()
     */
    virtual int DTOSHARED_CCA get_CertCenter(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает тип таблицы.
    /*!
       \param value Тип таблицы
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_TableType()
     */
    virtual int DTOSHARED_CCA put_TableType(int value) = 0;

    //! Возвращает тип таблицы.
    /*!
       \param value Тип таблицы
       \retval 0 Успех
       \sa put_TableType()
     */
    virtual int DTOSHARED_CCA get_TableType(int *value) = 0;

    //! Устанаваливает дату.
    /*!
       \param day День
       \param month Месяц
       \param year Год
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Date()
     */
    virtual int DTOSHARED_CCA put_Date(int day, int month, int year) = 0;

    //! Возвращает дату.
    /*!
       \param day День
       \param month Месяц
       \param year Год
       \retval 0 Успех
       \sa put_Date()
     */
    virtual int DTOSHARED_CCA get_Date(int *day, int *month, int *year) = 0;

    //! Устанавливает время.
    /*!
       \param hours Часы
       \param minutes Минуты
       \param seconds Секунды
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Time()
     */
    virtual int DTOSHARED_CCA put_Time(int hours, int minutes, int seconds) = 0;

    //! Возвращает время.
    /*!
       \param hours Часы
       \param minutes Минуты
       \param seconds Секунды
       \retval 0 Успех
       \sa put_Time()
     */
    virtual int DTOSHARED_CCA get_Time(int *hours, int *minutes, int *seconds) = 0;

    //! Возвращает прогресс операциии.
    /*!
       \param ready Готово
       \param all Всего
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_Progress(int *ready, int *all) = 0;

    //! Возвращает минимальное значение PLU.
    /*!
       \param value PLU
       \retval 0 Успех
       \sa get_MaxPLUNumber()
     */
    virtual int DTOSHARED_CCA get_MinPLUNumber(int *value) = 0;

    //! Возвращает максимальное значение PLU.
    /*!
       \param value PLU
       \retval 0 Успех
       \sa get_MinPLUNumber()
     */
    virtual int DTOSHARED_CCA get_MaxPLUNumber(int *value) = 0;

    //! Возвращает минимальное значение номера сообщения.
    /*!
       \param value Номер сообщения
       \retval 0 Успех
       \sa get_MaxMessageNumber()
     */
    virtual int DTOSHARED_CCA get_MinMessageNumber(int *value) = 0;

    //! Возвращает максимальное значение номера сообщения.
    /*!
       \param value Номер сообщения
       \retval 0 Успех
       \sa get_MinMessageNumber()
     */
    virtual int DTOSHARED_CCA get_MaxMessageNumber(int *value) = 0;

    //! Возвращает максимальную ширину строки сообщения.
    /*!
       \param value Ширина строки сообщения
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_MaxMessageLineLength(int *value) = 0;

protected:
    //! Конструктор
    /*!
     */
    IScaleLP()
    {
    }
    //! Деструктор
    /*!
     */
    virtual ~IScaleLP()
    {
    }
private:
    //! Конструктор копирования
    /*!
     */
    IScaleLP(const IScaleLP&);
    //! Оператор присвоения
    /*!
     */
    IScaleLP& operator=(const IScaleLP&);
};

} // namespace ScaleLP

} // namespace TED
#endif // ISCALE_H
