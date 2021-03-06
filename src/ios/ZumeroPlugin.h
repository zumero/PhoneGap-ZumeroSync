//
//  ZumeroPlugin.h
//  Copyright 2013-2017 Sourcegear, LLC dba Zumero
//

#import <Cordova/CDV.h>

@interface ZumeroPlugin : CDVPlugin

- (void) sync:(CDVInvokedUrlCommand *)command;
- (void) sync2:(CDVInvokedUrlCommand *)command;
- (void) syncQuarantine:(CDVInvokedUrlCommand *)command;
- (void) quarantineSinceLastSync:(CDVInvokedUrlCommand *)command;
- (void) deleteQuarantine:(CDVInvokedUrlCommand *)command;
- (void) setupJSPassthrough:(CDVInvokedUrlCommand *)command;

@end
