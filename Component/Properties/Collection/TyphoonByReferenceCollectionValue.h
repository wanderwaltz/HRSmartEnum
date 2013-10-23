////////////////////////////////////////////////////////////////////////////////
//
//  JASPER BLUES
//  Copyright 2013 Jasper Blues
//  All Rights Reserved.
//
//  NOTICE: Jasper Blues permits you to use, modify, and distribute this file
//  in accordance with the terms of the license agreement accompanying it.
//
////////////////////////////////////////////////////////////////////////////////



#import <Foundation/Foundation.h>
#import "TyphoonCollectionValue.h"

@interface TyphoonByReferenceCollectionValue : NSObject<TyphoonCollectionValue>

@property (nonatomic, strong, readonly) NSString* componentName;

- (id)initWithComponentName:(NSString*)componentName;


@end