/*
 * addExpenseDialog.cpp
 *
 *  Created on: Jun 19, 2012
 *      Author: Cipri
 */

#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/RelativeLayout.h>
#include <NativeUI/Label.h>
#include <NativeUI/CheckBox.h>
#include <NativeUI/EditBox.h>
#include <NativeUI/ImageButton.h>
#include <NativeUI/Button.h>
#include <NativeUI/Slider.h>
#include <NativeUI/ToggleButton.h>
#include <NativeUI/TimePicker.h>
#include <NativeUI/DatePicker.h>

#include "../Logical/observer.h"
#include "addExpenseDialog.h"
#include "homeScreen.h"
#include "listScreen.h"
#include "MAHeaders.h"

#include "../Logical/settingsManager.h"

namespace GUI
{
	AddExpenseDialog::AddExpenseDialog()
	{
		_coin = DEFAULT_COIN;
		_availableBudget = MAX_VALUE;
		_acceptedDept = MAX_VALUE;
		checkBoxVector = NULL;
		_createUI();
	}
	AddExpenseDialog::AddExpenseDialog(const double& availableBudget, const double& posibleDeptValue, const MAUtil::String& coin)
	{
		_coin = coin;
		_availableBudget = availableBudget;
		_acceptedDept = posibleDeptValue;
		checkBoxVector = NULL;
		_createUI();
	}

	AddExpenseDialog::~AddExpenseDialog()
	{
		_addButton->removeButtonListener(this);
		_cancelButton->removeButtonListener(this);
		_descriptionToggleButton->removeToggleButtonListener(this);
		_imageAtachementToggleButton->removeToggleButtonListener(this);
		_amountSlider->removeSliderListener(this);
		_selectImageButton->removeButtonListener(this);
		_captureImageButton->removeButtonListener(this);

		MAUtil::Vector<NativeUI::CheckBox*>::iterator it;
		for(it = checkBoxVector->begin(); it != checkBoxVector->end(); it++)
		{
			(*it)->removeCheckBoxListener(this);
		}
	}

	void AddExpenseDialog::_createUI()
	{
		NativeUI::VerticalLayout* parent = new NativeUI::VerticalLayout();
		parent->setHeight(DIALOG_HEIGHT);
		_mainLayout = new NativeUI::VerticalLayout();
		_mainLayout->setScrollable(true);

		_mainLayout->addChild(_createCheckBoxGroup());
		_mainLayout->addChild(_createAmountBar((int)(_availableBudget + _acceptedDept)));

		_mainLayout->addChild(_createDatePicker());
		_mainLayout->addChild(_createTimePicker());

		_createDescriptionBox();
		_mainLayout->addChild(_descriptionBoxParent);

		_createImageBox();
		_mainLayout->addChild(_imageBoxAndToggleLayout);
		_mainLayout->addChild(_createBottomSpacer());
		_mainLayout->addChild(_createBottomButtonBar());

		parent->addChild(_mainLayout);
		setMainWidget(parent);
	}

	NativeUI::VerticalLayout* AddExpenseDialog::_createCheckBoxGroup()
	{
		if(NULL == checkBoxVector)
			checkBoxVector = new MAUtil::Vector<NativeUI::CheckBox*>();

		NativeUI::VerticalLayout* checkBoxGroupParentLayout = new NativeUI::VerticalLayout();

		NativeUI::Label* categoryLabel = new NativeUI::Label();
		categoryLabel->setText("Choose a category:");
		categoryLabel->setFontSize(DIALOG_FONT_SIZE);

		checkBoxGroupParentLayout->addChild(categoryLabel);
		NativeUI::HorizontalLayout* checkBoxLabelLayout;
		for(int i = 0; i < Model::NO_OF_CATEGORIES; i++)
		{
			checkBoxLabelLayout = new NativeUI::HorizontalLayout();
			checkBoxLabelLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
			checkBoxLabelLayout->fillSpaceHorizontally();

			NativeUI::Label* checkBoxLabel = new NativeUI::Label();
			NativeUI::CheckBox* checkBox = new NativeUI::CheckBox();
			checkBox->addCheckBoxListener(this);
			checkBoxLabel->setText(Model::CATEGORY_LIST[i]);
			checkBoxLabel->setFontSize(DIALOG_FONT_SIZE);
			checkBoxLabelLayout->addChild(checkBox);
			checkBoxLabelLayout->addChild(checkBoxLabel);
			checkBoxGroupParentLayout->addChild(checkBoxLabelLayout);
			checkBoxVector->add(checkBox);
		}

		return checkBoxGroupParentLayout;
	}

