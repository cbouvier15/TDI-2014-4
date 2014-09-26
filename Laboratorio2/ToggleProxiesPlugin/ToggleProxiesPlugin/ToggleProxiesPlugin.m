//
//  ToggleProxiesPlugin.m
//  ToggleProxiesPlugin
//
//  Created by Marc Silverman on 12/2/6.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ToggleProxiesPlugin.h"
#import <Adium/AIAccountControllerProtocol.h>
#import <Adium/AIMenuControllerProtocol.h>
#import <Adium/AIAccount.h>
#import <AIUtilities/AIStringAdditions.h>
#import <AIUtilities/AIMenuAdditions.h>
#import <Adium/AIMessageEntryTextView.h>
#import "GCDAsyncUdpSocket.h"
#import "AsyncUdpSocket.h"

GCDAsyncUdpSocket *udpSocket;
AsyncUdpSocket *udpSocketReceiver;

@interface ToggleProxiesPlugin()

@end

@implementation ToggleProxiesPlugin

- (void)installPlugin
{
	NSMenuItem *rootMenu = [[NSMenuItem alloc] initWithTitle:(@"Insertar Emotion")
												target:self
												action:@selector(getEmoticon:)
										 keyEquivalent:@"z"];
    
	[rootMenu setKeyEquivalentModifierMask:(NSCommandKeyMask | NSAlternateKeyMask)];
	
	[adium.menuController addMenuItem:rootMenu toLocation:LOC_File_Additions];
    
	[rootMenu release];
    
    // Receive emotion from of_expressions_detector_app.
    NSError *error = nil;
    udpSocketReceiver = [[AsyncUdpSocket alloc] initWithDelegate:self];
    
    if (![udpSocketReceiver bindToPort:11888 error:&error]){
        NSLog(@"No bindea.");
        return;
    }
    
    NSLog(@"Bindea.");
    [udpSocketReceiver receiveWithTimeout:-1 tag:0];
    NSLog(@"Recibe.");
}

/*!
 * @brief Uninstall Plugin
 */
- (void)uninstallPlugin
{

}

/*!
 * @brief Turns all proxies off
 */
- (void)getEmoticon:(NSMenuItem *)menuItem
{
    // Send sync signal to of_expressions_detector_app.
    NSLog(@"Sending...");
    udpSocket = [[GCDAsyncUdpSocket alloc] initWithDelegate:self delegateQueue:dispatch_get_main_queue()];
    NSData *data = [[NSString stringWithFormat:@"a"] dataUsingEncoding:NSUTF8StringEncoding];
    [udpSocket sendData:data toHost:@"127.0.0.1" port:11999 withTimeout:-1 tag:1];
    NSLog(@"SYN Sent.");
}

- (BOOL)onUdpSocket:(AsyncUdpSocket *)sock
     didReceiveData:(NSData *)data
            withTag:(long)tag
           fromHost:(NSString *)host
               port:(UInt16)port
{
    
    NSLog(@"Recibe algo porque entra porque entra a onUdpSocket");
    
	NSString *msg = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
	if (msg){
        NSLog(msg);
	}
	else{
		NSLog(@"Error converting received data into UTF-8 String");
	}
    
    [udpSocketReceiver sendData:data toHost:@"127.0.0.1" port:11999 withTimeout:-1 tag:0];
	[udpSocketReceiver receiveWithTimeout:-1 tag:0];
    
	return YES;
}

@end

