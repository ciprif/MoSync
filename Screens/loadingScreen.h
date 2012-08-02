/*
 * loadingScreen.h
 *
 *  Created on: Aug 2, 2012
 *      Author: Cipri
 */

#ifndef LOADINGSCREEN_H_
#define LOADINGSCREEN_H_

#include <NativeUI/Screen.h>

namespace NativeUI
{
	class VerticalLayout;
	class ActivityIndicator;
	class Label;
}

namespace GUI
{
	class LoadingScreen : public NativeUI::Screen
	{
	public:
		LoadingScreen();
		~LoadingScreen();
	};
}
#endif /* LOADINGSCREEN_H_ */
