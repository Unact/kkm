/** @file */

#ifndef IRCPPRN_H
#define IRCPPRN_H

#include "idtobase.h"


namespace TED
{

//! Драйвер принтеров чеков (дПЧ)
namespace RcpPrn
{

#define DTO_IRCPPRN_VER1    4

class IRcpPrn;

//! Модель
enum Model
{
    ModelPP7000 = 1,        /*! Posiflex Aura PP7000/PP8000                   */
    ModelPP5200 = 2,        /*! Posiflex Aura PP5200                          */
    ModelEpson = 3,         /*! Epson                                         */
    ModelPP9000 = 4,        /*! Posiflex Aura PP9000                          */
    ModelRP326USE = 5       /*! АТОЛ RP-326USE                                */
};

//! Выравнивание
enum Alignment
{
    AlignmentLeft = 0,      /*!< По левому краю                               */
    AlignmentCenter,        /*!< По центру                                    */
    AlignmentRight          /*!< По правому краю                              */
};

//! Перенос текста
enum TextWrap
{
    TextWrapNone = 0,       /*!< Без переноса                                 */
    TextWrapWord,           /*!< Перенос по словам                            */
    TextWrapLine            /*!< Перенос по строкам                           */
};

//! Тип штрихкода
enum BarcodeType
{
    BarcodeEAN8 = 0,        /*!< EAN8                                         */
    BarcodeEAN13,           /*!< EAN13                                        */
    BarcodeUPCA,            /*!< UPCA                                         */
    BarcodeCode39           /*!< CODE 39                                      */
};

//! Действие с денежным ящиком
enum DrawerAction
{
    DrawerNone = 0,         /*!< Ничего не делать                             */
    DrawerOpen,             /*!< Открыть                                      */
    DrawerClose             /*!< Закрыть                                      */
};

//! Набор символов
enum CharSet
{
    CharSetUSA =  0,        /*!< */
    CharSetFrance,          /*!< */
    CharSetGermany,         /*!< */
    CharSetEngland,         /*!< */
    CharSetDenmark1,        /*!< */
    CharSetSweden,          /*!< */
    CharSetItaly,           /*!< */
    CharSetSpain1,          /*!< */
    CharSetJapan,           /*!< */
    CharSetNorway,          /*!< */
    CharSetDenmark2,        /*!< */
    CharSetSpain2,          /*!< */
    CharSetLatinAmerica,    /*!< */
    CharSetKorea,           /*!< */
    CharSetIreland,         /*!< */
    CharSetLegal            /*!< */
};

//! Поворот текста и изображеня
enum Rotation
{
    Rotation0 = 0,          /*!< Без поворота                                 */
    Rotation90,             /*!< 90 градусов                                  */
    Rotation180,            /*!< 180 градусов                                 */
    Rotation270             /*!< 270 градусов                                 */
};

//! Цвет печати
enum Color
{
    ColorBlack = 0,         /*!< Черно-белый                                  */
    ColorOther              /*!< Другой                                       */
};

//! Кодовая страница
enum CharPage
{
    CharPage437 = 1,        /*!< */
    CharPageKatakana = 2,   /*!< */
    CharPage858 = 4,        /*!< */
    CharPage852 = 5,        /*!< */
    CharPage860 = 6,        /*!< */
    CharPage863 = 8,        /*!< */
    CharPage865 = 9,        /*!< */
    CharPage866 = 10,       /*!< */
    CharPage1252 = 22,      /*!< */
    CharPage850 = 41        /*!< */
};

//! Способ отрезки чека
enum CutType
{
    CutNone = 0,        /*!< Не отрезать                                      */
    CutFull,            /*!< Полная отрезка                                   */
    CutPart,            /*!< Частичная отрезка                                */
    CutFullFeed,        /*!< Полная отрезка с промоткой                       */
    CutPartFeed         /*!< Частичная отрезка с промоткой                    */
};

}
}

//! Создает экземпляр драйвера ПЧ.
/*!
   При несовпадении версии ver и актуальной будет возвращаться 0, а в лог записываться соответствующее сообщение.
   \param ver Версия драйвера (DTO_IRCPPRN_VER1)
   \return Указатель на экземпляр драйвера ПЧ
   \retval 0 Ошибка
 */
