//
//  ZumeroPlugin.m
//  Copyright 2013-2016 Sourcegear, LLC dba Zumero
//

#import "ZumeroPlugin.h"
#import <ZumeroSync/ZumeroSync.h>

@interface ZumeroPlugin() {
	NSMutableDictionary *callbacksMap;
	NSMutableDictionary *statementsQueue;
	NSMutableDictionary *autocommits;
	NSMutableDictionary *intx;
	dispatch_queue_t txqueue;
}
@end

@implementation ZumeroPlugin

- (void)pluginInitialize
{
	[super pluginInitialize];
	callbacksMap = [[NSMutableDictionary dictionary] retain];
	statementsQueue = [[NSMutableDictionary dictionary] retain];
	autocommits = [[NSMutableDictionary dictionary] retain];
	txqueue = dispatch_queue_create("com.zumero.txqueue", NULL);
	dispatch_retain(txqueue);
}

- (void)dispose
{
	[callbacksMap release];
	[statementsQueue release];
	[autocommits release];
	dispatch_release(txqueue);
		
	[super dispose];
}

#pragma mark - sync


- (void) setupJSPassthrough:(CDVInvokedUrlCommand *)command
{
	// no-op here, needed for Android
}

- (void) sync:(CDVInvokedUrlCommand *)command
{
	NSString *path = [command argumentAtIndex:0];
	NSString *key = [command argumentAtIndex:1];
	NSString *serverUrl = [command argumentAtIndex:2];
	NSString *dbFileName = [command argumentAtIndex:3];
	NSString *scheme = [command argumentAtIndex:4];
	NSString *user = [command argumentAtIndex:5];
	NSString *password = [command argumentAtIndex:6];
	
	dispatch_async(txqueue, ^{
		NSError *err = nil;
		
		BOOL ok = [ZumeroSync Sync:path cipherKey:key serverUrl:serverUrl remote:dbFileName authSchemeJS:scheme user:user password:password error:&err];
		
		CDVPluginResult* pluginResult = nil;
		if (ok)
			pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
		else
			pluginResult = [self errorResult:err];
		
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
		});
	});
}

- (void) sync2:(CDVInvokedUrlCommand *)command
{
	NSString *path = [command argumentAtIndex:0];
	NSString *key = [command argumentAtIndex:1];
	NSString *serverUrl = [command argumentAtIndex:2];
	NSString *dbFileName = [command argumentAtIndex:3];
	NSString *scheme = [command argumentAtIndex:4];
	NSString *user = [command argumentAtIndex:5];
	NSString *password = [command argumentAtIndex:6];
	NSNumber *callbackToken = [command argumentAtIndex:7];
    __block CDVPlugin * me = self;
    
	dispatch_async(txqueue, ^{
		NSError *err = nil;
		ZumeroProgressCallback cb = nil;
		if (callbackToken != nil)
		{
			cb = ^(int cancellationToken, int phase, zumero_int64 bytesSoFar, zumero_int64 bytesTotal, void * object) 
			{
                dispatch_async(dispatch_get_main_queue(), ^{
                [self.commandDelegate evalJs:[NSString stringWithFormat:@"zumero_global_progress_callback_function(%d, %d, %d, %lld, %lld)", [callbackToken intValue], cancellationToken, phase, bytesSoFar, bytesTotal] ];
                });
                
			};
		}
        BOOL ok = [ZumeroSync Sync:path cipherKey:key serverUrl:serverUrl remote:dbFileName authSchemeJS:scheme user:user password:password callback:cb dataPointer:NULL error:&err];
		
		CDVPluginResult* pluginResult = nil;
		if (ok)
			pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
		else
			pluginResult = [self errorResult:err];
		
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
		});
	});
}

- (CDVPluginResult *)errorResult:(NSError *)err
{
	NSInteger code = err ? [err code] : -1;
	NSNumber *numCode = [NSNumber numberWithInteger:code];
	NSString *msg = err ? [err localizedDescription] : @"unknown error";
	
	NSDictionary *result = @{ @"code": numCode, @"message":msg };
	
	return [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsDictionary:result];
}

- (void) syncQuarantine:(CDVInvokedUrlCommand *)command
{
	NSString *path = [command argumentAtIndex:0];
	NSString *key = [command argumentAtIndex:1];
	NSNumber *qid = [command argumentAtIndex:2];
	NSString *serverUrl = [command argumentAtIndex:3];
	NSString *dbFileName = [command argumentAtIndex:4];
	NSString *scheme = [command argumentAtIndex:5];
	NSString *user = [command argumentAtIndex:6];
	NSString *password = [command argumentAtIndex:7];

	dispatch_async(txqueue, ^{
		BOOL partial = TRUE;
		BOOL ok = TRUE;
		NSError *err = nil;
		
		while (ok && partial)
		{
			ok = [ZumeroSync SyncQuarantine:path cipherKey:key qid:(sqlite3_int64)[qid longLongValue] serverUrl:serverUrl remote:dbFileName authSchemeJS:scheme user:user password:password partial:&partial error:&err];
		}
		
		CDVPluginResult* pluginResult = nil;
		
		if (ok)
		{
			pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
		}
		else
		{
			pluginResult = [self errorResult:err];
		}
		
		dispatch_async(dispatch_get_main_queue(), ^{
			[self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
		});
	});
}

- (void) quarantineSinceLastSync:(CDVInvokedUrlCommand *)command
{
	NSString *fullpath = [command argumentAtIndex:0];
	NSString *key = nil;
	
	if ([[command arguments] count] > 1)
	{
		key = [command argumentAtIndex:1];
	}
	
	sqlite3_int64 qid = -1;
	CDVPluginResult* pluginResult = nil;
	
	NSError *err = nil;
	BOOL ok = [ZumeroSync QuarantineSinceLastSync:fullpath cipherKey:key pqid:&qid error:&err];
	
	if (ok)
	{
		NSDictionary *result = @{ @"quarantineID": [NSNumber numberWithLongLong:(long long)qid] };
		pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:result];
	}
	else
	{
		pluginResult = [self errorResult:err];
	}
		
	dispatch_async(dispatch_get_main_queue(), ^{
		[self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
	});
}

- (void) deleteQuarantine:(CDVInvokedUrlCommand *)command
{
	NSString *path = [command argumentAtIndex:0];
	NSString *key = [command argumentAtIndex:1];
	NSNumber *qid = [command argumentAtIndex:2];

	NSError *err = nil;
	CDVPluginResult* pluginResult = nil;
	
	BOOL ok = [ZumeroSync DeleteQuarantine:path cipherKey:key qid:(sqlite3_int64)[qid longLongValue] error:&err];
	
	if (ok)
	{
		pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
	}
	else
	{
		pluginResult = [self errorResult:err];
	}
	
	dispatch_async(dispatch_get_main_queue(), ^{
		[self.commandDelegate sendPluginResult:pluginResult callbackId:command.callbackId];
	});
}


@end
