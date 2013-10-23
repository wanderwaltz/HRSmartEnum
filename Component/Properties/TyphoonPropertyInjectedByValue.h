////////////////////////////////////////////////////////////////////////////////
//
//  JASPER BLUES
//  Copyright 2012 - 2013 Jasper Blues
//  All Rights Reserved.
//
//  NOTICE: Jasper Blues permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////


#import <Foundation/Foundation.h>
#import "TyphoonInjectedProperty.h"


@interface TyphoonPropertyInjectedByValue : NSObject <TyphoonInjectedProperty>
{
    NSString* _textValue;
}

@property (nonatomic, strong, readonly) NSString* name;
@property (nonatomic, readonly) TyphoonPropertyInjectionType type;
@property (nonatomic, strong, readonly) NSString* textValue;

- (id)initWithName:(NSString*)name value:(NSString*)value;


@end