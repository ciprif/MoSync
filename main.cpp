#include <ma.h>
#include <mavsprintf.h>
#include <MAUtil/Moblet.h>
#include <NativeUI/TabScreen.h>
#include <NativeUI/PanoramaView.h>
#include <conprint.h>

#include "Screens/homeScreen.h"
#include "Screens/addExpenseDialog.h"
#include "Screens/addIncomeDialog.h"
#include "Screens/listScreen.h"
#include "Screens/settingsScreen.h"
#include "Screens/loadingScreen.h"
#include "Logical/observer.h"
#include "Logical/settingsManager.h"

using namespace MAUtil;

/**
 * Moblet to be used as a template for a Native UI application.
 */
class NativeUIMoblet : public Moblet
{
public:
	/**
	 * The constructor creates the user interface.
	 */
	NativeUIMoblet()
	{
		// create a new observer object.
		_observer = new Logical::Observer();

		// create the GUI.
		createUI();
	}

	/**
	 * Destructor.
	 */
	virtual ~NativeUIMoblet()
	{
		delete _homeScreen;
		delete _parentScreen;
		delete _observer;
	}

	/**
	 * Create the user interface.
	 */
	void createUI()
	{
		GUI::LoadingScreen loadingScreen;
		loadingScreen.show();
		// the parent container is a panoramaView for WP7 and a TabScreen for Android and iOS

		char buffer[BUFF_SIZE];
		maGetSystemProperty("mosync.device.OS", buffer, BUFF_SIZE);

		lprintfln("%s", buffer);

		_listScreen = new GUI::ListScreen();
		_listScreen->setTitle("Transactions");
		_homeScreen = new GUI::HomeScreen();
		_homeScreen->setTitle("Home");
		_settingsScreen = new GUI::SettingsScreen();
		_settingsScreen->setTitle("Settings");

		_observer->setHomeScreenRef(_homeScreen);
		_observer->setListScreenRef(_listScreen);
		_observer->setSettingsScreenRef(_settingsScreen);

		if(strcmp(buffer, "iOS") == 0 || strcmp(buffer, "Android") == 0)
		{
			_parentScreen = new NativeUI::TabScreen();
			_parentScreen->setTitle("MyBudget");

			((NativeUI::TabScreen*)_parentScreen)->addTab(_homeScreen);
			((NativeUI::TabScreen*)_parentScreen)->addTab(_listScreen);
			((NativeUI::TabScreen*)_parentScreen)->addTab(_settingsScreen);
		}
		else
		{
			_parentScreen = new NativeUI::PanoramaView();
			_parentScreen->setTitle("MyBudget");

			((NativeUI::PanoramaView*) _parentScreen)->addScreen(_homeScreen);
			((NativeUI::PanoramaView*) _parentScreen)->addScreen(_listScreen);
			((NativeUI::PanoramaView*) _parentScreen)->addScreen(_settingsScreen);
		}

		//Show the screen
		_parentScreen->show();

		_incomesDialog = new GUI::AddIncomeDialog();
		_expensesDialog = new GUI::AddExpenseDialog();

		_homeScreen->setAddExpensesDialogReference(_expensesDialog);
		_homeScreen->setAddIncomesDialogReference(_incomesDialog);
		_listScreen->setAddExpensesDialogReference(_expensesDialog);
		_listScreen->setAddIncomeDialogReference(_incomesDialog);
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

private:
	NativeUI::Screen* _parentScreen;
    GUI::HomeScreen* _homeScreen;
    GUI::ListScreen* _listScreen;
    GUI::SettingsScreen* _settingsScreen;
    GUI::AddExpenseDialog* _expensesDialog;
    GUI::AddIncomeDialog* _incomesDialog;

    Logical::Observer* _observer;
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new NativeUIMoblet());
	return 0;
}
