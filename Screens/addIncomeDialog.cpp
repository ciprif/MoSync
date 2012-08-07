/*
 * addIncomeDialog.cpp
 *
 *  Created on: Jul 3, 2012
 *      Author: Cipri
 */

#include <NativeUI/HorizontalLayout.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/RelativeLayout.h>
#include <NativeUI/Label.h>
#include <NativeUI/CheckBox.h>
#include <NativeUI/EditBox.h>
#include <NativeUI/Button.h>
#include <NativeUI/Slider.h>
#include <NativeUI/ToggleButton.h>
#include <NativeUI/TimePicker.h>
#include <NativeUI/DatePicker.h>

#include "../Logical/observer.h"
#include "addIncomeDialog.h"
#include "homeScreen.h"
#include "listScreen.h"
#include "MAHeaders.h"

#include "../Logical/settingsManager.h"

namespace GUI
{
	AddIncomeDialog::AddIncomeDialog() : checkBoxVector(NULL)
	{
		 _coin = GUI::DEFAULT_COIN;
		_createUI();
	}
	AddIncomeDialog::AddIncomeDialog(const MAUtil::String& coin) : checkBoxVector(NULL)
	{
		_coin = coin;
		_createUI();
	}

	AddIncomeDialog::~AddIncomeDialog()
	{
		_addButton->removeButtonListener(this);
		_cancelButton->removeButtonListener(this);
		_descriptionToggleButton->removeToggleButtonListener(this);
		_transactionInformationToggleButton->removeToggleButtonListener(this);
		_amountSliderThousands->removeSliderListener(this);
		_amountSliderUnits->removeSliderListener(this);

		MAUtil::Vector<NativeUI::CheckBox*>::iterator it;
		for(it = checkBoxVector->begin(); it != checkBoxVector->end(); it++)
		{
			(*it)->removeCheckBoxListener(this);
		}
	}

	void AddIncomeDialog::_createUI()
	{
		NativeUI::VerticalLayout* parent = new NativeUI::VerticalLayout();
		parent->setHeight(DIALOG_HEIGHT);
		_mainLayout = new NativeUI::VerticalLayout();
		_mainLayout->setScrollable(true);

		_mainLayout->addChild(_createCheckBoxGroup(Model::INCOME_TYPES_LIST, Model::NO_OF_INCOMES));
		_mainLayout->addChild(_createAmountBars());

		_mainLayout->addChild(_createDatePicker());
		_mainLayout->addChild(_createTimePicker());

		_createDescriptionBox();
		_mainLayout->addChild(_descriptionBoxParent);

		_createTransactionInfoBox();
		_mainLayout->addChild(_transactionInfoBoxParent);
		_mainLayout->addChild(_createBottomSpacer());
		_mainLayout->addChild(_createBottomButtonBar());

		parent->addChild(_mainLayout);
		setMainWidget(parent);
	}

	NativeUI::VerticalLayout* AddIncomeDialog::_createCheckBoxGroup(const MAUtil::String categories[], int length)
	{
		if(NULL == checkBoxVector)
			checkBoxVector = new MAUtil::Vector<NativeUI::CheckBox*>();

		NativeUI::VerticalLayout* checkBoxGroupParentLayout = new NativeUI::VerticalLayout();

		NativeUI::Label* categoryLabel = new NativeUI::Label();
		categoryLabel->setText("Choose a type:");
		categoryLabel->setFontSize(DIALOG_FONT_SIZE);
		NativeUI::HorizontalLayout* checkBoxLabelLayout;

		checkBoxGroupParentLayout->addChild(categoryLabel);
		for(int i = 0; i < length; i++)
		{
			checkBoxLabelLayout = new NativeUI::HorizontalLayout();
			checkBoxLabelLayout->setChildHorizontalAlignment(MAW_ALIGNMENT_LEFT);
			checkBoxLabelLayout->fillSpaceHorizontally();

			NativeUI::Label* checkBoxLabel = new NativeUI::Label();
			NativeUI::CheckBox* checkBox = new NativeUI::CheckBox();
			checkBox->addCheckBoxListener(this);

			checkBoxLabel->setText(categories[i]);
			checkBoxLabel->setFontSize(DIALOG_FONT_SIZE);

			checkBoxLabelLayout->addChild(checkBox);
			checkBoxLabelLayout->addChild(checkBoxLabel);

			checkBoxGroupParentLayout->addChild(checkBoxLabelLayout);

			checkBoxVector->add(checkBox);
		}

		return checkBoxGroupParentLayout;
	}

	NativeUI::HorizontalLayout* AddIncomeDialog::_createBottomButtonBar()
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

