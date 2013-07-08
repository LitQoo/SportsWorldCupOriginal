//
//  iphoneAppDelegate.h
//  iphone
//
//  Created by Walzer on 10-11-16.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Facebook.h"
#import "FBConnector.h"

@class RootViewController;
extern NSString *const FBSessionStateChangedNotification;



@interface AppController : NSObject <FBDialogDelegate,UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
@public
	string fbRValue;
	int fbDelekey;
	
@protected
	UIWindow *window;
    RootViewController	*viewController;
}
@property (retain, nonatomic) RootViewController* viewController;
@property (nonatomic, retain) Facebook *facebook;
@property   (retain,nonatomic)  UIWindow *window;
@end