extern "C" DTOSHARED_EXPORT TED::RcpPrn::IRcpPrn * DTOSHARED_CCA CreateRcpPrnInterface(int ver);

//! Разрушает экземпляр драйвера ПЧ.
/*!
   \param iface Экземпляр драйвера ПЧ
 */
extern "C" DTOSHARED_EXPORT void DTOSHARED_CCA ReleaseRcpPrnInterface(TED::RcpPrn::IRcpPrn **iface);

//! Тип функции CreateFptrInterface().
typedef TED::RcpPrn::IRcpPrn*(*CreateRcpPrnInterfacePtr)(int ver);
//! Тип функции ReleaseFptrInterface().
typedef void (*ReleaseRcpPrnInterfacePtr)(TED::RcpPrn::IRcpPrn **iface);

namespace TED
{

namespace RcpPrn
{

//! Интерфейс драйвера принтеров чеков (дПЧ)
class IRcpPrn : public IDTOBase
{
public:
    friend void::ReleaseRcpPrnInterface(TED::RcpPrn::IRcpPrn * *);
    typedef ReleaseRcpPrnInterfacePtr ReleaseFunction;

    //! Возвращает указатель на функцию удаления интерфейса.
    /*!
       \return Указатель на функцию удаления интерфейса
     */
    virtual ReleaseFunction DTOSHARED_CCA get_ReleaseFunction() = 0;

    //! Добавляет задачу печати текста в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>Caption         <td>Текст                                   <td>put_Caption()
        <tr><td>Alignment       <td>Выравнивание (TED::RcpPrn::Alignment)   <td>put_Alignment()
        <tr><td>TextWrap        <td>Перенос текста (TED::RcpPrn::TextWrap)  <td>put_TextWrap()
        <tr><td>Color           <td>Идентификатор цвета (TED::RcpPrn::Color)<td>put_Color()
        <tr><td>FontBold        <td>Шрифт двойной толщины                   <td>put_FontBold()
        <tr><td>FontDblHeight   <td>Шрифт двойной высоты                    <td>put_FontDblHeight()
        <tr><td>FontDblWidth    <td>Шрифт двойной ширины                    <td>put_FontDblWidth()
        <tr><td>FontIndex       <td>Номер шрифта                            <td>put_FontIndex()
        <tr><td>FontItalic      <td>Наклонный шрифт                         <td>put_FontItalic()
        <tr><td>FontOverLine    <td>Надчеркнутый шрифт                      <td>put_FontOverLine()
        <tr><td>FontUnderLine   <td>Подчеркнутый шрифт                      <td>put_FontUnderLine()
        <tr><td>FontNegative    <td>Негативный шрифт                        <td>put_FontNegative()
        <tr><td>TextUpSideDown  <td>Перевернутый шрифт                      <td>put_TextUpSideDown()
        <tr><td>LineSpacing     <td>Межстрочный интервал                    <td>put_LineSpacing()
        <tr><td>TextNewLine     <td>Флаг переноса текста                    <td>put_TextNewLine()
        <tr><td>ZeroSlashed     <td>Печатать перечеркнутый нуль             <td>put_ZeroSlashed()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddText() = 0;

    //! Добавляет задачу печати изображения из файла в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>FileName        <td>Название файла                          <td>put_FileName()
        <tr><td>Color           <td>Идентификатор цвета (TED::RcpPrn::Color)<td>put_Color()
        <tr><td>TextWrap        <td>Перенос текста (TED::RcpPrn::TextWrap)  <td>put_TextWrap()
        <tr><td>Alignment       <td>Выравнивание (TED::RcpPrn::Alignment)   <td>put_Alignment()
        <tr><td>Rotation        <td>Поворот (TED::RcpPrn::Rotation)         <td>put_Rotation()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddPicture() = 0;

    //! Добавляет задачу печати изображения из памяти в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>PictureNumber   <td>Номер файла из памяти                   <td>put_PictureNumber()
        <tr><td>Color           <td>Идентификатор цвета (TED::RcpPrn::Color)<td>put_Color()
        <tr><td>TextWrap        <td>Перенос текста (TED::RcpPrn::TextWrap)  <td>put_TextWrap()
        <tr><td>Alignment       <td>Выравнивание (TED::RcpPrn::Alignment)   <td>put_Alignment()
        <tr><td>Rotation        <td>Поворот (TED::RcpPrn::Rotation)         <td>put_Rotation()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddPictureFromMemory() = 0;

