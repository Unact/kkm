#include "string"
#include "ruby.h"
#include <ruby/encoding.h>
#include "lib/include/ifptr.h"
#include "lib/include/dto_errors.h"
#include "lib/include/dto_const.h"

// Для корректной компиляции, так как ruby написан в C, а либа в C++
// http://www.angelfire.com/electronic2/issac/rb_cpp_ext_tut.txt
typedef VALUE (ruby_method)(...);

// C(Qnil) == Ruby(nil)
VALUE Kkm = Qnil;
VALUE DeviceDriver = Qnil;

// Для работы с char и wchar_t всегда используется буффер этого размера
int BUFFER_MAX_SIZE = 2000;

// ***********************
// Вспомогательные функции
// ***********************

// Для преобразовывания ruby строк в wchar_t
extern "C" void rb_string_to_wchar(VALUE rb_string, wchar_t* wc_string_to_write){
  mbstowcs(wc_string_to_write, StringValueCStr(rb_string), BUFFER_MAX_SIZE);
}

// Для преобразовывания char строк в wchar_t
extern "C" void wchar_to_char(wchar_t* wc_string, char* c_string_to_write){
  wcstombs(c_string_to_write, wc_string, BUFFER_MAX_SIZE);
}

// Для преобразовывания wchar_t в ruby
extern "C" VALUE wchar_to_rb_string(wchar_t* wc_string){
  char* c_string = new char[BUFFER_MAX_SIZE];
  VALUE rb_string;

  wchar_to_char(wc_string, c_string);
  rb_string = rb_enc_str_new_cstr(c_string, rb_enc_find("UTF-8"));

  delete[] c_string;
  return rb_string;
}

extern "C" TED::Fptr::IFptr* get_ifptr(VALUE self){
  TED::Fptr::IFptr *ifptr;
  Data_Get_Struct(self, TED::Fptr::IFptr, ifptr);

  return ifptr;
}

// Обработчик ошибок из ККМ
extern "C" void check_error(VALUE self) {
  TED::Fptr::IFptr *ifptr = get_ifptr(self);
  VALUE DeviceDriverError = rb_const_get(Kkm, rb_intern("DeviceDriverError"));
  int result_code = EC_OK;
  wchar_t* wc_result_error = new wchar_t[BUFFER_MAX_SIZE];
  wchar_t* wc_bad_param_error = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_result_error_msg;
  int result;

  ifptr->get_ResultCode(&result_code);

  if (result_code < 0) {
    ifptr->get_ResultDescription(wc_result_error, BUFFER_MAX_SIZE);
    rb_result_error_msg = rb_sprintf("%"PRIsVALUE, wchar_to_rb_string(wc_result_error));

    if (result_code == EC_INVALID_PARAM) {
      ifptr->get_BadParamDescription(wc_bad_param_error, BUFFER_MAX_SIZE);
      rb_result_error_msg = rb_sprintf(
        "%"PRIsVALUE", %"PRIsVALUE,
        wchar_to_rb_string(wc_result_error),
        wchar_to_rb_string(wc_bad_param_error)
      );
    }

    rb_funcall(
      rb_intern("Kernel"),
      rb_intern("raise"),
      1,
      rb_funcall(DeviceDriverError, rb_intern("new"), 3, INT2NUM(result), rb_result_error_msg, self)
    );
  }

  delete [] wc_result_error;
  delete [] wc_bad_param_error;
}
// *************************
// НАЧАЛО ДЕКЛАРАЦИИ МЕТОДОВ
// *************************

