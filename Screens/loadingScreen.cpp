/*
 * loadingScreen.cpp
 *
 *  Created on: Aug 2, 2012
 *      Author: Cipri
 */


#include <NativeUI/Screen.h>
#include <NativeUI/VerticalLayout.h>
#include <NativeUI/ActivityIndicator.h>
#include <NativeUI/Label.h>

#include "loadingScreen.h"

namespace GUI
{
	LoadingScreen::LoadingScreen()
	{
		NativeUI::VerticalLayout* mainLayout = new NativeUI::VerticalLayout();
		NativeUI::ActivityIndicator* activityIndicator = new NativeUI::ActivityIndicator();
		NativeUI::Label* loading = new NativeUI::Label("Loading...");

		NativeUI::VerticalLayout* spacer1;
		NativeUI::VerticalLayout* spacer2;

		loading->setTextHorizontalAlignment(MAW_ALIGNMENT_CENTER);
		loading->fillSpaceHorizontally();
		activityIndicator->fillSpaceHorizontally();

		spacer1 = new NativeUI::VerticalLayout();
		spacer1->fillSpaceVertically();

		spacer2 = new NativeUI::VerticalLayout();
		spacer2->fillSpaceVertically();

		mainLayout->addChild(spacer1);
		mainLayout->addChild(loading);
		mainLayout->addChild(activityIndicator);
		mainLayout->addChild(spacer2);

		activityIndicator->show();

		this->setMainWidget(mainLayout);
	}

	LoadingScreen::~LoadingScreen() {}
}