	NativeUI::HorizontalLayout* AddExpenseDialog::_createBottomButtonBar()
	{
		NativeUI::HorizontalLayout* buttonGroup = new NativeUI::HorizontalLayout();
		buttonGroup->fillSpaceHorizontally();
		buttonGroup->wrapContentVertically();
		buttonGroup->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);

		_addButton = new NativeUI::Button();
		_cancelButton = new NativeUI::Button();

		_addButton->setText("Save");
		_addButton->setWidth(DIALOG_BUTTON_WIDTH);
		_addButton->addButtonListener(this);
		_cancelButton->setText("Cancel");
		_cancelButton->setWidth(DIALOG_BUTTON_WIDTH);
		_cancelButton->addButtonListener(this);

		buttonGroup->addChild(_addButton);
		buttonGroup->addChild(_cancelButton);

		return buttonGroup;
	}

	NativeUI::HorizontalLayout* AddExpenseDialog::_createAmountBar(const int& maxVal)
	{
		NativeUI::HorizontalLayout* amountBar = new NativeUI::HorizontalLayout();
		amountBar->fillSpaceHorizontally();

		NativeUI::VerticalLayout* labelSliderParentLayout = new NativeUI::VerticalLayout();
		labelSliderParentLayout->fillSpaceHorizontally();
		labelSliderParentLayout->fillSpaceVertically();

		_amountSlider = new NativeUI::Slider();
		_amountSlider->addSliderListener(this);
		_amountSlider->setMaximumValue(maxVal);
		_amountSlider->setValue(0);
		_amountSlider->fillSpaceHorizontally();

		_amountLabel = new NativeUI::Label();
		_amountLabel->fillSpaceHorizontally();
		_amountLabel->setFontSize(DIALOG_FONT_SIZE);
		_amountLabel->setText("Set the value of your expense: ");

		labelSliderParentLayout->addChild(_amountLabel);
		labelSliderParentLayout->addChild(_amountSlider);

		amountBar->addChild(labelSliderParentLayout);

		return amountBar;
	}

	void AddExpenseDialog::_createDescriptionBox() //this is void because _descriptionBoxParent needs to be accessed from another function call. A return for NativeUI::VerticalLayout* would be redundant
	{
		_descriptionBoxParent = new NativeUI::VerticalLayout();

		NativeUI::VerticalLayout* descritionToggleAndLabelParent = new NativeUI::VerticalLayout();
		NativeUI::Label* descriptionToggleLabel = new NativeUI::Label();

		_descriptionToggleButton = new NativeUI::ToggleButton();
		_descriptionToggleButton->fillSpaceHorizontally();
		_descriptionToggleButton->setCheckedState(false);

		_descriptionEditBox = new NativeUI::EditBox();

		descriptionToggleLabel->setText("Description:");
		descriptionToggleLabel->setFontSize(DIALOG_FONT_SIZE);

		descritionToggleAndLabelParent->addChild(descriptionToggleLabel);
		descritionToggleAndLabelParent->addChild(_descriptionToggleButton);

		//_descriptionEditBox->setMaxLines(DESCRIPTION_EDIT_BOX_LINES);
		_descriptionEditBox->setHeight(DESCRIPTION_EDIT_BOX_HEIGHT);
		_descriptionEditBox->fillSpaceHorizontally();

		_descriptionToggleButton->addToggleButtonListener(this);

		_descriptionBoxParent->addChild(descritionToggleAndLabelParent);
	}

	void AddExpenseDialog::_createImageBox()
	{
		NativeUI::VerticalLayout* imageToggleAndLabelParent = new NativeUI::VerticalLayout();
		NativeUI::Label* imageToggleLabel = new NativeUI::Label();

		_imageBoxAndToggleLayout = new NativeUI::VerticalLayout();
		_imageButtonsParentLayout = new NativeUI::HorizontalLayout();
		_imageAtachementToggleButton = new NativeUI::ToggleButton();

		_imageAtachementToggleButton->fillSpaceHorizontally();
		_imageAtachementToggleButton->setCheckedState(false);
		_imageAtachementToggleButton->addToggleButtonListener(this);

		_captureImageButton = new NativeUI::ImageButton();
		_selectImageButton = new NativeUI::ImageButton();

		_selectImageButton->addButtonListener(this);
		_captureImageButton->addButtonListener(this);

		imageToggleLabel->setText("Atach an image:");
		imageToggleLabel->setFontSize(DIALOG_FONT_SIZE);

		imageToggleAndLabelParent->addChild(imageToggleLabel);
		imageToggleAndLabelParent->addChild(_imageAtachementToggleButton);

		NativeUI::RelativeLayout* captureButtonWrapper = new NativeUI::RelativeLayout();
		NativeUI::RelativeLayout* selectButtonWrapper = new NativeUI::RelativeLayout();

		captureButtonWrapper->setSize(100, 120);
		selectButtonWrapper->setSize(100, 120);

		NativeUI::Label* captureLabel = new NativeUI::Label();
		NativeUI::Label* selectLabel = new NativeUI::Label();

		captureLabel->setText("Capture");
		captureLabel->setTopPosition(80);
		captureLabel->setWidth(100);
		captureLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_TOP);

		selectLabel->setText("Select");
		selectLabel->setTopPosition(80);
		selectLabel->setWidth(100);
		selectLabel->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);

		_captureImageButton->setSize(100, 100);
		_captureImageButton->setTopPosition(0);
		_selectImageButton->setSize(100, 100);
		_selectImageButton->setTopPosition(0);

		_captureImageButton->setBackgroundImage(RES_BUTTON_CAMERA);
		_selectImageButton->setBackgroundImage(RES_BUTTON_FOLDER);

		captureButtonWrapper->addChild(_captureImageButton);
		captureButtonWrapper->addChild(captureLabel);
		selectButtonWrapper->addChild(_selectImageButton);
		selectButtonWrapper->addChild(selectLabel);

		NativeUI::HorizontalLayout* spacer = new NativeUI::HorizontalLayout();
		spacer->setWidth(100);

		_imageButtonsParentLayout->addChild(captureButtonWrapper);
		_imageButtonsParentLayout->addChild(spacer);
		_imageButtonsParentLayout->addChild(selectButtonWrapper);
		_imageButtonsParentLayout->fillSpaceVertically();
		_imageButtonsParentLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_CENTER);

		_imageBoxAndToggleLayout->addChild(imageToggleAndLabelParent);
	}

	NativeUI::HorizontalLayout* AddExpenseDialog::_createDatePicker()
	{
		NativeUI::HorizontalLayout* datePickerBar = new NativeUI::HorizontalLayout();
		datePickerBar->fillSpaceHorizontally();

		NativeUI::VerticalLayout* labelDPParentLayout = new NativeUI::VerticalLayout();
		labelDPParentLayout->fillSpaceHorizontally();
		labelDPParentLayout->fillSpaceVertically();

		_datePicker = new NativeUI::DatePicker();
		_datePicker->fillSpaceHorizontally();

		_datePicker->setDayOfMonth(Logical::DEFAULT_DAY);
		_datePicker->setMonth(Logical::DEFAULT_MONTH);
		_datePicker->setYear(Logical::DEFAULT_YEAR);

		NativeUI::Label* datePickerLabel = new NativeUI::Label();
		datePickerLabel->setFontSize(DIALOG_FONT_SIZE);
		datePickerLabel->setText("Chose a date:");

		labelDPParentLayout->addChild(datePickerLabel);
		labelDPParentLayout->addChild(_datePicker);

		datePickerBar->addChild(labelDPParentLayout);

		return datePickerBar;
	}

	NativeUI::HorizontalLayout* AddExpenseDialog::_createTimePicker()
	{
		NativeUI::HorizontalLayout* timePickerBar = new NativeUI::HorizontalLayout();
		timePickerBar->fillSpaceHorizontally();

		NativeUI::VerticalLayout* labelTPParentLayout = new NativeUI::VerticalLayout();
		labelTPParentLayout->fillSpaceHorizontally();
		labelTPParentLayout->fillSpaceVertically();

		_timePicker = new NativeUI::TimePicker();
		_timePicker->fillSpaceHorizontally();

		NativeUI::Label* timePickerLabel = new NativeUI::Label();
		timePickerLabel->setFontSize(DIALOG_FONT_SIZE);
		timePickerLabel->setText("Chose a time:");

		labelTPParentLayout->addChild(timePickerLabel);
		labelTPParentLayout->addChild(_timePicker);

		timePickerBar->addChild(labelTPParentLayout);

		return timePickerBar;
	}
	NativeUI::HorizontalLayout* AddExpenseDialog::_createBottomSpacer()
	{
		NativeUI::HorizontalLayout* line = new NativeUI::HorizontalLayout();
		line->fillSpaceHorizontally();
		line->setHeight(DIALOG_BOTTOM_LINE_HEIGHT);

		return line;
	}

	void AddExpenseDialog::setHomeScreenRef(HomeScreen* homeScreenRef)
	{
		_homeScreenRef = homeScreenRef;
		//at this part the dialog is loaded
		_homeScreenRef->addExpensesDialogLoaded();
	}

	void AddExpenseDialog::setListScreenRef(ListScreen* listScreenRef)
	{
		_listScreenRef = listScreenRef;
	}

	void AddExpenseDialog::buttonClicked(Widget* button)
	{
		if(button == _addButton)
		{
			this->hide();
			if(_launcedFromHomeScreen) _homeScreenRef->createOptionsMenu();
			else _listScreenRef->createOptionsMenu();

			NativeUI::Date d = _datePicker->getDate();

			Model::DateStruct date;
			date._year = d.year;
			date._mounth = d.month;
			date._day = d.day;

			Model::TimeStruct time;
			time._hour = _timePicker->getHour();
			time._minutes = _timePicker->getMinute();

			_observerReference->requestExpenseAddition((double)_amountSlider->getValue(), _categoryValue, _descriptionEditBox->getText(), "", date, time);
		}
		else if(button == _cancelButton)
		{
			this->hide();
			if(_launcedFromHomeScreen) _homeScreenRef->createOptionsMenu();
			else _listScreenRef->createOptionsMenu();
		}
		else
		{
			maAlert("Alert!", "Feature is not yet available. Please wait for a future update.", NULL, NULL, NULL);
		}
	}

	void AddExpenseDialog::checkBoxStateChanged(NativeUI::CheckBox* checkBox, bool state)
	{
		if(state == true)
		{
			checkBox->setState(true);
			for(int i = 0; i < checkBoxVector->size(); i++)
			{
				if((*checkBoxVector)[i] != checkBox)
					(*checkBoxVector)[i]->setState(false);
				else
				{
					_categoryValue = Model::CATEGORY_LIST[i];
				}
			}
		}
		if(state == false)
		{
			checkBox->setState(true);
		}
	}

	void AddExpenseDialog::toggleButtonStateChanged(NativeUI::ToggleButton* toggleButton, bool state)
	{
		if(toggleButton == _descriptionToggleButton)
		{
			if(true == state)
			{
				_descriptionBoxParent->addChild(_descriptionEditBox);
			}
			else
			{
				_descriptionBoxParent->removeChild(_descriptionEditBox);
				_descriptionEditBox->setText("");
			}
		}
		else if(toggleButton == _imageAtachementToggleButton)
		{
			if(true == state)
			{
				_imageBoxAndToggleLayout->addChild(_imageButtonsParentLayout);
			}
			else
			{
				_imageBoxAndToggleLayout->removeChild(_imageButtonsParentLayout);
			}
		}
	}

	void AddExpenseDialog::show()
	{
		lprintfln("show");
		MAUtil::Vector<NativeUI::CheckBox*>::iterator it;
		for(it = checkBoxVector->begin(); it != checkBoxVector->end(); it++)
		{
			NativeUI::CheckBox* cb = *it;
			cb->setState(false);
		}

		it = checkBoxVector->begin();
		(*it)->setState(true);


		_amountSlider->setValue(0);
		_descriptionToggleButton->setCheckedState(false);
		_imageAtachementToggleButton->setCheckedState(false);

		_imageBoxAndToggleLayout->removeChild(_imageButtonsParentLayout);
		_descriptionBoxParent->removeChild(_descriptionEditBox);
		_descriptionEditBox->setText("");

		if(_launcedFromHomeScreen) _homeScreenRef->removeOptionsMenu();
		else _listScreenRef->removeOptionsMenu();

		NativeUI::Dialog::show();
	}

	void AddExpenseDialog::setObserver(Logical::Observer* obs)
	{
		_observerReference = obs;
	}

	void AddExpenseDialog::sliderValueChanged(NativeUI::Slider* s, int sliderValue)
	{
		if(_amountSlider == s)
		{
			char budgetString[BUFFER_SIZE];
			sprintf(budgetString, "The value of the expense: %d %s", sliderValue, _coin.c_str());

			_amountLabel->setText(budgetString);
		}
	}

	void AddExpenseDialog::setAvailableBudget(const double& availableBudget)
	{
		_availableBudget = availableBudget;
	}

	void AddExpenseDialog::updateAmountSliderValue()
	{
		_amountSlider->setMaximumValue((int)(_availableBudget + _acceptedDept));
	}

	void AddExpenseDialog::setAcceptedDebtValue(const double& value)
	{
		_acceptedDept = value;
	}

	void AddExpenseDialog::setCoin(const MAUtil::String& coin)
	{
		_coin = coin;
	}

	void AddExpenseDialog::setLaunchedFromHomeScreen(bool value)
	{
		_launcedFromHomeScreen = value;
	}
}
