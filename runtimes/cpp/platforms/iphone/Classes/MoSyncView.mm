/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#import "MoSyncView.h"
#include "MosyncMain.h"

//#include <helpers/CPP_IX_ACCELEROMETER.h>

//#include "iphone_helpers.h"
//#include "Platform.h"

@interface UIApplication(MyExtras) 
- (void)terminateWithSuccess; 
@end

@interface MessageBoxHandler : UIViewController <UIAlertViewDelegate> {
	BOOL kill;
	NSString *title;
	NSString *msg;
}

@property BOOL kill;
@property (copy, nonatomic) NSString* title;
@property (copy, nonatomic) NSString* msg;

- (void)alertViewCancel:(UIAlertView *)alertView;

@end

@implementation MessageBoxHandler

@synthesize kill;
@synthesize title;
@synthesize msg;

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	if(kill)
		MoSync_Exit();
}

- (void)alertViewCancel:(UIAlertView *)alertView {
	// don't know if this is allowed...
	if(kill)
		MoSync_Exit();
}

@end

@interface TextBoxData : UIViewController <UIAlertViewDelegate> {
	NSString *title;
	NSString *inText;
	wchar* outText;
	int maxSize;
	int constraints;
	UITextField *textField;
}

@property (copy, nonatomic) NSString* title;
@property (copy, nonatomic) NSString* inText;
@property wchar* outText;
@property int maxSize;
@property int constraints;
@property (assign, nonatomic) UITextField* textField;
@end

@implementation TextBoxData 

@synthesize title;
@synthesize inText;
@synthesize outText;
@synthesize maxSize;
@synthesize constraints;
@synthesize textField;

- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	MAEvent event;
	event.type = EVENT_TYPE_TEXTBOX;
	if(buttonIndex == 1) {
		int length = MIN(textField.text.length, maxSize-1);
		[textField.text getCharacters:(unichar*)outText range:NSMakeRange(0, length)];
		outText[length] = '\0';
		event.textboxResult = MA_TB_RES_OK;
		event.textboxLength = length;
	} else {
		event.textboxResult = MA_TB_RES_CANCEL;
	}
	Base::gEventQueue.put(event);		
}

@end

@implementation MoSyncView

- (void)updateMoSyncView:(CGImageRef)ref {
	mosyncView = ref;
	[self performSelectorOnMainThread : @ selector(setNeedsDisplay) withObject:nil waitUntilDone:NO];
}

-(void) startUpdatingLocation {
	[locationController.locationManager startUpdatingLocation];
}

-(void) stopUpdatingLocation {
	[locationController.locationManager stopUpdatingLocation];
}

/*
#define kUpdateFrequency 10  // Hz
-(void) startUpdatingAccelerometer {
	[[UIAccelerometer sharedAccelerometer] setUpdateInterval:(20.0 / kUpdateFrequency)];
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];	
}

-(void) stopUpdatingAccelerometer {
	[[UIAccelerometer sharedAccelerometer] setDelegate:nil];	
}

- (void)accelerometer:(UIAccelerometer *)accelerometer didAccelerate:(UIAcceleration *)acceleration {
	MAEvent event;
	event.type = EVENT_TYPE_ACCELEROMETER;
	MAAccelerometer* accData = new MAAccelerometer;
	event.data = accData;
	
	accData->roll = acceleration.x;
	accData->pitch = acceleration.y;
	accData->yaw = acceleration.z;
	
	Base::gEventQueue.put(event);
}
 */

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        // Initialization code
		self.clearsContextBeforeDrawing = NO;
		UIDevice* myDevice = [UIDevice currentDevice];
		[myDevice beginGeneratingDeviceOrientationNotifications];
		[myDevice setBatteryMonitoringEnabled:YES];
		
		[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationChanged:)
													 name:UIDeviceOrientationDidChangeNotification object:nil];
		
		self.frame.origin.y = 0;
		self.frame.origin.x = 0;
		mosyncView = nil;
        // Initialization code
		CGRect appFrame = [[UIScreen mainScreen] bounds];
		CGFloat screenHeight = appFrame.size.height;
		CGFloat screenWidth = appFrame.size.width;
		
		locationController = [[MoSyncCLController alloc] init];
		
		self.multipleTouchEnabled = YES;
		touchHelper = [[TouchHelper alloc] init];
		
		//MoSyncMain(self.frame.size.width, screenHeight, self);
		MoSync_Main(screenWidth, screenHeight, self);
    }
    return self;
}

- (void)drawRect:(CGRect)rect {
	if(mosyncView == nil) return;
	
    CGContextRef context = UIGraphicsGetCurrentContext();
	CGContextSetInterpolationQuality(context, kCGInterpolationNone);
	CGContextSetAllowsAntialiasing(context, false);
	CGContextTranslateCTM(context, 0, CGImageGetHeight(mosyncView));
	CGContextScaleCTM(context, 1.0, -1.0);
	CGContextDrawImage(context, rect, mosyncView);		
	
	MoSync_DoneUpdatingView();	 
}


- (void)dealloc {
    [super dealloc];
    [locationController release];	
}

- (void)deviceOrientationChanged:(NSNotification *)notification {
	//UIDeviceOrientation deviceOrientation = [UIDevice currentDevice].orientation;
	MoSync_AddScreenChangedEvent();
}