	NativeUI::HorizontalLayout* AddIncomeDialog::_createAmountBars()
	{
		NativeUI::HorizontalLayout* amountBar = new NativeUI::HorizontalLayout();
		amountBar->fillSpaceHorizontally();
		NativeUI::VerticalLayout* labelSliderParentLayout = new NativeUI::VerticalLayout();

		NativeUI::VerticalLayout* labelSliderParentLayoutThousands = new NativeUI::VerticalLayout();
		labelSliderParentLayoutThousands->fillSpaceHorizontally();
		labelSliderParentLayoutThousands->fillSpaceVertically();
		NativeUI::VerticalLayout* labelSliderParentLayoutUnits = new NativeUI::VerticalLayout();
		labelSliderParentLayoutUnits->fillSpaceHorizontally();
		labelSliderParentLayoutUnits->fillSpaceVertically();

		NativeUI::Label* _amountThousandsLabel = new NativeUI::Label();
		NativeUI::Label* _amountUnitsLabel = new NativeUI::Label();

		_amountThousandsLabel->setText("Thousands");
		_amountUnitsLabel->setText("Units");
		_amountThousandsLabel->setFontSize(DIALOG_FONT_SIZE_SMALL);
		_amountUnitsLabel->setFontSize(DIALOG_FONT_SIZE_SMALL);
		_amountThousandsLabel->fillSpaceHorizontally();
		_amountUnitsLabel->fillSpaceHorizontally();

		_amountSliderThousands = new NativeUI::Slider();
		_amountSliderThousands->addSliderListener(this);
		_amountSliderThousands->setMaximumValue(30);
		_amountSliderThousands->setValue(0);
		_amountSliderThousands->fillSpaceHorizontally();

		_amountSliderUnits = new NativeUI::Slider();
		_amountSliderUnits->addSliderListener(this);
		_amountSliderUnits->setMaximumValue(111);
		_amountSliderUnits->setValue(0);
		_amountSliderUnits->fillSpaceHorizontally();

		_amountLabel = new NativeUI::Label();
		_amountLabel->fillSpaceHorizontally();
		_amountLabel->setFontSize(DIALOG_FONT_SIZE);
		_amountLabel->setText("Set the value of your income: ");

		labelSliderParentLayout->addChild(_amountLabel);
		labelSliderParentLayout->addChild(_amountThousandsLabel);
		labelSliderParentLayout->addChild(_amountSliderThousands);
		labelSliderParentLayout->addChild(_amountUnitsLabel);
		labelSliderParentLayout->addChild(_amountSliderUnits);

		amountBar->addChild(labelSliderParentLayout);

		return amountBar;
	}

	void AddIncomeDialog::_createDescriptionBox() //this is void because _descriptionBoxParent needs to be accessed from another function call. A return for NativeUI::VerticalLayout* would be redundant
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

	void AddIncomeDialog::_createTransactionInfoBox()
	{
		_transactionInfoBoxParent = new NativeUI::VerticalLayout();

		NativeUI::VerticalLayout* transactionToggleAndLabelParent = new NativeUI::VerticalLayout();
		NativeUI::Label* transactionToggleLabel = new NativeUI::Label();

		_transactionInformationToggleButton = new NativeUI::ToggleButton();
		_transactionInformationToggleButton->fillSpaceHorizontally();
		_transactionInformationToggleButton->setCheckedState(false);

		_transactionInformationEditBox = new NativeUI::EditBox();

		transactionToggleLabel->setText("Transaction info:");
		transactionToggleLabel->setFontSize(DIALOG_FONT_SIZE);

		transactionToggleAndLabelParent->addChild(transactionToggleLabel);
		transactionToggleAndLabelParent->addChild(_transactionInformationToggleButton);

		//_transactionInformationEditBox->setMaxLines(DESCRIPTION_EDIT_BOX_LINES);
		_transactionInformationEditBox->setHeight(DESCRIPTION_EDIT_BOX_HEIGHT);
		_transactionInformationEditBox->setEditMode(NativeUI::EDIT_BOX_MODE_TEXT);
		_transactionInformationEditBox->fillSpaceHorizontally();

		_transactionInformationToggleButton->addToggleButtonListener(this);

		_transactionInfoBoxParent->addChild(transactionToggleAndLabelParent);
	}

	NativeUI::HorizontalLayout* AddIncomeDialog::_createDatePicker()
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

		_datePicker->addDatePickerListener(this);

		NativeUI::Label* datePickerLabel = new NativeUI::Label();
		datePickerLabel->setFontSize(DIALOG_FONT_SIZE);
		datePickerLabel->setText("Chose a date:");

