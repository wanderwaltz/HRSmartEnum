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
#import "TyphoonTypeConverter.h"

/**
* A 'type converter' for NSString and NSMutableString.
*/
@interface TyphoonPassThroughTypeConverter : NSObject <TyphoonTypeConverter>
{
    BOOL _isMutable;
}

- (id)initWithIsMutable:(BOOL)isMutable;

@end