/*
int addTouch(UITouch* touch) {
	if(touchArray == nil) {
		touchArray = [[NSMutableArray alloc] init];
		dummyTouch = [[UITouch alloc] init];
	}
	
	int index = 0;
	for (UITouch* cur in touchArray) {
		if(touch == cur) {
			return -1;
		} else if(cur == dummyTouch) {
			[touchArray replaceObjectAtIndex:index withObject:touch];
			return index;
		}
		
		index++;
	}
	
	[touchArray addObject:touch];
	return index;
	
}

int getTouchId(UITouch* touch) {
	return [touchArray indexOfObject:touch];
}

void removeTouch(UITouch* touch) {
	//[touchArray removeObject:touch];
	int index = 0;
	for (UITouch* cur in touchArray) {
		if(cur == touch) {
			[touchArray replaceObjectAtIndex: index withObject:dummyTouch];
			return;
		}
		index++;
	}
	
}
 */

- (void)viewAppeared {
	// some touches might not have been registered so let's clear the touch helper.
    [touchHelper clearTouches];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
	for (UITouch *touch in touches) 
	{
		if(touch.phase ==  UITouchPhaseBegan) {
			CGPoint point = [touch locationInView:self];
			int touchId = [touchHelper addTouch: touch];
			MoSync_AddTouchPressedEvent(point.x, point.y, touchId);	
		}
	}	
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
	for (UITouch *touch in touches) 
	{
		if(touch.phase ==  UITouchPhaseMoved) {
			CGPoint point = [touch locationInView:self];
			int touchId = [touchHelper getTouchId: touch];
			MoSync_AddTouchMovedEvent(point.x, point.y, touchId);
		}
	}	
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event {	
    for (UITouch *touch in touches) 
	{
		if(touch.phase ==  UITouchPhaseEnded) {	
			CGPoint point = [touch locationInView:self];
			int touchId = [touchHelper getTouchId: touch];		
			MoSync_AddTouchReleasedEvent(point.x, point.y, touchId);
			[touchHelper removeTouch: touch];
		}
	}
}

-(void) messageBox:(id) obj {
	MessageBoxHandler *mbh = (MessageBoxHandler*) obj;
	UIAlertView *alert = [[UIAlertView alloc] 
                          initWithTitle:mbh.title
                          message:mbh.msg
                          delegate:mbh
                          cancelButtonTitle:@"OK"
                          otherButtonTitles:nil];
	
	[touchHelper clearTouches];
	
    [alert show];
    [alert release];
}
 
-(void) showMessageBox: (NSString*)msg withTitle: (NSString*)title shouldKill: (bool)kill {
	MessageBoxHandler *mbh = [MessageBoxHandler alloc];
	mbh.kill = kill;
	mbh.title = title;
	mbh.msg = msg;
	[self performSelectorOnMainThread: @ selector(messageBox:) withObject:(id)mbh waitUntilDone:NO];
}

-(void) textBox:(id) obj {
	TextBoxData *textBoxData = (TextBoxData*)obj;
	
	UIAlertView *textBoxAlert = [[UIAlertView alloc] initWithTitle:textBoxData.title message:@"\n\n" delegate:textBoxData cancelButtonTitle:NSLocalizedString(@"Cancel",nil) otherButtonTitles:NSLocalizedString(@"OK",nil), nil];
	
	/*
	UILabel *passwordLabel = [[UILabel alloc] initWithFrame:CGRectMake(12,40,260,25)];
	passwordLabel.font = [UIFont systemFontOfSize:16];
	passwordLabel.textColor = [UIColor whiteColor];
	passwordLabel.backgroundColor = [UIColor clearColor];
	passwordLabel.shadowColor = [UIColor blackColor];
	passwordLabel.shadowOffset = CGSizeMake(0,-1);
	passwordLabel.textAlignment = UITextAlignmentCenter;
	passwordLabel.text = @"Account Name";
	[passwordAlert addSubview:passwordLabel];
	*/
	//UIImageView *passwordImage = [[UIImageView alloc] initWithImage:[UIImage imageWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"passwordfield" ofType:@"png"]]];
	//passwordImage.frame = CGRectMake(11,79,262,31);
	//[passwordAlert addSubview:passwordImage];
	
	UITextField *textField = [[UITextField alloc] initWithFrame:CGRectMake(16,50,252,25)];
	textField.font = [UIFont systemFontOfSize:18];
	textField.backgroundColor = [UIColor whiteColor];
	textField.secureTextEntry = (textBoxData.constraints&MA_TB_FLAG_PASSWORD)?YES:NO; // password mode?
	textField.keyboardAppearance = UIKeyboardAppearanceAlert;
	textField.delegate = self;
	textField.text = textBoxData.inText;
	[textField becomeFirstResponder];
	[textBoxAlert addSubview:textField];
	
	textBoxData.textField = textField;
	
    [touchHelper clearTouches];
	
	//[textBoxAlert setTransform:CGAffineTransformMakeTranslation(0,109)];
	[textBoxAlert show];
	[textBoxAlert release];
	[textField release];
	
	//[passwordImage release];
	//[passwordLabel release];	
}

-(void) showTextBox:(NSString*)title 
		 withInText:(NSString*)inText 
			outText:(wchar*)outText 
			maxSize:(int)maxSize 
	 andConstraints:(int)constraints {
	
	TextBoxData *textBoxData = [TextBoxData alloc];
	textBoxData.title = title;
	textBoxData.inText = inText;
	textBoxData.outText = outText;
	textBoxData.maxSize = maxSize;
	textBoxData.constraints = constraints;
	
	[self performSelectorOnMainThread: @ selector(textBox:) withObject:(id)textBoxData waitUntilDone:NO];
}

@end