		labelDPParentLayout->addChild(datePickerLabel);
		labelDPParentLayout->addChild(_datePicker);

		datePickerBar->addChild(labelDPParentLayout);

		return datePickerBar;
	}

	NativeUI::HorizontalLayout* AddIncomeDialog::_createTimePicker()
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
	NativeUI::HorizontalLayout* AddIncomeDialog::_createBottomSpacer()
	{
		NativeUI::HorizontalLayout* line = new NativeUI::HorizontalLayout();
		line->fillSpaceHorizontally();
		line->setHeight(DIALOG_BOTTOM_LINE_HEIGHT);

		return line;
	}

	void AddIncomeDialog::setHomeScreenRef(HomeScreen* parent)
	{
		_homeScreenRef = parent;
		//at this part the dialog is loaded
		_homeScreenRef->addIncomesDialogLoaded();
	}

	void AddIncomeDialog::setListScreenRef(ListScreen* listScreenRef)
	{
		_listScreenRef = listScreenRef;
	}


	void AddIncomeDialog::buttonClicked(Widget* button)
	{
		if(button == _addButton)
		{
			//@todo change the path from hardcoded version to actual value
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

			double value = _amountSliderThousands->getValue() * 1000 + _amountSliderUnits->getValue()*9;

			_observerReference->requestIncomeAddition(value, _typeValue, _descriptionEditBox->getText(), _transactionInformationEditBox->getText(), date, time);
		}
		else if(button == _cancelButton)
		{
			this->hide();
			if(_launcedFromHomeScreen) _homeScreenRef->createOptionsMenu();
			else _listScreenRef->createOptionsMenu();
		}
	}

	void AddIncomeDialog::checkBoxStateChanged(NativeUI::CheckBox* checkBox, bool state)
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
					_typeValue = Model::INCOME_TYPES_LIST[i];
				}
			}
		}
		if(state == false)
		{
			checkBox->setState(true);
		}
	}

	void AddIncomeDialog::toggleButtonStateChanged(NativeUI::ToggleButton* toggleButton, bool state)
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
		else if(toggleButton == _transactionInformationToggleButton)
		{
			if(true == state)
			{
				_transactionInfoBoxParent->addChild(_transactionInformationEditBox);
			}
			else
			{
				_transactionInfoBoxParent->removeChild(_transactionInformationEditBox);
				_transactionInformationEditBox->setText("");
			}
		}
	}

	void AddIncomeDialog::show()
	{
		MAUtil::Vector<NativeUI::CheckBox*>::iterator it;
		for(it = checkBoxVector->begin(); it != checkBoxVector->end(); it++)
		{
			NativeUI::CheckBox* cb = *it;
			cb->setState(false);
		}

		it = checkBoxVector->begin();
		(*it)->setState(true);

		_amountSliderThousands->setValue(0);
		_amountSliderUnits->setValue(0);

		_descriptionToggleButton->setCheckedState(false);
		_transactionInformationToggleButton->setCheckedState(false);

		_descriptionBoxParent->removeChild(_descriptionEditBox);
		_descriptionEditBox->setText("");
		_transactionInfoBoxParent->removeChild(_transactionInformationEditBox);
		_transactionInformationEditBox->setText("");

		NativeUI::Dialog::show();

		if(_launcedFromHomeScreen) _homeScreenRef->removeOptionsMenu();
		else _listScreenRef->removeOptionsMenu();
	}

	void AddIncomeDialog::setObserver(Logical::Observer* obs)
	{
		_observerReference = obs;
	}

	void AddIncomeDialog::sliderValueChanged(NativeUI::Slider* s, int sliderValue)
	{
		if(_amountSliderThousands == s)
		{
			char budgetString[BUFFER_SIZE];
			sprintf(budgetString, "The value of the income: %d %s", sliderValue*1000 + _amountSliderUnits->getValue()*9, _coin.c_str());

			_amountLabel->setText(budgetString);
		}
		else if(_amountSliderUnits == s)
		{
			char budgetString[BUFFER_SIZE];
			sprintf(budgetString, "The value of the income: %d %s", _amountSliderThousands->getValue()*1000 + sliderValue*9, _coin.c_str());

			_amountLabel->setText(budgetString);
		}
	}

	void AddIncomeDialog::setCoin(const MAUtil::String& coin)
	{
		_coin = coin;
	}

	void AddIncomeDialog::setLaunchedFromHomeScreen(bool value)
	{
		_launcedFromHomeScreen = value;
	}

	void AddIncomeDialog::datePickerValueChanged(NativeUI::DatePicker* datePicker, const NativeUI::Date& selectedDate)
	{

	}
}
