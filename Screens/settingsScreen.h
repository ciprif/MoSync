/*
 * settingsScreen.h
 *
 *  Created on: Jul 17, 2012
 *      Author: Cipri
 */

#ifndef SETTINGSSCREEN_H_
#define SETTINGSSCREEN_H_

#include <NativeUI/Screen.h>
#include <NativeUI/ScreenListener.h>
#include <NativeUI/ButtonListener.h>
#include <NativeUI/CheckBoxListener.h>
#include <NativeUI/NumberPickerListener.h>
#include <NativeUI/DatePickerListener.h>
#include <NativeUI/ListViewListener.h>
#include <MAUtil/String.h>

#include "../Logical/observer.h"
#include "../Model/util.h"

const MAUtil::String COINS[] = { "EUR", "USD", "GBP", "CAD", "AUD", "JPY", "INR", "NZD",
							     "CHF", "ZAR", "DZD", "ARS", "BHD", "BRL", "BGN", "CLP",
							     "CNY", "COP", "CRC", "HRK", "CZK", "DKK", "DOP", "EGP",
							     "EEK", "FJD", "HKD", "HUF", "ISK", "IDR", "ILS", "JMD",
							     "JOD", "KES", "KRW", "KWD", "LBP", "MYR", "MUR", "MXN",
							     "MAD", "NOK", "OMR", "PKR", "PEN", "PHP", "PLN", "QAR",
							     "RON", "RUB", "SAR", "SGD", "LKR", "TWD", "THB", "TTD",
							     "TND", "TRY", "AED", "VEB", "VND", "ZMK" };

#define NUMBER_OF_COINS 62

namespace NativeUI
{
	class Button;
	class CheckBox;
	class VerticalLayout;
	class HorizontalLayout;
	class ListView;
	class ListViewItem;
	class Label;
	class EditBox;
	class DatePicker;
	class NumberPicker;
}

namespace GUI
{
	class SettingsScreen : public NativeUI::Screen, public NativeUI::ScreenListener, public NativeUI::CheckBoxListener,
						   public NativeUI::ButtonListener, public NativeUI::ListViewListener,
						   public NativeUI::DatePickerListener, public NativeUI::NumberPickerListener
	{
	public:
		SettingsScreen();
		~SettingsScreen();

		void setObserver(Logical::Observer*);

		void buttonClicked(NativeUI::Widget*);
		void listViewItemClicked(NativeUI::ListView*, int index);
		void listViewItemClicked(NativeUI::ListView*, NativeUI::ListViewItem*) {}
		void optionsMenuClosed(NativeUI::Screen*) {}
		void optionsMenuItemSelected(NativeUI::Screen*, int index);
		void checkBoxStateChanged(NativeUI::CheckBox*, bool state);
		void numberPickerValueChanged(NativeUI::NumberPicker*, int value);
		void datePickerValueChanged(NativeUI::DatePicker*, const NativeUI::Date& selected);
		void createOptionsMenu();
	private:
		void _createUI();
		void _updateValues();

		NativeUI::VerticalLayout* _createCoinSettingsLayout();
		NativeUI::VerticalLayout* _createTransactionListSettingsLayout();
		NativeUI::VerticalLayout* _createDebtValueSettingsLayout();

		Logical::Observer* _observerReference;

		MAUtil::String _coin;
		double _debtValue;

		NativeUI::Label* _debtValueLabel;
		NativeUI::Label* _coinLabel;
		NativeUI::Button* _coinChangeToggle;

		NativeUI::VerticalLayout* _coinSettingsLayout;
		NativeUI::VerticalLayout* _transactionSettingsLayout;
		NativeUI::VerticalLayout* _mainLayout;

		NativeUI::ListView* _coinsList;

		NativeUI::CheckBox* _allItems;
		NativeUI::CheckBox* _monthly;
		NativeUI::CheckBox* _fromDate;


		NativeUI::EditBox* _newDebtValueEditBox;

		NativeUI::DatePicker* _datePicker;
		NativeUI::NumberPicker* _numberPicker;

		int _itemWidth;

		bool _isAllItems;
		bool _isMontly;
		bool _isFromDate;

		int _dayValue;

		int _saveButtonIndex;
		int _restoreButtonIndex;
	};
}

#endif /* SETTINGSSCREEN_H_ */