    //! Добавляет задачу печати штрихкода в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>Caption             <td>Данные ШК                               <td>put_Caption()
        <tr><td>Color               <td>Идентификатор цвета (TED::RcpPrn::Color)<td>put_Color()
        <tr><td>Alignment           <td>Выравнивание (TED::RcpPrn::Alignment)   <td>put_Alignment()
        <tr><td>Rotation            <td>Поворот (TED::RcpPrn::Rotation)         <td>put_Rotation()
        <tr><td>BarcodeHeight       <td>Высота ШК                               <td>put_BarcodeHeight()
        <tr><td>BarcodePrintText    <td>Флаг печати данных ШК                   <td>put_BarcodePrintText()
        <tr><td>BarcodeControlCode  <td>Наличие контрольных символов            <td>put_BarcodeControlCode()
        <tr><td>BarcodeType         <td>Тип ШК (TED::RcpPrn::BarcodeType)       <td>put_BarcodeType()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddBarCode() = 0;

    //! Добавляет отрезку бумаги в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>CutValue    <td>Способ отрезки (TED::RcpPrn::CutType)   <td>put_CutValue()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddCut() = 0;

    //! Добавляет подачу звукового сигнала в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>BeepValue   <td>Длительность гудка  <td>put_BeepValue()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddBeep() = 0;

    //! Добавляет операцию с денежным ящиком в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>DrawerValue     <td>Тип операции (TED::RcpPrn::DrawerAction)    <td>put_DrawerValue()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddDrawer() = 0;

    //! Добавляет прокрутку бумаги в очередь задач.
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Входные свойства
        <tr><td>FeedValue   <td>Количество строк промотки (TED::RcpPrn::DrawerAction)   <td>put_FeedValue()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddFeed() = 0;

    //! Добавляет операцию подкладной печати в очередь задач.
    /*!
       \warning Не используется в текущей версии.
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA AddSlip() = 0;

    //! Возвращает код набора символов для печати.
    /*!
       \param value Код набора символов
       \retval 0 Успех
       \sa put_CharSet()
     */
    virtual int DTOSHARED_CCA get_CharSet(int *value) = 0;

    //! Устанавливает код набора символов для печати.
    /*!
       \param value Код набора символов
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_CharSet()
     */
    virtual int DTOSHARED_CCA put_CharSet(int value) = 0;

    //! Возвращает кодовую страницу
    /*!
       \param value Кодовая страница
       \retval 0 Успех
       \sa put_CodePage()
     */
    virtual int DTOSHARED_CCA get_CodePage(int *value) = 0;

    //! Устанавливает кодовую страницу
    /*!
       \param value Кодовая страница
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_CodePage()
     */
    virtual int DTOSHARED_CCA put_CodePage(int value) = 0;