extern "C" VALUE method_beep(VALUE self){
  if (get_ifptr(self)->Beep() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_sound(VALUE self){
  if (get_ifptr(self)->Sound() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_set_time(VALUE self){
  if (get_ifptr(self)->SetTime() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_set_date(VALUE self){
  if (get_ifptr(self)->SetDate() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_date(VALUE self, VALUE date){
  int year = NUM2INT(rb_funcall(date, rb_intern("year"), 0));
  int month = NUM2INT(rb_funcall(date, rb_intern("month"), 0));
  int day = NUM2INT(rb_funcall(date, rb_intern("day"), 0));

  if (get_ifptr(self)->put_Date(day, month, year) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_date(VALUE self){
  int* year = new int;
  int* month = new int;
  int* day = new int;
  VALUE rb_date;

  if (get_ifptr(self)->get_Date(day, month, year) < 0)
    check_error(self);

  rb_date = rb_funcall(rb_cTime, rb_intern("new"), 3, INT2NUM(*year), INT2NUM(*month), INT2NUM(*day));

  delete year;
  delete month;
  delete day;

  return rb_funcall(rb_date, rb_intern("to_date"), 0);;
}

extern "C" VALUE method_put_time(VALUE self, VALUE rb_time){
  int hours = NUM2INT(rb_funcall(rb_time, rb_intern("hour"), 0));
  int minutes = NUM2INT(rb_funcall(rb_time, rb_intern("min"), 0));
  int seconds = NUM2INT(rb_funcall(rb_time, rb_intern("sec"), 0));

  if (get_ifptr(self)->put_Time(hours, minutes, seconds) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_time(VALUE self){
  int* hours = new int;
  int* minutes = new int;
  int* seconds = new int;
  VALUE rb_date;
  VALUE rb_time;

  if (get_ifptr(self)->get_Time(hours, minutes, seconds) < 0)
    check_error(self);

  rb_date = method_get_date(self);
  rb_time = rb_funcall(
    rb_cTime,
    rb_intern("new"),
    6,
    rb_funcall(rb_date, rb_intern("year"), 0),
    rb_funcall(rb_date, rb_intern("month"), 0),
    rb_funcall(rb_date, rb_intern("day"), 0),
    INT2NUM(*hours),
    INT2NUM(*minutes),
    INT2NUM(*seconds)
  );

  delete hours;
  delete minutes;
  delete seconds;

  return rb_time;
}

extern "C" VALUE method_put_need_result_flag(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_NeedResultFlag(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_need_result_flag(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_NeedResultFlag(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_command_buffer(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_CommandBuffer(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_command_buffer(VALUE self){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_CommandBuffer(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_get_answer_buffer(VALUE self){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_AnswerBuffer(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_run_command(VALUE self){
  if (get_ifptr(self)->RunCommand() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_ofd_error(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_OFDError(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_get_network_error(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_NetworkError(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_get_fn_error(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_FNError(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_get_fn_ffd_version(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_FNFfdVersion(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_get_device_ffd_version(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_DeviceFfdVersion(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_get_ffd_version(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_FfdVersion(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_get_current_status(VALUE self){
  if (get_ifptr(self)->GetCurrentStatus() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_current_mode(VALUE self){
  if (get_ifptr(self)->GetCurrentMode() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_new_document(VALUE self){
  if (get_ifptr(self)->NewDocument() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_fiscalization(VALUE self){
  if (get_ifptr(self)->Fiscalization() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_inn(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_INN(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_inn(VALUE self){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_INN(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_put_mode(VALUE self, VALUE number){
  if (get_ifptr(self)->put_Mode(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_mode(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_Mode(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_set_mode(VALUE self){
  if (get_ifptr(self)->SetMode() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_report_type(VALUE self, VALUE number){
  if (get_ifptr(self)->put_ReportType(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_report_type(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_ReportType(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_report(VALUE self){
  if (get_ifptr(self)->Report() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_value(VALUE self, VALUE dbl){
  if (get_ifptr(self)->put_Value(NUM2DBL(dbl)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_value(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_Value(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_put_check_type(VALUE self, VALUE number){
  if (get_ifptr(self)->put_CheckType(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_check_type(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_CheckType(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_position_type(VALUE self, VALUE number){
  if (get_ifptr(self)->put_PositionType(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_position_type(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_PositionType(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_position_payment_type(VALUE self, VALUE number){
  if (get_ifptr(self)->put_PositionPaymentType(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_position_payment_type(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_PositionPaymentType(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_doc_number(VALUE self, VALUE number){
  if (get_ifptr(self)->put_DocNumber(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_doc_number(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_DocNumber(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_cash_income(VALUE self){
  if (get_ifptr(self)->CashIncome() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_cash_outcome(VALUE self){
  if (get_ifptr(self)->CashOutcome() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_check_number(VALUE self, VALUE number){
  if (get_ifptr(self)->put_CheckNumber(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_check_number(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_CheckNumber(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_open_check(VALUE self){
  if (get_ifptr(self)->OpenCheck() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_cancel_check(VALUE self){
  if (get_ifptr(self)->CancelCheck() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_close_check(VALUE self){
  if (get_ifptr(self)->CloseCheck() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_count(VALUE self, VALUE number){
  if (get_ifptr(self)->put_Count(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_count(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_Count(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_discount_type(VALUE self, VALUE number){
  if (get_ifptr(self)->put_DiscountType(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_discount_type(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_CheckType(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_tax_number(VALUE self, VALUE number){
  if (get_ifptr(self)->put_TaxNumber(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_tax_number(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_TaxNumber(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_text_wrap(VALUE self, VALUE number){
  if (get_ifptr(self)->put_TextWrap(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_text_wrap(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_TextWrap(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_print_purpose(VALUE self, VALUE number){
  if (get_ifptr(self)->put_PrintPurpose(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_print_purpose(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_PrintPurpose(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_receipt_font(VALUE self, VALUE number){
  if (get_ifptr(self)->put_ReceiptFont(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_receipt_font(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_ReceiptFont(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_receipt_font_height(VALUE self, VALUE number){
  if (get_ifptr(self)->put_ReceiptFontHeight(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_receipt_font_height(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_ReceiptFontHeight(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_receipt_linespacing(VALUE self, VALUE number){
  if (get_ifptr(self)->put_ReceiptLinespacing(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_receipt_linespacing(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_ReceiptLinespacing(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_receipt_brightness(VALUE self, VALUE number){
  if (get_ifptr(self)->put_ReceiptBrightness(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_receipt_brightness(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_ReceiptBrightness(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_journal_font(VALUE self, VALUE number){
  if (get_ifptr(self)->put_JournalFont(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_journal_font(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_JournalFont(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_journal_font_height(VALUE self, VALUE number){
  if (get_ifptr(self)->put_JournalFontHeight(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_journal_font_height(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_JournalFontHeight(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_journal_linespacing(VALUE self, VALUE number){
  if (get_ifptr(self)->put_JournalLinespacing(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_journal_linespacing(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_JournalLinespacing(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_journal_brightness(VALUE self, VALUE number){
  if (get_ifptr(self)->put_JournalBrightness(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_journal_brightness(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_JournalBrightness(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_font_negative(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FontNegative(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_font_negative(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FontNegative(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_font_underline(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FontUnderline(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_font_underline(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FontUnderline(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_font_bold(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FontBold(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_font_bold(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FontBold(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_font_dbl_width(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FontDblWidth(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_font_dbl_width(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FontDblWidth(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_font_dbl_height(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FontDblHeight(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_font_dbl_height(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FontDblHeight(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_font_italic(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FontItalic(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_font_italic(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FontItalic(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_alignment(VALUE self, VALUE number){
  if (get_ifptr(self)->put_Alignment(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_alignment(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_Alignment(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_registration(VALUE self){
  if (get_ifptr(self)->Registration() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_return_registration(VALUE self){
  // Кто же додумался метод назвать ключевым словом...
  if (get_ifptr(self)->Return() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_name(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_Name(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_name(VALUE self){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_Name(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_put_price(VALUE self, VALUE dbl){
  if (get_ifptr(self)->put_Price(NUM2DBL(dbl)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_price(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_Price(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_put_quantity(VALUE self, VALUE dbl){
  if (get_ifptr(self)->put_Quantity(NUM2DBL(dbl)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_quantity(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_Quantity(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_put_position_sum(VALUE self, VALUE dbl){
  if (get_ifptr(self)->put_PositionSum(NUM2DBL(dbl)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_position_sum(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_PositionSum(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_put_summ(VALUE self, VALUE dbl){
  if (get_ifptr(self)->put_Summ(NUM2DBL(dbl)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_summ(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_Summ(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_put_type_close(VALUE self, VALUE number){
  if (get_ifptr(self)->put_TypeClose(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_type_close(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_TypeClose(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_test_mode(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_TestMode(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_test_mode(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_TestMode(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_enable_check_summ(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_EnableCheckSumm(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_enable_check_summ(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_EnableCheckSumm(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_get_fiscal(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_Fiscal(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_payment(VALUE self){
  if (get_ifptr(self)->Payment() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_status(VALUE self){
  if (get_ifptr(self)->GetStatus() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_partial_cut(VALUE self){
  if (get_ifptr(self)->PartialCut() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_serial_number(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_SerialNumber(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_serial_number(VALUE self) {
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_SerialNumber(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_put_register_number(VALUE self, VALUE number){
  if (get_ifptr(self)->put_RegisterNumber(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_register_number(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_RegisterNumber(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_get_register(VALUE self){
  if (get_ifptr(self)->GetRegister() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_has_not_sended_docs(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_HasNotSendedDocs(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_full_cut(VALUE self){
  if (get_ifptr(self)->FullCut() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_print_formatted_text(VALUE self){
  if (get_ifptr(self)->PrintFormattedText() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_print_string(VALUE self){
  if (get_ifptr(self)->PrintString() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_print_header(VALUE self){
  if (get_ifptr(self)->PrintHeader() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_print_footer(VALUE self){
  if (get_ifptr(self)->PrintFooter() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_remainder(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_Remainder(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_get_change(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_Change(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_put_caption_purpose(VALUE self, VALUE number){
  if (get_ifptr(self)->put_CaptionPurpose(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_caption_purpose(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_CaptionPurpose(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_set_caption(VALUE self){
  if (get_ifptr(self)->SetCaption() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_current_caption(VALUE self){
  if (get_ifptr(self)->GetCaption() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_begin_form_fiscal_property(VALUE self){
  if (get_ifptr(self)->BeginFormFiscalProperty() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_end_form_fiscal_property(VALUE self){
  if (get_ifptr(self)->EndFormFiscalProperty() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_add_fiscal_property(VALUE self){
  if (get_ifptr(self)->AddFiscalProperty() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_reset_fiscal_properties(VALUE self){
  if (get_ifptr(self)->ResetFiscalProperties() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_write_fiscal_property(VALUE self){
  if (get_ifptr(self)->WriteFiscalProperty() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_read_fiscal_property(VALUE self){
  if (get_ifptr(self)->ReadFiscalProperty() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_fiscal_property_number(VALUE self, VALUE number){
  if (get_ifptr(self)->put_FiscalPropertyNumber(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_fiscal_property_number(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_FiscalPropertyNumber(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_fiscal_property_value(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_FiscalPropertyValue(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_fiscal_property_value(VALUE self) {
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_FiscalPropertyValue(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_put_fiscal_property_type(VALUE self, VALUE number){
  if (get_ifptr(self)->put_FiscalPropertyType(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_fiscal_property_type(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_FiscalPropertyType(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_fiscal_property_print(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FiscalPropertyPrint(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_fiscal_property_print(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FiscalPropertyPrint(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_fiscal_property_user(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_FiscalPropertyUser(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_fiscal_property_user(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_FiscalPropertyUser(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_caption(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_Caption(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_caption(VALUE self) {
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_Caption(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_put_print_check(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_PrintCheck(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_print_check(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_PrintCheck(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_get_session_opened(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_SessionOpened(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_session(VALUE self, VALUE number){
  if (get_ifptr(self)->put_Session(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_session(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_Session(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_file_name(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_FileName(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_file_name(VALUE self) {
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_FileName(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_put_left_margin(VALUE self, VALUE number){
  if (get_ifptr(self)->put_LeftMargin(NUM2INT(number)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_left_margin(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_LeftMargin(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_put_scale(VALUE self, VALUE dbl){
  if (get_ifptr(self)->put_Scale(NUM2DBL(dbl)) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_scale(VALUE self){
  double* dbl = new double;
  VALUE rb_dbl;

  if (get_ifptr(self)->get_Scale(dbl) < 0)
    check_error(self);

  rb_dbl = rb_float_new(*dbl);

  delete dbl;
  return rb_dbl;
}

extern "C" VALUE method_get_pixel_line_length(VALUE self){
  int* number = new int;
  VALUE rb_number;

  if (get_ifptr(self)->get_PixelLineLength(number) < 0)
    check_error(self);

  rb_number = INT2NUM(*number);

  delete number;
  return rb_number;
}

extern "C" VALUE method_print_picture(VALUE self){
  if (get_ifptr(self)->PrintPicture() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_machine_number(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_MachineNumber(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_machine_number(VALUE self) {
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_MachineNumber(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

extern "C" VALUE method_put_device_enabled(VALUE self, VALUE rb_boolean){
  int number;
  if (rb_boolean == Qtrue){
    number = 1;
  } else {
    number = 0;
  }

  if (get_ifptr(self)->put_DeviceEnabled(number) < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_get_device_enabled(VALUE self){
  int* number = new int;
  VALUE rb_boolean;

  if (get_ifptr(self)->get_DeviceEnabled(number) < 0)
    check_error(self);

  rb_boolean = *number == 1 ? Qtrue : Qfalse;

  delete number;
  return rb_boolean;
}

extern "C" VALUE method_put_device_single_setting(VALUE self, VALUE rb_string_name, VALUE rb_string_value){
  wchar_t* wc_string_name = new wchar_t[BUFFER_MAX_SIZE];
  wchar_t* wc_string_value = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string_name, wc_string_name);
  rb_string_to_wchar(rb_string_value, wc_string_value);

  if (get_ifptr(self)->put_DeviceSingleSetting(wc_string_name, wc_string_value) < 0)
    check_error(self);

  delete[] wc_string_name;
  delete[] wc_string_value;
  return Qnil;
}

extern "C" VALUE method_get_device_single_setting(VALUE self, VALUE rb_string_name) {
  wchar_t* wc_string_name = new wchar_t[BUFFER_MAX_SIZE];
  wchar_t* wc_string_value = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string_value;
  rb_string_to_wchar(rb_string_name, wc_string_name);

  if (get_ifptr(self)->get_DeviceSingleSetting(wc_string_name, wc_string_value, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string_value = wchar_to_rb_string(wc_string_value);

  delete[] wc_string_name;
  delete[] wc_string_value;
  return rb_string_value;
}

extern "C" VALUE method_apply_single_settings(VALUE self){
  if (get_ifptr(self)->ApplySingleSettings() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_reset_single_settings(VALUE self){
  if (get_ifptr(self)->ResetSingleSettings() < 0)
    check_error(self);
  return Qnil;
}

extern "C" VALUE method_put_device_settings(VALUE self, VALUE rb_string){
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  rb_string_to_wchar(rb_string, wc_string);

  if (get_ifptr(self)->put_DeviceSettings(wc_string) < 0)
    check_error(self);

  delete[] wc_string;
  return Qnil;
}

extern "C" VALUE method_get_device_settings(VALUE self) {
  wchar_t* wc_string = new wchar_t[BUFFER_MAX_SIZE];
  VALUE rb_string;

  if (get_ifptr(self)->get_DeviceSettings(wc_string, BUFFER_MAX_SIZE) < 0)
    check_error(self);

  rb_string = wchar_to_rb_string(wc_string);

  delete[] wc_string;
  return rb_string;
}

// Для работы с C данными
extern "C" void free_device_driver(TED::Fptr::IFptr *ifptr) {
  ReleaseFptrInterface(&ifptr);
}

// Для работы с C данными
extern "C" VALUE alloc_device_driver(VALUE self) {
  return Data_Wrap_Struct(self, NULL, free_device_driver, CreateFptrInterface(DTO_IFPTR_VER1));
}

extern "C" VALUE method_initialize(VALUE self, VALUE settings) {
  TED::Fptr::IFptr *ifptr = get_ifptr(self);

  method_put_device_settings(self, settings);

  return self;
}

// ************************
// КОНЕЦ ДЕКЛАРАЦИИ МЕТОДОВ
// ************************

// Метод который объявляет весь модуль
extern "C" void Init_kkm() {
  Kkm = rb_define_module("Kkm");
  DeviceDriver = rb_define_class_under(Kkm, "DeviceDriver", rb_cObject);

  rb_define_alloc_func(DeviceDriver, alloc_device_driver);
  rb_define_method(DeviceDriver, "initialize", (ruby_method*) &method_initialize, 1);
  rb_define_method(DeviceDriver, "add_fiscal_property", (ruby_method*) &method_add_fiscal_property, 0);
  rb_define_method(DeviceDriver, "apply_single_settings", (ruby_method*) &method_apply_single_settings, 0);
  rb_define_method(DeviceDriver, "begin_form_fiscal_property", (ruby_method*) &method_begin_form_fiscal_property, 0);
  rb_define_method(DeviceDriver, "beep", (ruby_method*) &method_beep, 0);
  rb_define_method(DeviceDriver, "cancel_check", (ruby_method*) &method_cancel_check, 0);
  rb_define_method(DeviceDriver, "cash_income", (ruby_method*) &method_cash_income, 0);
  rb_define_method(DeviceDriver, "cash_outcome", (ruby_method*) &method_cash_outcome, 0);
  rb_define_method(DeviceDriver, "close_check", (ruby_method*) &method_close_check, 0);
  rb_define_method(DeviceDriver, "end_form_fiscal_property", (ruby_method*) &method_end_form_fiscal_property, 0);
  rb_define_method(DeviceDriver, "fiscalization", (ruby_method*) &method_fiscalization, 0);
  rb_define_method(DeviceDriver, "get_alignment", (ruby_method*) &method_get_alignment, 0);
  rb_define_method(DeviceDriver, "get_answer_buffer", (ruby_method*) &method_get_answer_buffer, 0);
  rb_define_method(DeviceDriver, "get_caption", (ruby_method*) &method_get_caption, 0);
  rb_define_method(DeviceDriver, "get_caption_purpose", (ruby_method*) &method_get_caption_purpose, 0);
  rb_define_method(DeviceDriver, "get_change", (ruby_method*) &method_get_change, 0);
  rb_define_method(DeviceDriver, "get_check_number", (ruby_method*) &method_get_check_number, 0);
  rb_define_method(DeviceDriver, "get_check_type", (ruby_method*) &method_get_check_type, 0);
  rb_define_method(DeviceDriver, "get_command_buffer", (ruby_method*) &method_get_command_buffer, 0);
  rb_define_method(DeviceDriver, "get_count", (ruby_method*) &method_get_count, 0);
  rb_define_method(DeviceDriver, "get_current_mode", (ruby_method*) &method_get_current_mode, 0);
  rb_define_method(DeviceDriver, "get_current_status", (ruby_method*) &method_get_current_status, 0);
  rb_define_method(DeviceDriver, "get_current_caption", (ruby_method*) &method_get_current_caption, 0);
  rb_define_method(DeviceDriver, "get_date", (ruby_method*) &method_get_date, 0);
  rb_define_method(DeviceDriver, "get_device_enabled", (ruby_method*) &method_get_device_enabled, 0);
  rb_define_method(DeviceDriver, "get_device_ffd_version", (ruby_method*) &method_get_device_ffd_version, 0);
  rb_define_method(DeviceDriver, "get_device_settings", (ruby_method*) &method_get_device_settings, 0);
  rb_define_method(DeviceDriver, "get_device_single_setting", (ruby_method*) &method_get_device_single_setting, 1);
  rb_define_method(DeviceDriver, "get_doc_number", (ruby_method*) &method_get_doc_number, 0);
  rb_define_method(DeviceDriver, "get_discount_type", (ruby_method*) &method_get_discount_type, 0);
  rb_define_method(DeviceDriver, "get_enable_check_summ", (ruby_method*) &method_get_enable_check_summ, 0);
  rb_define_method(DeviceDriver, "get_ffd_version", (ruby_method*) &method_get_ffd_version, 0);
  rb_define_method(DeviceDriver, "get_font_negative", (ruby_method*) &method_get_font_negative, 0);
  rb_define_method(DeviceDriver, "get_font_underline", (ruby_method*) &method_get_font_underline, 0);
  rb_define_method(DeviceDriver, "get_font_bold", (ruby_method*) &method_get_font_bold, 0);
  rb_define_method(DeviceDriver, "get_font_dbl_width", (ruby_method*) &method_get_font_dbl_width, 0);
  rb_define_method(DeviceDriver, "get_font_dbl_height", (ruby_method*) &method_get_font_dbl_height, 0);
  rb_define_method(DeviceDriver, "get_font_italic", (ruby_method*) &method_get_font_italic, 0);
  rb_define_method(DeviceDriver, "get_file_name", (ruby_method*) &method_get_file_name, 0);
  rb_define_method(DeviceDriver, "get_fiscal", (ruby_method*) &method_get_fiscal, 0);
  rb_define_method(DeviceDriver, "get_fiscal_property_number", (ruby_method*) &method_get_fiscal_property_number, 0);
  rb_define_method(DeviceDriver, "get_fiscal_property_print", (ruby_method*) &method_get_fiscal_property_print, 0);
  rb_define_method(DeviceDriver, "get_fiscal_property_type", (ruby_method*) &method_get_fiscal_property_type, 0);
  rb_define_method(DeviceDriver, "get_fiscal_property_user", (ruby_method*) &method_get_fiscal_property_user, 0);
  rb_define_method(DeviceDriver, "get_fiscal_property_value", (ruby_method*) &method_get_fiscal_property_value, 0);
  rb_define_method(DeviceDriver, "get_fn_ffd_version", (ruby_method*) &method_get_fn_ffd_version, 0);
  rb_define_method(DeviceDriver, "get_fn_error", (ruby_method*) &method_get_fn_error, 0);
  rb_define_method(DeviceDriver, "get_journal_font", (ruby_method*) &method_get_journal_font, 0);
  rb_define_method(DeviceDriver, "get_journal_font_height", (ruby_method*) &method_get_journal_font_height, 0);
  rb_define_method(DeviceDriver, "get_journal_linespacing", (ruby_method*) &method_get_journal_linespacing, 0);
  rb_define_method(DeviceDriver, "get_journal_brightness", (ruby_method*) &method_get_journal_brightness, 0);
  rb_define_method(DeviceDriver, "get_has_not_sended_docs", (ruby_method*) &method_get_has_not_sended_docs, 0);
  rb_define_method(DeviceDriver, "get_inn", (ruby_method*) &method_get_inn, 0);
  rb_define_method(DeviceDriver, "get_left_margin", (ruby_method*) &method_get_left_margin, 0);
  rb_define_method(DeviceDriver, "get_machine_number", (ruby_method*) &method_get_machine_number, 0);
  rb_define_method(DeviceDriver, "get_mode", (ruby_method*) &method_get_mode, 0);
  rb_define_method(DeviceDriver, "get_name", (ruby_method*) &method_get_name, 0);
  rb_define_method(DeviceDriver, "get_need_result_flag", (ruby_method*) &method_get_need_result_flag, 0);
  rb_define_method(DeviceDriver, "get_network_error", (ruby_method*) &method_get_network_error, 0);
  rb_define_method(DeviceDriver, "get_ofd_error", (ruby_method*) &method_get_ofd_error, 0);
  rb_define_method(DeviceDriver, "get_pixel_line_length", (ruby_method*) &method_get_pixel_line_length, 0);
  rb_define_method(DeviceDriver, "get_position_payment_type", (ruby_method*) &method_get_position_payment_type, 0);
  rb_define_method(DeviceDriver, "get_position_type", (ruby_method*) &method_get_position_type, 0);
  rb_define_method(DeviceDriver, "get_position_sum", (ruby_method*) &method_get_position_sum, 0);
  rb_define_method(DeviceDriver, "get_price", (ruby_method*) &method_get_price, 0);
  rb_define_method(DeviceDriver, "get_print_check", (ruby_method*) &method_get_print_check, 0);
  rb_define_method(DeviceDriver, "get_print_purpose", (ruby_method*) &method_get_print_purpose, 0);
  rb_define_method(DeviceDriver, "get_quantity", (ruby_method*) &method_get_quantity, 0);
  rb_define_method(DeviceDriver, "get_receipt_font", (ruby_method*) &method_get_receipt_font, 0);
  rb_define_method(DeviceDriver, "get_receipt_font_height", (ruby_method*) &method_get_receipt_font_height, 0);
  rb_define_method(DeviceDriver, "get_receipt_linespacing", (ruby_method*) &method_get_receipt_linespacing, 0);
  rb_define_method(DeviceDriver, "get_receipt_brightness", (ruby_method*) &method_get_receipt_brightness, 0);
  rb_define_method(DeviceDriver, "get_register", (ruby_method*) &method_get_register, 0);
  rb_define_method(DeviceDriver, "get_register_number", (ruby_method*) &method_get_register_number, 0);
  rb_define_method(DeviceDriver, "get_remainder", (ruby_method*) &method_get_remainder, 0);
  rb_define_method(DeviceDriver, "get_report_type", (ruby_method*) &method_get_report_type, 0);
  rb_define_method(DeviceDriver, "get_scale", (ruby_method*) &method_get_scale, 0);
  rb_define_method(DeviceDriver, "get_serial_number", (ruby_method*) &method_get_serial_number, 0);
  rb_define_method(DeviceDriver, "get_session", (ruby_method*) &method_get_session, 0);
  rb_define_method(DeviceDriver, "get_session_opened", (ruby_method*) &method_get_session_opened, 0);
  rb_define_method(DeviceDriver, "get_status", (ruby_method*) &method_get_status, 0);
  rb_define_method(DeviceDriver, "get_summ", (ruby_method*) &method_get_summ, 0);
  rb_define_method(DeviceDriver, "get_tax_number", (ruby_method*) &method_get_tax_number, 0);
  rb_define_method(DeviceDriver, "get_test_mode", (ruby_method*) &method_get_test_mode, 0);
  rb_define_method(DeviceDriver, "get_text_wrap", (ruby_method*) &method_get_text_wrap, 0);
  rb_define_method(DeviceDriver, "get_time", (ruby_method*) &method_get_time, 0);
  rb_define_method(DeviceDriver, "get_type_close", (ruby_method*) &method_get_type_close, 0);
  rb_define_method(DeviceDriver, "get_value", (ruby_method*) &method_get_value, 0);
  rb_define_method(DeviceDriver, "open_check", (ruby_method*) &method_open_check, 0);
  rb_define_method(DeviceDriver, "partial_cut", (ruby_method*) &method_partial_cut, 0);
  rb_define_method(DeviceDriver, "full_cut", (ruby_method*) &method_full_cut, 0);
  rb_define_method(DeviceDriver, "new_document", (ruby_method*) &method_new_document, 0);
  rb_define_method(DeviceDriver, "payment", (ruby_method*) &method_payment, 0);
  rb_define_method(DeviceDriver, "print_formatted_text", (ruby_method*) &method_print_formatted_text, 0);
  rb_define_method(DeviceDriver, "print_footer", (ruby_method*) &method_print_footer, 0);
  rb_define_method(DeviceDriver, "print_header", (ruby_method*) &method_print_header, 0);
  rb_define_method(DeviceDriver, "print_picture", (ruby_method*) &method_print_picture, 0);
  rb_define_method(DeviceDriver, "print_string", (ruby_method*) &method_print_string, 0);
  rb_define_method(DeviceDriver, "put_alignment", (ruby_method*) &method_put_alignment, 1);
  rb_define_method(DeviceDriver, "put_caption", (ruby_method*) &method_put_caption, 1);
  rb_define_method(DeviceDriver, "put_caption_purpose", (ruby_method*) &method_put_caption_purpose, 1);
  rb_define_method(DeviceDriver, "put_check_number", (ruby_method*) &method_put_check_number, 1);
  rb_define_method(DeviceDriver, "put_check_type", (ruby_method*) &method_put_check_type, 1);
  rb_define_method(DeviceDriver, "put_command_buffer", (ruby_method*) &method_put_command_buffer, 1);
  rb_define_method(DeviceDriver, "put_count", (ruby_method*) &method_put_count, 1);
  rb_define_method(DeviceDriver, "put_date", (ruby_method*) &method_put_date, 1);
  rb_define_method(DeviceDriver, "put_device_enabled", (ruby_method*) &method_put_device_enabled, 1);
  rb_define_method(DeviceDriver, "put_device_settings", (ruby_method*) &method_put_device_settings, 1);
  rb_define_method(DeviceDriver, "put_device_single_setting", (ruby_method*) &method_put_device_single_setting, 2);
  rb_define_method(DeviceDriver, "put_doc_number", (ruby_method*) &method_put_doc_number, 1);
  rb_define_method(DeviceDriver, "put_discount_type", (ruby_method*) &method_put_discount_type, 1);
  rb_define_method(DeviceDriver, "put_enable_check_summ", (ruby_method*) &method_put_enable_check_summ, 1);
  rb_define_method(DeviceDriver, "put_font_negative", (ruby_method*) &method_put_font_negative, 1);
  rb_define_method(DeviceDriver, "put_font_underline", (ruby_method*) &method_put_font_underline, 1);
  rb_define_method(DeviceDriver, "put_font_bold", (ruby_method*) &method_put_font_bold, 1);
  rb_define_method(DeviceDriver, "put_font_dbl_width", (ruby_method*) &method_put_font_dbl_width, 1);
  rb_define_method(DeviceDriver, "put_font_dbl_height", (ruby_method*) &method_put_font_dbl_height, 1);
  rb_define_method(DeviceDriver, "put_font_italic", (ruby_method*) &method_put_font_italic, 1);
  rb_define_method(DeviceDriver, "put_file_name", (ruby_method*) &method_put_file_name, 1);
  rb_define_method(DeviceDriver, "put_fiscal_property_number", (ruby_method*) &method_put_fiscal_property_number, 1);
  rb_define_method(DeviceDriver, "put_fiscal_property_print", (ruby_method*) &method_put_fiscal_property_print, 1);
  rb_define_method(DeviceDriver, "put_fiscal_property_type", (ruby_method*) &method_put_fiscal_property_type, 1);
  rb_define_method(DeviceDriver, "put_fiscal_property_user", (ruby_method*) &method_put_fiscal_property_user, 1);
  rb_define_method(DeviceDriver, "put_fiscal_property_value", (ruby_method*) &method_put_fiscal_property_value, 1);
  rb_define_method(DeviceDriver, "put_journal_font", (ruby_method*) &method_put_journal_font, 1);
  rb_define_method(DeviceDriver, "put_journal_font_height", (ruby_method*) &method_put_journal_font_height, 1);
  rb_define_method(DeviceDriver, "put_journal_linespacing", (ruby_method*) &method_put_journal_linespacing, 1);
  rb_define_method(DeviceDriver, "put_journal_brightness", (ruby_method*) &method_put_journal_brightness, 1);
  rb_define_method(DeviceDriver, "put_inn", (ruby_method*) &method_put_inn, 1);
  rb_define_method(DeviceDriver, "put_left_margin", (ruby_method*) &method_put_left_margin, 1);
  rb_define_method(DeviceDriver, "put_machine_number", (ruby_method*) &method_put_machine_number, 1);
  rb_define_method(DeviceDriver, "put_mode", (ruby_method*) &method_put_mode, 1);
  rb_define_method(DeviceDriver, "put_name", (ruby_method*) &method_put_name, 1);
  rb_define_method(DeviceDriver, "put_need_result_flag", (ruby_method*) &method_put_need_result_flag, 1);
  rb_define_method(DeviceDriver, "put_position_payment_type", (ruby_method*) &method_put_position_payment_type, 1);
  rb_define_method(DeviceDriver, "put_position_type", (ruby_method*) &method_put_position_type, 1);
  rb_define_method(DeviceDriver, "put_position_sum", (ruby_method*) &method_put_position_sum, 1);
  rb_define_method(DeviceDriver, "put_price", (ruby_method*) &method_put_price, 1);
  rb_define_method(DeviceDriver, "put_print_check", (ruby_method*) &method_put_print_check, 1);
  rb_define_method(DeviceDriver, "put_print_purpose", (ruby_method*) &method_put_print_purpose, 1);
  rb_define_method(DeviceDriver, "put_quantity", (ruby_method*) &method_put_quantity, 1);
  rb_define_method(DeviceDriver, "put_receipt_font", (ruby_method*) &method_put_receipt_font, 1);
  rb_define_method(DeviceDriver, "put_receipt_font_height", (ruby_method*) &method_put_receipt_font_height, 1);
  rb_define_method(DeviceDriver, "put_receipt_linespacing", (ruby_method*) &method_put_receipt_linespacing, 1);
  rb_define_method(DeviceDriver, "put_receipt_brightness", (ruby_method*) &method_put_receipt_brightness, 1);
  rb_define_method(DeviceDriver, "put_register_number", (ruby_method*) &method_put_register_number, 1);
  rb_define_method(DeviceDriver, "put_report_type", (ruby_method*) &method_put_report_type, 1);
  rb_define_method(DeviceDriver, "put_scale", (ruby_method*) &method_put_scale, 1);
  rb_define_method(DeviceDriver, "put_session", (ruby_method*) &method_put_session, 1);
  rb_define_method(DeviceDriver, "put_serial_number", (ruby_method*) &method_put_serial_number, 1);
  rb_define_method(DeviceDriver, "put_summ", (ruby_method*) &method_put_summ, 1);
  rb_define_method(DeviceDriver, "put_tax_number", (ruby_method*) &method_put_tax_number, 1);
  rb_define_method(DeviceDriver, "put_test_mode", (ruby_method*) &method_put_test_mode, 1);
  rb_define_method(DeviceDriver, "put_text_wrap", (ruby_method*) &method_put_text_wrap, 1);
  rb_define_method(DeviceDriver, "put_time", (ruby_method*) &method_put_time, 1);
  rb_define_method(DeviceDriver, "put_type_close", (ruby_method*) &method_put_type_close, 1);
  rb_define_method(DeviceDriver, "put_value", (ruby_method*) &method_put_value, 1);
  rb_define_method(DeviceDriver, "return_registration", (ruby_method*) &method_return_registration, 0);
  rb_define_method(DeviceDriver, "registration", (ruby_method*) &method_registration, 0);
  rb_define_method(DeviceDriver, "read_fiscal_property", (ruby_method*) &method_read_fiscal_property, 0);
  rb_define_method(DeviceDriver, "report", (ruby_method*) &method_report, 0);
  rb_define_method(DeviceDriver, "reset_fiscal_properties", (ruby_method*) &method_reset_fiscal_properties, 0);
  rb_define_method(DeviceDriver, "reset_single_settings", (ruby_method*) &method_reset_single_settings, 0);
  rb_define_method(DeviceDriver, "run_command", (ruby_method*) &method_run_command, 0);
  rb_define_method(DeviceDriver, "sound", (ruby_method*) &method_sound, 0);
  rb_define_method(DeviceDriver, "set_date", (ruby_method*) &method_set_date, 0);
  rb_define_method(DeviceDriver, "set_mode", (ruby_method*) &method_set_mode, 0);
  rb_define_method(DeviceDriver, "set_time", (ruby_method*) &method_set_time, 0);
  rb_define_method(DeviceDriver, "set_caption", (ruby_method*) &method_set_caption, 0);
  rb_define_method(DeviceDriver, "write_fiscal_property", (ruby_method*) &method_write_fiscal_property, 0);
}
