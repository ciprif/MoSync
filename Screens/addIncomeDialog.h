/*
 * addIncomeDialog.h
 *
 *  Created on: Jul 3, 2012
 *      Author: Cipri
 */

#ifndef ADDINCOMEDIALOG_H_
#define ADDINCOMEDIALOG_H_

#include <NativeUI/Dialog.h>
#include <NativeUI/CheckBoxListener.h>
#include <NativeUI/ButtonListener.h>
#include <NativeUI/ToggleButtonListener.h>
#include <NativeUI/SliderListener.h>
#include <NativeUI/DatePickerListener.h>
#include <MAUtil/String.h>
#include <MAUtil/Vector.h>
#include "../Model/util.h"

namespace NativeUI
{
	class HorizontalLayout;
	class VerticalLayout;
	class Label;
	class CheckBox;
	class EditBox;
	class Button;
	class Slider;
	class ToggleButton;
	class DatePicker;
	class TimePicker;
}


namespace GUI
{
	class HomeScreen;
	class ListScreen;

	class AddIncomeDialog : public NativeUI::Dialog, public NativeUI::CheckBoxListener,
							public NativeUI::ButtonListener, public NativeUI::ToggleButtonListener,
							public NativeUI::SliderListener, public NativeUI::DatePickerListener
	{
	public:
		AddIncomeDialog();
		AddIncomeDialog(const MAUtil::String& coin);
		~AddIncomeDialog();

		 /**
		 * This method is called if the touch-up event was inside the
		 * bounds of the button.
		 * Platform: iOS, Android, Windows Phone.
		 * @param button The button object that generated the event.
		 */
		void buttonClicked(Widget* button);

		/**
		 * This method is called when the state of the check box was changed
		 * by the user.
		 * @param checkBox The check box object that generated the event.
		 * @param state True if the check box is checked, false otherwise.
		 */
		void checkBoxStateChanged(NativeUI::CheckBox* checkBox, bool state);

		/**
		 * @todo add the toggle buttons for description and image
		 */
		/**
		 * !!!!!!!!!! ownership not passed
		 * @param parent
		 */
		void setHomeScreenRef(HomeScreen* homeScreenRef);
		void setListScreenRef(ListScreen* listScreenRef);

		/**
		 * This method is called when the state of the toggle button was changed
		 * by the user.
		 * @param toggleButton The toggle button object that generated the event.
		 * @param state True if the toggle button is checked, false otherwise.
		 */
		void toggleButtonStateChanged(NativeUI::ToggleButton* toggleButton, bool state);

		void sliderValueChanged(NativeUI::Slider* s, int sliderValue);

		void show();

		void setObserver(Logical::Observer* obs);

		void setCoin(const MAUtil::String& coin);

		void setLaunchedFromHomeScreen(bool value);

		void datePickerValueChanged(NativeUI::DatePicker* datePicker, const NativeUI::Date& selectedDate);
	private:
		void _createUI();

		HomeScreen* _homeScreenRef;
		ListScreen* _listScreenRef;

		NativeUI::VerticalLayout* _createCheckBoxGroup(const MAUtil::String categories[], int length);
		NativeUI::HorizontalLayout* _createBottomButtonBar();
		NativeUI::HorizontalLayout* _createAmountBars();
		NativeUI::HorizontalLayout* _createBottomSpacer();
		NativeUI::HorizontalLayout* _createDatePicker();
		NativeUI::HorizontalLayout* _createTimePicker();
		void _createTransactionInfoBox();
		void _createDescriptionBox();

		NativeUI::VerticalLayout* _mainLayout;
		NativeUI::Label* _categoryLabel;

		NativeUI::EditBox* _descriptionEditBox;
		NativeUI::EditBox* _transactionInformationEditBox;

		NativeUI::Button* _addButton;
		NativeUI::Button* _cancelButton;

		NativeUI::Slider* _amountSliderThousands;
		NativeUI::Slider* _amountSliderUnits;
		NativeUI::Label* _amountLabel;

		NativeUI::DatePicker* _datePicker;
		NativeUI::TimePicker* _timePicker;

		NativeUI::ToggleButton* _descriptionToggleButton;
		NativeUI::ToggleButton* _transactionInformationToggleButton;
		NativeUI::VerticalLayout* _descriptionBoxParent;
		NativeUI::VerticalLayout* _transactionInfoBoxParent;
		MAUtil::Vector<NativeUI::CheckBox*>* checkBoxVector;

		Logical::Observer* _observerReference;

		MAUtil::String _typeValue;

		MAUtil::String _coin;

		bool _launcedFromHomeScreen;
	};
}
#endif /* ADDINCOMEDIALOG_H_ */