    //! Возвращает имя файла
    /*!
       \param bfr Буфер для имени
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_FileName()
     */
    virtual int DTOSHARED_CCA get_FileName(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает имя файла
    /*!
       \param value Имя файла
       \param value Кодовая страница
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FileName()
     */
    virtual int DTOSHARED_CCA put_FileName(const wchar_t *value) = 0;

    //! Возвращает строковое значение
    /*!
       \param bfr Буфер для строки
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_Caption()
     */
    virtual int DTOSHARED_CCA get_Caption(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает строковое значение
    /*!
       \param value Строковое значение
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Caption()
     */
    virtual int DTOSHARED_CCA put_Caption(const wchar_t *value) = 0;

    //! Вовзращает номер шрифта
    /*!
       \param value Номер шрифта
       \retval 0 Успех
       \sa put_FontIndex()
     */
    virtual int DTOSHARED_CCA get_FontIndex(int *value) = 0;

    //! Устанавливает номер шрифта
    /*!
       \param value Номер шрифта
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontIndex()
     */
    virtual int DTOSHARED_CCA put_FontIndex(int value) = 0;

    //! Возвращает межстрочное расстояние
    /*!
       \param value Межстрочное расстояние
       \retval 0 Успех
       \sa put_LineSpacing()
     */
    virtual int DTOSHARED_CCA get_LineSpacing(int *value) = 0;

    //! Устанавливает межстрочное расстояние
    /*!
       \param value Межстрочное расстояние
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_LineSpacing()
     */
    virtual int DTOSHARED_CCA put_LineSpacing(int value) = 0;

    //! Возвращает флаг жирного начертания текста
    /*!
       \param value Флаг жирного начертания
       \retval 0 Успех
       \sa put_FontBold()
     */
    virtual int DTOSHARED_CCA get_FontBold(int *value) = 0;

    //! Устанавливает жирное начертание текста
    /*!
       \param value Флаг жирного начертания
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontBold()
     */
    virtual int DTOSHARED_CCA put_FontBold(int value) = 0;

    //! Возвращает флаг курсивного начертания текста
    /*!
       \param value Флаг курсивного начертания
       \retval 0 Успех
       \sa put_FontItalic()
     */
    virtual int DTOSHARED_CCA get_FontItalic(int *value) = 0;

    //! Устанавливает курсивное начертание текста
    /*!
       \param value Флаг курсивного начертания текста
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontItalic()
     */
    virtual int DTOSHARED_CCA put_FontItalic(int value) = 0;

    //! Возвращает флаг двойной высоты текста
    /*!
       \param value Флаг печати текста двойной высоты
       \retval 0 Успех
       \sa put_FontDblHeight()
     */
    virtual int DTOSHARED_CCA get_FontDblHeight(int *value) = 0;

    //! Устанавливает флаг двойной высоты текста
    /*!
       \param value Флаг печати текста двойной высоты
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontDblHeight()
     */
    virtual int DTOSHARED_CCA put_FontDblHeight(int value) = 0;

    //! Возвращает флаг двойной ширины текста
    /*!
       \param value Флаг печати текста двойной ширины
       \retval 0 Успех
       \sa put_FontDblWidth()
     */
    virtual int DTOSHARED_CCA get_FontDblWidth(int *value) = 0;

    //! Устанавливает флаг двойной ширины текста
    /*!
       \param value Флаг печати текста двойной ширины
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontDblWidth()
     */
    virtual int DTOSHARED_CCA put_FontDblWidth(int value) = 0;

    //! Возвращает флаг подчеркнутого начертания текста
    /*!
       \param value Флаг подчеркнутого начертания текста
       \retval 0 Успех
       \sa put_FontUnderLine()
     */
    virtual int DTOSHARED_CCA get_FontUnderLine(int *value) = 0;

    //! Устанавливает подчеркнутое начертание текста
    /*!
       \param value Флаг подчеркнутого начертания текста
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontUnderLine()
     */
    virtual int DTOSHARED_CCA put_FontUnderLine(int value) = 0;

    //! Возвращает флаг надчеркнутого начертания текста
    /*!
       \param value Флаг надчеркнутого начертания текста
       \retval 0 Успех
       \sa put_FontOverLine()
     */
    virtual int DTOSHARED_CCA get_FontOverLine(int *value) = 0;

    //! Устанавливает надчеркнутого начертание текста
    /*!
       \param value Флаг надчеркнутого начертания текста
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontOverLine()
     */
    virtual int DTOSHARED_CCA put_FontOverLine(int value) = 0;

    //! Возвращает флаг инвертированной печати текста (белым по черному)
    /*!
       \param value Флаг инвертированной печати
       \retval 0 Успех
       \sa put_FontNegative()
     */
    virtual int DTOSHARED_CCA get_FontNegative(int *value) = 0;

    //! Устанавливает инвертированную печать текста (белым по черному)
    /*!
       \param value Флаг инвертированной печати
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FontNegative()
     */
    virtual int DTOSHARED_CCA put_FontNegative(int value) = 0;

    //! Возвращает тип выравнивания печатаемых данных
    /*!
       \param value Тип выравнивания
       \retval 0 Успех
       \sa put_Alignment()
       \sa TED::RcpPrn::Alignment
     */
    virtual int DTOSHARED_CCA get_Alignment(int *value) = 0;

    //! Устанавливает тип печатаемых данных
    /*!
       \param value Тип выравнивания
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Alignment()
       \sa TED::RcpPrn::Alignment
     */
    virtual int DTOSHARED_CCA put_Alignment(int value) = 0;

    //! Возвращает флаг автоматической печать новой строки
    /*!
       \param value Флаг автоматической печати
       \retval 0 Успех
       \sa put_TextNewLine()
     */
    virtual int DTOSHARED_CCA get_TextNewLine(int *value) = 0;

    //! Устанавливает автоматическую печать новой строки
    /*!
       \param value Флаг автоматической печати
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_TextNewLine()
     */
    virtual int DTOSHARED_CCA put_TextNewLine(int value) = 0;

    //! Возвращает флаг печати перевернутого текста
    /*!
       \param value Флаг печати перевернутого текста
       \retval 0 Успех
       \sa put_TextUpSideDown()
     */
    virtual int DTOSHARED_CCA get_TextUpSideDown(int *value) = 0;

    //! Устанавливает флаг печати перевернутого текста
    /*!
       \param value Флаг печати перевернутого текста
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_TextUpSideDown()
     */
    virtual int DTOSHARED_CCA put_TextUpSideDown(int value) = 0;

    //! Возвращает значение поворота символов
    /*!
       \param value Значение поворота символов
       \retval 0 Успех
       \sa put_CharRotation()
     */
    virtual int DTOSHARED_CCA get_CharRotation(int *value) = 0;

    //! Устанавливает поворот символов
    /*!
       \param value Значение поворота символов
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_CharRotation()
     */
    virtual int DTOSHARED_CCA put_CharRotation(int value) = 0;

    //! Возвращает способ печати символа '0'
    /*!
       \param value Способ печати
       \retval 0 Успех
       \sa put_ZeroSlashed()
     */
    virtual int DTOSHARED_CCA get_ZeroSlashed(int *value) = 0;

    //! Устанавливает способ печати символа '0'
    /*!
       \param value Способ печати
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_ZeroSlashed()
     */
    virtual int DTOSHARED_CCA put_ZeroSlashed(int value) = 0;

    //! Возвращает значение поворота элемента задания (ШК или графики)
    /*!
       \param value Значение поворота
       \retval 0 Успех
       \sa put_Rotation()
     */
    virtual int DTOSHARED_CCA get_Rotation(int *value) = 0;

    //! Устанавливает поворот элемента задания (ШК или графики)
    /*!
       \param value Значение поворота
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Rotation()
     */
    virtual int DTOSHARED_CCA put_Rotation(int value) = 0;

    //! Возвращает цвет
    /*!
       \param value Цвет
       \retval 0 Успех
       \sa put_Color()
     */
    virtual int DTOSHARED_CCA get_Color(int *value) = 0;

    //! Устанавливает цвет
    /*!
       \param value Цвет
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_Color()
     */
    virtual int DTOSHARED_CCA put_Color(int value) = 0;

    //! Возвращает флаг переноса текста
    /*!
       \param value Флаг переноса
       \retval 0 Успех
       \sa put_TextWrap()
     */
    virtual int DTOSHARED_CCA get_TextWrap(int *value) = 0;

    //! Устанавливает флаг переноса текста
    /*!
       \param value Флаг переноса
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_TextWrap()
     */
    virtual int DTOSHARED_CCA put_TextWrap(int value) = 0;

    //! Возвращает текст штрихкода
    /*!
       \param bfr Буфер для текста
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_BarcodeText()
     */
    virtual int DTOSHARED_CCA get_BarcodeText(wchar_t *bfr, int bfrSize) = 0;

    //! Устанавливает текст для формирования штрихкода
    /*!
       \param bfr Текст штрихкода
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_BarcodeText()
     */
    virtual int DTOSHARED_CCA put_BarcodeText(const wchar_t *bfr) = 0;

    //! Возвращает высоту ШК
    /*!
       \param value Высота ШК
       \retval 0 Успех
       \sa put_BarcodeHeight()
     */
    virtual int DTOSHARED_CCA get_BarcodeHeight(int *value) = 0;

    //! Устанавливает высоту ШК
    /*!
       \param value Высота ШК
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_BarcodeHeight()
     */
    virtual int DTOSHARED_CCA put_BarcodeHeight(int value) = 0;

    //! Возвращает тип ШК
    /*!
       \param value Тип ШК
       \retval 0 Успех
       \sa put_BarcodeType()
     */
    virtual int DTOSHARED_CCA get_BarcodeType(int *value) = 0;

    //! Устанавливает тип ШК
    /*!
       \param value Тип ШК
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_BarcodeType()
     */
    virtual int DTOSHARED_CCA put_BarcodeType(int value) = 0;

    //! Возвращает флаг печати символьного эквивалента ШК
    /*!
       \param value Флаг эквивалента
       \retval 0 Успех
       \sa put_BarcodePrintText()
     */
    virtual int DTOSHARED_CCA get_BarcodePrintText(int *value) = 0;

    //! Устанавливает флаг печати символьного эквивалента ШК
    /*!
       \param value Флаг эквивалента
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_BarcodePrintText()
     */
    virtual int DTOSHARED_CCA put_BarcodePrintText(int value) = 0;

    //! Возвращает флаг использования контрольного символа ШК
    /*!
       \param value Флаг контрольного символа ШК
       \retval 0 Успех
       \sa put_BarcodeControlCode()
     */
    virtual int DTOSHARED_CCA get_BarcodeControlCode(int *value) = 0;

    //! Определяет, используется ли контрольный символ ШК
    /*!
       \param value Флаг контрольного символа ШК
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_BarcodeControlCode()
     */
    virtual int DTOSHARED_CCA put_BarcodeControlCode(int value) = 0;

    //! Возвращает тип отрезки
    /*!
       \param value Тип отрезки
       \retval 0 Успех
       \sa put_CutValue()
     */
    virtual int DTOSHARED_CCA get_CutValue(int *value) = 0;

    //! Устанавливает тип отрезки
    /*!
       \param value Тип отрезки
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_CutValue()
     */
    virtual int DTOSHARED_CCA put_CutValue(int value) = 0;

    //! Возвращает кол-во проматываемых строк
    /*!
       \param value Кол-во строк
       \retval 0 Успех
       \sa put_FeedValue()
     */
    virtual int DTOSHARED_CCA get_FeedValue(int *value) = 0;

    //! Устанавливает кол-во проматываемых строк
    /*!
       При отрицательном значении осуществляется промотка назад
       \param value Кол-во строк
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_FeedValue()
     */
    virtual int DTOSHARED_CCA put_FeedValue(int value) = 0;

    //! Возвращает код операции с ДЯ
    /*!
       \param value Код операции
       \retval 0 Успех
       \sa put_DrawerValue()
     */
    virtual int DTOSHARED_CCA get_DrawerValue(int *value) = 0;

    //! Устанавливает код операции с ДЯ
    /*!
       \param value Код операции
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_DrawerValue()
     */
    virtual int DTOSHARED_CCA put_DrawerValue(int value) = 0;

    //! Возвращает тип операции с подкладным документом
    /*!
       \param value Код операции
       \warning Не используется в текущей версии.
       \retval 0 Успех
       \sa put_SlipValue()
     */
    virtual int DTOSHARED_CCA get_SlipValue(int *value) = 0;

    //! Устанавливает тип операции с подкладным документом
    /*!
       \param value Код операции
       \warning Не используется в текущей версии.
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_SlipValue()
     */
    virtual int DTOSHARED_CCA put_SlipValue(int value) = 0;

    //! Возвращает кол-во повторов звукового сигнала
    /*!
       \param value Кол-во повторов
       \retval 0 Успех
       \sa put_BeepValue()
     */
    virtual int DTOSHARED_CCA get_BeepValue(int *value) = 0;

    //! Устанавливает кол-во повторов звукового сигнала
    /*!
       \param value Кол-во повторов
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_BeepValue()
     */
    virtual int DTOSHARED_CCA put_BeepValue(int value) = 0;

    //! Возвращает флаг автоматического изменения размера граф. элемента
    /*!
       \param value Флаг изменения размера
       \retval 0 Успех
       \sa put_AutoSize()
     */
    virtual int DTOSHARED_CCA get_AutoSize(int *value) = 0;

    //! Устанавливает флаг автоматического изменения размера граф. элемента
    /*!
       \param value Флаг изменения размера
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_AutoSize()
     */
    virtual int DTOSHARED_CCA put_AutoSize(int value) = 0;

    //! Возвращает номер изображения из памяти ПЧ
    /*!
       \param value Номер изображения
       \retval 0 Успех
       \sa put_PictureNumber()
     */
    virtual int DTOSHARED_CCA get_PictureNumber(int *value) = 0;

    //! Устанавливает номер изображения из памяти ПЧ
    /*!
       \param value Номер изображения
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_PictureNumber()
     */
    virtual int DTOSHARED_CCA put_PictureNumber(int value) = 0;

    //! Устанавливает флаг буферизированной печати
    /*!
       При установке флага в 1 (true) драйвер будет начинать печатать очередь заданий
       при обновлении очереди
       \param value Флаг
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_AutoPrint()
     */
    virtual int DTOSHARED_CCA put_AutoPrint(int value) = 0;

    //! Получает флаг буферизированной печати
    /*!
       \param value Флаг
       \retval 0 Успех
       \sa put_AutoPrint()
     */
    virtual int DTOSHARED_CCA get_AutoPrint(int *value) = 0;

    //! Устанавливает параметры печати по умолчанию
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ResetPrintSettings() = 0;

    //! Запускает печать заданий из очереди
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
       \sa get_TaskID()
     */
    virtual int DTOSHARED_CCA PrintAllTasks() = 0;

    //! Очищает буфер заданий
    /*!
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA ClearAllTasks() = 0;

    //! Возвращает прогресс печати
    /*!
       \param ready Готово заданий
       \param all Всего заданий
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_Progress(int *ready, int *all) = 0;

    //! Проверяет состояние принтера
    /*!
       <table>
        <caption>Свойства</caption>
        <tr><th>Название<th>Описание<th>Доступ
        <tr><th colspan=3>Выходные свойства
        <tr><td>StatusErrorCount        <td>Количество ошибок принтера  <td>get_StatusErrorCount()
        <tr><td>StatusErrorValue        <td>Код ошибки принтера         <td>get_StatusErrorValue()
        <tr><td>StatusErrorDescription  <td>Описание ошибки принтера    <td>get_StatusErrorDescription()
       </table>
       \retval -1 Ошибка
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA GetStatus() = 0;

    //! Возвращает количество ошибок принтера
    /*!
       \param value Количество ошибок
       \retval 0 Успех
     */
    virtual int DTOSHARED_CCA get_StatusErrorCount(int *value) = 0;

    //! Возвращает значение ошибки принтера
    /*!
       \param value Значение ошибки
       \retval 0 Успех
       \sa put_StatusErrorIndex()
     */
    virtual int DTOSHARED_CCA get_StatusErrorValue(int *value) = 0;

    //! Возвращает описание ошибки принтера
    /*!
       Возвращает описание ошибки с номером \a StatusErrorIndex.
       \param bfr Буфер для описания
       \param bfrSize Размер буфера
       \return Требуемый размер буфера
       \sa put_StatusErrorIndex()
     */
    virtual int DTOSHARED_CCA get_StatusErrorDescription(wchar_t *bfr, int bfrSize) = 0;

    //! Возвращает номер ошибки
    /*!
       \param value Номер ошибки
       \return Код результата
       \sa put_StatusErrorIndex()
     */
    virtual int DTOSHARED_CCA get_StatusErrorIndex(int *value) = 0;

    //! Устанавливает номер ошибки
    /*!
       \param value Номер ошибки
       \return Код результата
       \sa get_StatusErrorIndex()
     */
    virtual int DTOSHARED_CCA put_StatusErrorIndex(int value) = 0;

    virtual int DTOSHARED_CCA get_CapBeep(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapPicture(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapPictureFromMemory(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFeed(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapCutFull(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapCutPart(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapDrawer(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontCount(int *value) = 0;
    virtual int DTOSHARED_CCA get_CapLineSpacing(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontBold(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontItalic(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontDblHeight(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontDblWidth(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontUnderLine(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontOverLine(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapFontNegative(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapTextUpSideDown(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapZeroSlashed(int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapColorCount(int *value) = 0;
    virtual int DTOSHARED_CCA get_CapCharCount(int *value) = 0;
    virtual int DTOSHARED_CCA get_CapCharRotation(int value, int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapRotation(int value, int *isCap) = 0;
    virtual int DTOSHARED_CCA get_CapPictureWidth(int *value) = 0;

protected:
    IRcpPrn()
    {
    }
    ~IRcpPrn()
    {
    }

private:
    IRcpPrn(const IRcpPrn&);
    IRcpPrn& operator=(const IRcpPrn&);

};

} // namespace RcpPrn

} // namespace TED


#endif /* IRCPPRN_H */
