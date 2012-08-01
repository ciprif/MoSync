/*
 * addExpenseDialog.h
 *
 *  Created on: Jun 19, 2012
 *      Author: Cipri
 */

#ifndef ADDEXPENSEDIALOG_H_
#define ADDEXPENSEDIALOG_H_

#include <NativeUI/Dialog.h>
#include <NativeUI/CheckBoxListener.h>
#include <NativeUI/ButtonListener.h>
#include <NativeUI/ToggleButtonListener.h>
#include <NativeUI/SliderListener.h>
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
	class ImageButton;
	class Button;
	class Slider;
	class ToggleButton;
	class DatePicker;
	class TimePicker;
}

namespace Logical
{
	class Observer;
}

namespace GUI
{
	class HomeScreen;
	class ListScreen;

	class AddExpenseDialog : public NativeUI::Dialog, public NativeUI::CheckBoxListener, public NativeUI::ButtonListener, public NativeUI::ToggleButtonListener, NativeUI::SliderListener
	{
	public:
		AddExpenseDialog();
		AddExpenseDialog(const double& availableBudget, const double& posibleDeptValue, const MAUtil::String& coin);
		~AddExpenseDialog();

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

		void setAvailableBudget(const double& availableBudget);

		void setAcceptedDebtValue(const double& value);
		void updateAmountSliderValue();

		void setCoin(const MAUtil::String& coin);

		void setLaunchedFromHomeScreen(bool value);
	private:
		void _createUI();

		HomeScreen* _homeScreenRef;
		ListScreen* _listScreenRef;

		NativeUI::VerticalLayout* _createCheckBoxGroup();
		NativeUI::HorizontalLayout* _createBottomButtonBar();
		NativeUI::HorizontalLayout* _createAmountBar(const int& maxVal);
		NativeUI::HorizontalLayout* _createBottomSpacer();
		NativeUI::HorizontalLayout* _createDatePicker();
		NativeUI::HorizontalLayout* _createTimePicker();
		void _createImageBox();
		void _createDescriptionBox();

		NativeUI::VerticalLayout* _mainLayout;
		NativeUI::Label* _categoryLabel;

		NativeUI::ImageButton* _selectImageButton;
		NativeUI::ImageButton* _captureImageButton;
		NativeUI::EditBox* _descriptionEditBox;

		NativeUI::Button* _addButton;
		NativeUI::Button* _cancelButton;

		NativeUI::Slider* _amountSlider;
		NativeUI::Label* _amountLabel;

		NativeUI::DatePicker* _datePicker;
		NativeUI::TimePicker* _timePicker;

		NativeUI::ToggleButton* _descriptionToggleButton;
		NativeUI::ToggleButton* _imageAtachementToggleButton;
		NativeUI::VerticalLayout* _descriptionBoxParent;
		NativeUI::HorizontalLayout* _imageButtonsParentLayout;
		NativeUI::VerticalLayout* _imageBoxAndToggleLayout;
		MAUtil::Vector<NativeUI::CheckBox*>* checkBoxVector;

		Logical::Observer* _observerReference;

		MAUtil::String _categoryValue;

		double _availableBudget;
		double _acceptedDept;
		MAUtil::String _coin;

		bool _launcedFromHomeScreen;
	};
}

#endif /* ADDEXPENSEDIALOG_H_ */
