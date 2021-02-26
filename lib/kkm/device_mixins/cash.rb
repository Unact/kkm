# frozen_string_literal: true

module KKM
  module DeviceMixins
    # A utility mixin for adding or removing cash from kkm
    module Cash
      def add_cash(sum)
        set_param(LibFptr::LIBFPTR_PARAM_SUM, sum)
        cash_income
      end

      def remove_cash(sum)
        set_param(LibFptr::LIBFPTR_PARAM_SUM, sum)
        cash_outcome
      end
    end
  end
end
